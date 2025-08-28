#ifndef BABRAT_COMMAND_INVOKER_H
#define BABRAT_COMMAND_INVOKER_H
#include "command_factory.h"
#include <memory>
#include <vector>
#include <any>
#include <stdexcept>
#include <utility>
/*

class CommandInvoker final {

    CommandFactory* factory_;
    std::vector<std::unique_ptr<Command>> executed_commands_;
    size_t undo_limit_ = 100;

public:

    explicit CommandInvoker(CommandFactory* factory, size_t undo_limit = 100)
            : factory_(factory)
    {
        if (factory_ == nullptr) {
            throw std::invalid_argument("CommandFactory cannot be null");
        }
        executed_commands_.reserve(undo_limit_);
    }


    void executeCommand(TypeModel type, const SqlStatement& model) {
        auto cmd = factory_->createCommand(type);
        cmd->execute(model);

        if (undo_limit_ > 0 && executed_commands_.size() >= undo_limit_) {
            executed_commands_.erase(executed_commands_.begin());
        }

        executed_commands_.push_back(std::move(cmd));
    }

    void undoLastCommand(const SqlStatement& data) {
        if (executed_commands_.empty()) {
            throw std::runtime_error("No sql_commands to undo");
        }

        auto& cmd = executed_commands_.back();
        if (cmd->canUndo()) {
            cmd->undo(data);
        }
        executed_commands_.pop_back();
    }

    size_t getUndoCount() const noexcept {
        return executed_commands_.size();
    }

    bool canUndo() const noexcept {
        return !executed_commands_.empty();
    }

    void clear() noexcept {
        executed_commands_.clear();
    }

    void setUndoLimit(size_t limit) noexcept {
        undo_limit_ = limit;
        if (undo_limit_ > 0 && executed_commands_.size() > undo_limit_) {
            executed_commands_.erase(
                    executed_commands_.begin(),
                    executed_commands_.begin() + (executed_commands_.size() - undo_limit_)
            );
        }
    }

    size_t getUndoLimit() const noexcept {
        return undo_limit_;
    }

    ~CommandInvoker() = default;
};

*/


#endif //BABRAT_COMMAND_INVOKER_H
