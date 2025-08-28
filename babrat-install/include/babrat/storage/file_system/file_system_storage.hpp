#pragma once

#include <babrat/storage/interfaces/storage.hpp>
#include <babrat/common/not_implemented_error.hpp>

namespace babrat {

    namespace storage {

        template<typename Key, typename Value, typename Compare = std::less<Key>>
        class FileSystemStorage final : public IStorage<Key,Value> {

        public:

            explicit InMemoryStorage() {

            }

        public:

            ~InMemoryStorage() noexcept override = default;

        public:


            void addPool(
                    const std::string &pool_name) override
            {

                throw NotImplementedError("method addPool()");

            }

            void addSchema(
                    const std::string &pool_name,
                    const std::string &schema_name) override
            {

                throw NotImplementedError("method addSchema()");

            }

            void addCollection(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name) override
            {

                throw NotImplementedError("method addCollection()");

            }

        public:

            void clearPool(
                    const std::string &pool_name) override
            {

                throw NotImplementedError("method clearPool()");

            }

            void clearSchema(
                    const std::string &pool_name,
                    const std::string &schema_name) override
            {

                throw NotImplementedError("method clearSchema()");


            }

            void clearCollection(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name) override {

                throw NotImplementedError("method clearCollection()");

            }

        public:

            void disposePool(
                    const std::string &pool_name) override
            {


                throw NotImplementedError("method disposePool()");

            }

            void disposeSchema(
                    const std::string &pool_name,
                    const std::string &schema_name) override
            {

                throw NotImplementedError("method disposeSchema()");

            }

            void disposeCollection(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name) override
            {

                throw NotImplementedError("method disposeCollection()");


            }

        public:

            void add(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name,
                    Key const &key,
                    Value const &value) override
            {

                throw NotImplementedError("method add()");

            }

            void add(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name,
                    Key const &key,
                    Value &&value) override
            {

                throw NotImplementedError("method add()");

            }

            void update(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name,
                    Key const &key,
                    Value const &value) override
            {

                throw NotImplementedError("method update()");

            }

            void update(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name,
                    Key const &key,
                    Value &&value) override
            {

                throw NotImplementedError("method update()");

            }

            Value &search(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name,
                    Key const &key) override
            {

                throw NotImplementedError("method search()");

            }

            std::vector<typename container::AssociativeContainer<Key, Value>::KeyValuePair> searchBetween(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name,
                    Key  const &lower_bound,
                    Key const &upper_bound,
                    bool lower_bound_inclusive,
                    bool upper_bound_inclusive) override
            {

                throw NotImplementedError("method searchBetween()");

            }

            Value dispose(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name,
                    Key const &key) override
            {

                throw NotImplementedError("method dispose()");

            }

        public:


            void addBatch(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name,
                    const std::vector<typename container::AssociativeContainer<Key, Value>::KeyValuePair>& items) override
            {

                throw NotImplementedError("method addBatch()");

            }

            std::vector<Value> disposeBatch(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name,
                    const std::vector<Key>& items) override
            {

                throw NotImplementedError("method disposeBatch()");

            }

        public:

            size_t serialize(std::ostream& stream) const override {
                throw NotImplementedError("method serialize()");
            }

            size_t deserialize(std::istream& stream) override {
                throw NotImplementedError("method deserialize()");
            }

        public:

            void reset() override {
                throw NotImplementedError("method reset()");
            }


        };


    } // namespace storage

} // namespace babrat
