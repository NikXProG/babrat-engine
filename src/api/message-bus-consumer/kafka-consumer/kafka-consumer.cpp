#include <iostream>
#include <kafka/KafkaConsumer.h>
#include <csignal>
#include <memory>
#include <vector>
#include "../../message-bus-producer/kafka-producer/kafka-producer.cpp"
#include "sql_statement.h"
#include "ddl/create_table.h"
#include <nlohmann/json.hpp>
#include "command_invoker.h"
#include <magic_enum/magic_enum.hpp>

using json = nlohmann::json;

std::atomic_bool running = {true};

class MessageBusConsumer {

public:

    using MessageHandler = std::function<void(const std::string&)>;

    MessageBusConsumer(const std::string& brokers,
                     const std::unordered_map<std::string, MessageHandler>& topicHandlers)
        : running_(true),
          topicHandlers_(topicHandlers)
    {

        kafka::Properties props(
            {
            {"bootstrap.servers", {brokers}}
        });

        consumer_ = std::make_unique<kafka::clients::consumer::KafkaConsumer>(props);

        for (const auto item : topicHandlers) {
            const kafka::Topic topic = item.first;
            consumer_->subscribe({ topic});
        }

    }

    void run() {
        while (running_) {
            try {

                auto records = consumer_->poll(std::chrono::milliseconds(100));


                for (const auto& record: records) {
                    if (!record.error()) {

                        std::string contentType = "application/json";

                        const auto& headers = record.headers();
                        if (!headers.empty()) {
                            for (const auto& header : headers) {
                                if (header.key == "Content-Type") {
                                    contentType = header.value.toString();
                                    break;
                                }
                            }
                        }

                         if (contentType == "application/json") {
                             if (auto it = topicHandlers_.find(record.topic());
                                     it != topicHandlers_.end()) {
                                 it->second(record.value().toString());
                             }
                         }

                    } else {
                        std::cerr << "Consumer error: " << record.error().message() << std::endl;
                    }
                }

                consumer_->commitSync();
            } catch (const kafka::KafkaException& e) {
                std::cerr << "Kafka error: " << e.what() << std::endl;
            }
        }

        consumer_->close();
    }

    void stop() {
        running_ = false;
    }

private:
    std::atomic_bool running_;
    std::unique_ptr<kafka::clients::consumer::KafkaConsumer> consumer_;
    std::unordered_map<std::string, MessageHandler> topicHandlers_;
};


void StopRunning(int sig) {
    if (sig != SIGINT) return;

    if (running) {
        running = false;
    } else {
        signal(SIGINT, SIG_IGN);
    }
}

void from_json(const json& j, ColumnDefinition& col) {
    j.at("ColumnName").get_to(col.column_name);
    j.at("DataType").get_to(col.column_type);
    j.at("IsNullable").get_to(col.isNullable);
  /*  j.at("Default").get_to(col.Default);*/
    j.at("IsPrimaryKey").get_to(col.isPrimaryKey);
/*    j.at("IsUnique").get_to(col.IsUnique);*/
}

class SqlModelFactory {
public:
    using Creator = std::function<std::unique_ptr<SqlStatement>(const nlohmann::json&)>;

    static std::unique_ptr<SqlStatement> create(TypeModel type, const nlohmann::json& j) {
        auto it = creators().find(type);
        if (it != creators().end()) {
            return it->second(j);
        }
        throw std::invalid_argument("Unsupported model type");
    }

    static bool register_creator(TypeModel type, Creator creator) {
        creators()[type] = std::move(creator);
        return true;
    }

private:
    static std::unordered_map<TypeModel, Creator>& creators() {
        static std::unordered_map<TypeModel, Creator> instance;
        return instance;
    }
};
namespace {


    bool register_table_creator = SqlModelFactory::register_creator(
            TypeModel::CREATE_TABLE,
            [](const nlohmann::json& j) -> std::unique_ptr<SqlStatement> {

                return std::make_unique<TableQueryModel>(
                        "default_db",
                        "public",
                        j["Payload"]["TableName"],
                        j["Payload"]["Columns"].get<std::vector<ColumnDefinition>>()
                );
            }
    );

