
#ifndef BABRAT_COMMAND_PROCESSOR_BUILDER_H
#define BABRAT_COMMAND_PROCESSOR_BUILDER_H
#include "command_invoker.h"

class CommandProcessorBuilder {

    CommandFactory* factory_;
    babrat::db_storage<ColumnBase*, std::vector<ColumnBase*>, Traits>* db_;
    Catalog* catalog_;
    std::function<void(CommandFactory&)> init_func_;

    CommandProcessorBuilder* set_command_factory(
            std::function<void(CommandFactory&)> init_func)
            {
        init_func_ = std::move(init_func);
        return this;
    }

    CommandProcessorBuilder* set_catalog(
            Catalog* catalog)
    {
        catalog_ = std::move(catalog);
        return this;
    }


    CommandProcessor* Build(){
        return new CommandProcessor(
                std::move(factory_),
                std::move(db_),
                std::move(catalog_));
    }

};

#endif //BABRAT_COMMAND_PROCESSOR_BUILDER_H
