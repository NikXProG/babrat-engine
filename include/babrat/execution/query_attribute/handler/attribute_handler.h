
#ifndef BABRAT_ATTRIBUTE_HANDLER_H
#define BABRAT_ATTRIBUTE_HANDLER_H

#include "babrat/commands/query_attribute/attribute_ctx.h"
#include <memory>
#include <utility>

class AttributeHandler {
public:
    virtual ~AttributeHandler() = default;

    void setNext(std::shared_ptr<AttributeHandler> handler) {
        next_ = std::move(handler);
    }

    virtual void handle(AttributeContext& context) {
        if (next_) {
            next_->handle(context);
        }
    }

protected:

    std::shared_ptr<AttributeHandler> next_;

};

#endif //BABRAT_ATTRIBUTE_HANDLER_H