    bool register_= SqlModelFactory::register_creator(
            TypeModel::INSERT,
            [](const nlohmann::json& j) -> std::unique_ptr<SqlStatement> {

               /* std::cout << j["Payload"]["TableName"] << std::endl;

                for(auto& it : j["Payload"]["ColumnNames"].get<std::vector<std::string>>()){
                    std::cout << it << std::endl;
                }*/

                std::vector<std::vector<std::string>> rows;
                for (const auto& row : j["Payload"]["Rows"]) {
                    rows.push_back(row["Values"].get<std::vector<std::string>>());
                }

                return std::make_unique<InsertQueryModel>(
                        "default_db",
                        "public",
                        j["Payload"]["TableName"],
                        j["Payload"]["ColumnNames"].get<std::vector<std::string>>(),
                        rows
                );
            }
    );

    // Другие регистрации...
};




int main() {

    signal(SIGINT, StopRunning);

    DbType* db_ = new DbType();


    auto catalog = new Catalog(db_);

    catalog->register_defaults();

    auto context = new DatabaseContext(catalog, db_);

    auto* factory = new SqlCommandFactory(*context);

    auto invoker = new CommandInvoker(factory);

    auto new_pool = new CreateDatabase("default_db");

    invoker->executeCommand(TypeModel::CREATE_DATABASE,*new_pool);

    invoker->undoLastCommand(*new_pool);

    invoker->executeCommand(TypeModel::CREATE_DATABASE,*new_pool);

    auto new_schema = new CreateSchema("default_db", "public",
                                       DEFAULT_ROLE_NAME);

    invoker->executeCommand(TypeModel::CREATE_NAMESPACE,*new_schema);

    /* auto new_collection =
             new TableQueryModel(
                     test.db_name,
                     test.schema_name,
                     test.table_name,
                     test.columns);*/


    std::unordered_map<std::string, MessageBusConsumer::MessageHandler> handlers = {
        {"table-topic", [&](const std::string& msg) {


            const auto producer =
                    std::make_unique<MessageBusProducer>("127.0.0.1:9092");

            auto j = nlohmann::json::parse(msg);

            auto id = j["RequestId"];

            auto type_string = j["TypeModel"];


            auto sql_model_cast = [](const std::string& type_name) -> TypeModel {

                if (auto value = magic_enum::enum_cast<TypeModel>(type_name)) {
                    return *value;
                }

                throw std::invalid_argument("Invalid enum value: " + type_name);
            };

            auto type = sql_model_cast(type_string);

            try{

                auto model =
                        SqlModelFactory::create(type, j);

                try {
                    invoker->executeCommand(type, *model);

                } catch (const std::exception& ex) {
                    invoker->undoLastCommand(*model);
                    throw;
                }

                if(type == TypeModel::CREATE_TABLE){
                    print_range(
                            DB_DEFAULT_NAME,
                            CATALOG_NAME,
                            CATALOG_FIELD_NAME,
                            db_);

                }

                if(type == TypeModel::INSERT){
                    print_range(
                            "default_db",
                            "public",
                            j["Payload"]["TableName"],
                            db_);

                }

                json format_message = {
                        {"status", "Complete"},
                        {"timestamp", get_current_timestamp()},
                        {"message", "changes have been made"}
                };

                json response = {
                        {"RequestId", id},
                        {"TypeModel", type_string},
                        {"Payload",format_message}
                };

                producer->send(response.dump());

            }catch(const std::exception& ex){

                json format_message = {
                        {"status", "Error"},
                        {"timestamp", get_current_timestamp()},
                        {"message", ex.what()}
                };

                json error = {
                        {"RequestId", id},
                        {"TypeModel",type_string},
                        {"Payload", format_message}
                };

                producer->send(error.dump());
                std::cerr << ex.what() << std::endl;
            }




        }}
    };

    const auto consumer =
        std::make_unique<MessageBusConsumer>("127.0.0.1:9092", handlers);

    consumer->run();

    delete db_;

    return 0;
}