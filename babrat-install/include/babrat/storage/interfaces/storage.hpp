#pragma once

#include <string>

#include "babrat/serialization/serializable.hpp"
#include <babrat/container/associative_container.hpp>

namespace babrat
{
    namespace storage
    {

        template<typename Key, typename Value>
        class IStorage : public Serializable {

        public:

            ~IStorage() noexcept override = default;

        public:

            virtual void addPool(
                    const std::string &poolName) = 0;

            virtual void addSchema(
                    const std::string &poolName,
                    const std::string &schemaName) = 0;

            virtual void addCollection(
                    const std::string &poolName,
                    const std::string &schemaName,
                    const std::string &collectionName) = 0;

        public:

            virtual void disposePool(
                    const std::string &poolName) = 0;

            virtual void disposeSchema(
                    const std::string &poolName,
                    const std::string &schemaName) = 0;

            virtual void disposeCollection(
                    const std::string &poolName,
                    const std::string &schemaName,
                    const std::string &collectionName) = 0;

        public:

            virtual void clearPool(
                    const std::string &poolName) = 0;

            virtual void clearSchema(
                    const std::string &poolName,
                    const std::string &schemaName) = 0;

            virtual void clearCollection(
                    const std::string &poolName,
                    const std::string &schemaName,
                    const std::string &collectionName) = 0;

        public:

            virtual void add(
                    const std::string &poolName,
                    const std::string &schemaName,
                    const std::string &collectionName,
                    Key const &key,
                    Value const &value) = 0;

            virtual void add(
                    const std::string &poolName,
                    const std::string &schemaName,
                    const std::string &collectionName,
                    Key  const &key,
                    Value &&value) = 0;

            virtual void update(
                    const std::string &poolName,
                    const std::string &schemaName,
                    const std::string &collectionName,
                    Key const &key,
                    Value const &value) = 0;

            virtual void update(
                    const std::string &poolName,
                    const std::string &schemaName,
                    const std::string &collectionName,
                    Key const &key,
                    Value &&value) = 0;

            virtual Value &search(
                    const std::string &poolName,
                    const std::string &schemaName,
                    const std::string &collectionName,
                    Key const &key) = 0;

            virtual std::vector<typename container::AssociativeContainer<Key, Value>::KeyValuePair> searchBetween(
                    const std::string &poolName,
                    const std::string &schemaName,
                    const std::string &collectionName,
                    Key  const &lowerBound,
                    Key const &upperBound,
                    bool lowerBoundInclusive,
                    bool upperBoundInclusive) = 0;

            virtual Value dispose(
                    const std::string &poolName,
                    const std::string &schemaName,
                    const std::string &collectionName,
                    Key const &key) = 0;

        public:

            virtual void addBatch(
                    const std::string &poolName,
                    const std::string &schemaName,
                    const std::string &collectionName,
                    const std::vector<typename container::AssociativeContainer<Key, Value>::KeyValuePair>& items) = 0;


            virtual std::vector<Value> disposeBatch(
                    const std::string &poolName,
                    const std::string &schemaName,
                    const std::string &collectionName,
                    const std::vector<Key>& items) = 0;

        public:

            virtual void reset() = 0;

        };

    }
}
