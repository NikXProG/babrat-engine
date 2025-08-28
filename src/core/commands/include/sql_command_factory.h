

#ifndef BABRAT_SQL_COMMAND_FACTORY_H
#define BABRAT_SQL_COMMAND_FACTORY_H
#include "command_factory.h"
/*
enum DdlCommandType {
    CREATE,
    DROP,
    ALTER,
    TRUNCATE
};*/

/*

enum SQLTypeFactory {
    DDL,
    DML,
    DQL,
    TCL,
    DCL
};


class ISqlCommandFactory {

public:

    virtual ~ISqlCommandFactory() = default;

public:

    virtual std::unique_ptr<Command> create(SqlStatement&& model) = 0;

};


class DdlCommandFactory : public ISqlCommandFactory {

    using Creator = std::function<std::unique_ptr<Command>(DatabaseContext& context,
                                                           DdlCommandType,
                                                           SqlStatement&&)>;

    std::unordered_map<std::type_index, Creator> creators_;

public:

    DdlCommandFactory(){
        registerCommands();
    }

public:

    std::unique_ptr<Command> create(DatabaseContext& context,
                                    DdlCommandType type,
                                    SqlStatement&& model) override {
        auto it = creators_.find(typeid(model));
        if (it != creators_.end()) {
            return it->second(context, type, std::move(model));
        }
        throw std::runtime_error("Unsupported model type");
    }


    template<typename Model>
    void registerCommand(Creator creator) {
        creators_[typeid(Model)] = std::move(creator);
    }

    void registerCommands(){


     */
/*   registerCommand<CreateDatabase>([](
                DatabaseContext&& context,
                DdlCommandType type,
                SqlStatement&& model) {
            return std::make_unique<CreateDatabaseCommand>(
                    context, type, std::move(model));
        });
*//*


     */
/*   registerCommand<TableQueryModel>([](
                DatabaseContext&& context,
                DdlCommandType type,
                SqlStatement&& model) {
            return std::make_unique<CreateTableCommand>(context, type, std::move(model));
        });*//*


      */
/*  registerCommand<CreateSchema>([](DdlCommandType type, SqlStatement&& model) {
            return std::make_unique<SchemaSqlCommandTest>(type,
                                                          std::move(dynamic_cast<CreateSchema&>(model)));
        });

        registerCommand<CreateDatabase>([](DdlCommandType type, SqlStatement&& model) {
            return std::make_unique<DatabaseCommandTest>(type,
                                                         std::move(dynamic_cast<CreateDatabase&>(model)));
        });*//*

    }


};





class DmlCommandFactory : public ISqlCommandFactory {

    using Creator = std::function<std::unique_ptr<Command>(SqlStatement&&)>;

    std::unordered_map<std::type_index, Creator> creators_;

public:

    DmlCommandFactory(){
        registerCommands();
    }

public:

    template<typename Model>
    void registerCommand(Creator creator) {
        creators_[typeid(Model)] = std::move(creator);
    }

public:

    std::unique_ptr<Command> create(SqlStatement&& model) override {
        auto it = creators_.find(typeid(model));
        if (it != creators_.end()) {
            return it->second( std::move(model));
        }
        throw std::runtime_error("Unsupported model type");
    }


private:

    void registerCommands(){

      */
/*  registerCommand<InsertQueryModel>([](SqlStatement& context) {
            return std::make_unique<InsertCommand>(std::move(context));
        });*//*



    }


};




class SqlCommandFactory {

public:

    SqlCommandFactory() {
        registerFactory(SQLTypeFactory::DDL, std::make_unique<DdlCommandFactory>());
        registerFactory(SQLTypeFactory::DML, std::make_unique<DmlCommandFactory>());
    }

public:

    ISqlCommandFactory& getFactory(SQLTypeFactory type) {
        auto it = factories_.find(type);
        if (it != factories_.end()) {
            return *it->second;
        }
        throw std::runtime_error("Factory type not registered");
    }

public:

    DdlCommandFactory& ddl() {
        return dynamic_cast<DdlCommandFactory&>(getFactory(SQLTypeFactory::DDL));
    }

    DmlCommandFactory& dml() {
        return dynamic_cast<DmlCommandFactory&>(getFactory(SQLTypeFactory::DML));
    }

public:

    SqlCommandFactory(const SqlCommandFactory&) = delete;
    SqlCommandFactory& operator=(const SqlCommandFactory&) = delete;

    SqlCommandFactory(SqlCommandFactory&&) = default;
    SqlCommandFactory& operator=(SqlCommandFactory&&) = default;

private:

    void registerFactory(SQLTypeFactory type, std::unique_ptr<ISqlCommandFactory> factory) {
        factories_[type] = std::move(factory);
    }

private:

    std::unordered_map<SQLTypeFactory, std::unique_ptr<ISqlCommandFactory>> factories_;


};
*/



/*template<typename SQLTypeFactory>
class SqlCommandFactory : public CommandFactory<SQLTypeFactory, SqlStatement> {

    using CommandCreator = std::function<std::unique_ptr<Command<SqlStatement>>(DatabaseContext&)>;

public:

    explicit SqlCommandFactory(DatabaseContext& context)
            : context_(context) {
        registerCommands();
    }

    void registerCommand(TypeModel type,
                         CommandCreator creator) override {
        commands_.try_emplace(type, std::move(creator));
    }

    std::unique_ptr<Command<SqlStatement>> createCommand(TypeModel type) const override {
        if (auto it = commands_.find(type); it != commands_.end()) {
            return it->second(context_);
        }
        throw std::runtime_error(std::format("Command `{}` not found",
                                             magic_enum::enum_name(type)));
    }

    template<typename T>
    void AutoRegisterCommand(TypeModel type) {
        registerCommand(type, [](DatabaseContext& ctx) {
            return std::make_unique<T>(ctx);
        });
    }

private:

    void registerCommands() {
        AutoRegisterCommand<CreateDatabaseCommand>(TypeModel::CREATE_DATABASE);
        AutoRegisterCommand<CreateNamespaceCommand>(TypeModel::CREATE_NAMESPACE);
        AutoRegisterCommand<CreateTableCommand>(TypeModel::CREATE_TABLE);
        AutoRegisterCommand<InsertCommand>(TypeModel::INSERT);
        AutoRegisterCommand<DeleteCommand>(TypeModel::DELETE);
        AutoRegisterCommand<DropDatabaseCommand>(TypeModel::DROP_DATABASE);
        AutoRegisterCommand<DropTableCommand>(TypeModel::DROP_TABLE);
        AutoRegisterCommand<DropNamespaceCommand>(TypeModel::DROP_NAMESPACE);
    }

private:

    DatabaseContext& context_;
    std::unordered_map<TypeModel, CommandCreator> commands_;

};*/
#endif //BABRAT_SQL_COMMAND_FACTORY_H
