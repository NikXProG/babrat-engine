#ifndef BABRAT_ATTRIBUTEPIPELINEBUILDER_H
#define BABRAT_ATTRIBUTEPIPELINEBUILDER_H

#include "handler/where_attribute.h"

class AttributePipelineBuilder {
public:
    AttributePipelineBuilder() : first_(nullptr), last_(nullptr) {}

    AttributePipelineBuilder& addWhereHandler() {
        auto handler = std::make_shared<WhereHandler>();
        addHandler(handler);
        return *this;
    }

 /*   AttributePipelineBuilder& addOrderByHandler() {
        auto handler = std::make_shared<OrderByHandler>();
        addHandler(handler);
        return *this;
    }

    AttributePipelineBuilder& addLimitHandler() {
        auto handler = std::make_shared<LimitOffsetHandler>();
        addHandler(handler);
        return *this;
    }
*/


    std::shared_ptr<AttributeHandler> build() {
        return first_;
    }

private:

    void addHandler(std::shared_ptr<AttributeHandler> handler) {
        if (!first_) {
            first_ = handler;
            last_ = handler;
        } else {
            last_->setNext(handler);
            last_ = handler;
        }
    }

    std::shared_ptr<AttributeHandler> first_;
    std::shared_ptr<AttributeHandler> last_;

};


#endif //BABRAT_ATTRIBUTEPIPELINEBUILDER_H
