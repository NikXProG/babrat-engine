#pragma once

#include <format>

#include "babrat_container_error.h"
#include "babrat_storage_error.h"


namespace babrat{

    namespace storage {


        template <typename F>
        decltype(auto) handleContainerOperation(const std::string& entityType, const std::optional<std::string>& name, F&& op)
        {
            try {
                return std::forward<F>(op)();
            } catch (const BabratContainerError& ex) {
                throw babrat::BabratStorageError(
                        name
                        ? std::format("{} with name `{}` failed: {}", entityType, *name, ex.what())
                        : std::format("{} key failed: {}", entityType, ex.what()));
            }
        }

        template <typename F>
        decltype(auto) handleSchemaOperation(const std::string& name, F&& op)
        {
            return handleContainerOperation("schema", name, std::forward<F>(op));
        }

        template <typename F>
        decltype(auto) handlePoolOperation(const std::string& name, F&& op)
        {
            return handleContainerOperation("pool", name, std::forward<F>(op));
        }

        template <typename F>
        decltype(auto) handleCollectionOperation(const std::string& name, F&& op)
        {
            return handleContainerOperation("collection", name, std::forward<F>(op));
        }

        template <typename F>
        decltype(auto) handleKeyCollectionOperation(F&& op)
        {
            return handleContainerOperation("collection", std::nullopt, std::forward<F>(op));
        }


    }

}