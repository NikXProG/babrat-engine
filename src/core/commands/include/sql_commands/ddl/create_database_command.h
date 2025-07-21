#ifndef BABRAT_CREATE_DATABASE_COMMAND_H
#define BABRAT_CREATE_DATABASE_COMMAND_H
#include "DDL/create_database.h"
#include "sql_command.h"

class CreateDatabaseCommand : public TypedSqlCommand<CreateDatabase, DdlCommandType> {

public:

    using TypedSqlCommand::TypedSqlCommand;

protected:

    void execute() override {

        switch(type_){
            case DdlCommandType::CREATE:
                std::cout << "hello CREATE";
                break;
            case DdlCommandType::DROP:
                std::cout << "hello DROP";
                break;
            case DdlCommandType::ALTER:
                std::cout << "hello ALTER";
                break;
            case DdlCommandType::TRUNCATE:
                std::cout << "hello TRUNCATE";
                break;
        }
   /*     if (type_ == DdlCommandType::CREATE){

        }
        context_.getCatalog()
            ->addDatabase(model_.db_name);

        context_.getDatabase()
            ->add_pool(model_.db_name);
*/
    }

    void undo() override {

        try{

            std::cout << "sdsd";


/*
            context_.getCatalog()
                    ->disposeDatabase(model_.db_name);

            context_.getDatabase()
                    ->dispose_pool(model_.db_name);*/

        }catch(...){
            // pass throw
        }

    }

};


#endif //BABRAT_CREATE_DATABASE_COMMAND_H
