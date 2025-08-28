#ifndef BABRAT_WHEREHANDLER_H
#define BABRAT_WHEREHANDLER_H

#include "attribute_handler.h"
#include "iostream"
#include "common/condition_evaluator.h"

class WhereHandler : public AttributeHandler {
public:

    void handle(AttributeContext& context) override {

        if (context.where_condition.has_value()) {

            ConditionEvaluator evaluator(context.columns_index_);

            context.vectors_.erase(
                    std::remove_if( context.vectors_.begin(),  context.vectors_.end(),
                                    [&](const auto& row) {
                                        return !evaluator.evaluate(row, context.where_condition.value());
                                    }),
                    context.vectors_.end()
            );

            PrintVisitor visitor(std::cout);

            for(auto it2 : context.columns_index_){
                std::cout << it2.first << " ";
            }
            std::cout << std::endl;

            for(auto it : context.vectors_){
                for(auto item : it){
                    if (item->isNull()){
                        std::cout << "<NULL>" << " ";
                        continue;
                    }
                    item->accept(visitor);
                }

                std::cout << std::endl;
            }


         }

         /*    for(auto it :  context.value){

                 for(auto item :  it.second){

                     if (item->getType() == BabratType::INT4){

                     }
                 }
                 std::cout << std::endl;
             }*/

        /*    context.value.erase(
                    std::remove_if(
                            context.value.begin(),
                            context.value.end(),
                                   [&](const auto& row) {
                                        return true;
                                    *//*   return !evaluator.evaluate(row, *context.where_condition);*//*
                                   }),
                    context.value.end());*/


            /*PrintVisitor visitor(std::cout);

            for(auto it2 : context.columns){
                std::cout << it2.column_name << " ";
            }
            std::cout << std::endl;

            for(auto it : context.value){
                for(auto item : it){
                    if (item->isNull()){
                        std::cout << "<NULL>" << " ";
                        continue;
                    }
                    item->accept(visitor);
                }

                std::cout << std::endl;
            }*/


/*

            ConditionEvaluator evaluator(context.);
*/


            /*context.rows.erase(
                    std::remove_if(context.rows.begin(), context.rows.end(),
                                   [&](const auto& row) {
                                       return !evaluator.evaluate(row, *context.where_condition);
                                   }),
                    context.rows.end()*/
           /* );*/


        AttributeHandler::handle(context);
    }



};

#endif //BABRAT_WHEREHANDLER_H
