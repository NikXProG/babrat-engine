#ifndef BABRAT_SDASDAS_H
#define BABRAT_SDASDAS_H
#include <utility>

#include "testing_type.h"
#include "sql_command_factory.h"
/*


class DBConnect {

private:

    std::string token;

public:

    virtual void open(std::string login, std::string password, std::string db) = 0;

    virtual void close() = 0;


};


class DBConnection final : public DBConnect {

private:

    std::string token;

public:

     void open(std::string login, std::string password, std::string db) override {

     }

     void close() override {

     }

};


class DBController {

   SqlCommandFactory manager_;

   DBConnection conn_;

public:

    explicit DBController(DBConnection conn) :
            conn_(std::move(conn)),
            manager_() {}

public:

    DBController* createDatabase(std::string db_name) {
        return executeCreateDDLCommand(
                DdlCommandType::CREATE,
                CreateDatabase(std::move(db_name))
        );
    }

    DBController* createSchema(std::string db_parent_name,
                               std::string schema_name,
                               std::string role_name) {
        return executeCreateDDLCommand(
                DdlCommandType::CREATE,
                CreateSchema(
                        std::move(db_parent_name),
                        std::move(schema_name),
                        std::move(role_name)
                )
        );
    }

    DBController* createTable(std::string db_parent_name,
                              std::string schema_parent_name,
                              std::string table_name,
                              std::vector<ColumnDefinition> columns) {
        return executeCreateDDLCommand(
                DdlCommandType::CREATE,
                TableQueryModel(
                        std::move(db_parent_name),
                        std::move(schema_parent_name),
                        std::move(table_name),
                        std::move(columns)
                )
        );
    }

public:

    DBController* DropTable(std::string db_parent_name,
                              std::string schema_parent_name,
                              std::string table_name) {
        return executeCreateDDLCommand(
                DdlCommandType::DROP,
                TableQueryModel(
                        std::move(db_parent_name),
                        std::move(schema_parent_name),
                        std::move(table_name)
                )
        );
    }

private:

    template<typename Model>
    DBController* executeCreateDDLCommand(DdlCommandType type, Model&& model) {

        auto command = manager_.ddl()
                .create(std::forward<Model>(model));

        try {
            command->execute();
        } catch (...) {
            command->undo();
        }

        return this;
    }


public:

    DBController(const DBController&) = delete;
    DBController& operator=(const DBController&) = delete;


    DBController(DBController&&) noexcept = default;
    DBController& operator=(DBController&&) noexcept = default;


    ~DBController() = default;

};
*/


#endif //BABRAT_SDASDAS_H
