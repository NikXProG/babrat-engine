#pragma once

#include "storage.h"

#include <boost/flyweight.hpp>

using FlyString = boost::flyweight<std::string>;
using StringCompare = std::less<std::string>;

#include "b_tree.h"
#include "red_black_tree.h"
#include "tree_factory.h"

#include "pool.h"
#include "schema.h"
#include "collection.h"


namespace babrat {

    namespace storage {

        template<
                typename Key,
                typename Value,
                typename Compare>
        class InMemoryStorage final : public IStorage<Key,Value> {

        public:

            typedef pool<Key, Value, Compare> pool;

        private:

            container::SearchTree<FlyString, pool>* m_pools;

        private:

            SearchTreeVariant m_treeVariant;

        public:

            explicit InMemoryStorage(SearchTreeVariant treeVariant) : m_treeVariant(treeVariant){
                m_pools = TreeFactory::create<FlyString, pool, StringCompare>(treeVariant);
            }

        public:

            ~InMemoryStorage() noexcept override = default;

        public:


            void addPool(
                    const std::string &pool_name) override
            {
                add(pool_name, m_treeVariant);
            }

            void addSchema(
                    const std::string &pool_name,
                    const std::string &schema_name) override
            {

                search(pool_name)
                        .add(schema_name, m_treeVariant);

            }

            void addCollection(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name) override
            {

                search(pool_name)
                        .search(schema_name)
                        .add(collection_name, m_treeVariant);

            }

        public:

            void clearPool(
                    const std::string &pool_name) override
            {
                search(pool_name).
                        reset();
            }

            void clearSchema(
                    const std::string &pool_name,
                    const std::string &schema_name) override
            {

                search(pool_name)
                        .search(schema_name)
                        .reset();

            }

            void clearCollection(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name) override {


                search(pool_name)
                        .search(schema_name)
                        .search(collection_name)
                        .reset();


            }

        public:

            void disposePool(
                    const std::string &pool_name) override
            {
                dispose(pool_name);
            }

            void disposeSchema(
                    const std::string &pool_name,
                    const std::string &schema_name) override
            {
                search(pool_name)
                        .dispose(schema_name);
            }

            void disposeCollection(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name) override
            {

                search(pool_name)
                        .search(schema_name)
                        .dispose(collection_name);


            }

        public:

            void add(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name,
                    Key const &key,
                    Value const &value) override
            {
                search(pool_name)
                        .search(schema_name)
                        .search(collection_name)
                        .add(key, value);
            }

            void add(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name,
                    Key const &key,
                    Value &&value) override
            {
                search(pool_name)
                        .search(schema_name)
                        .search(collection_name)
                        .add(key, std::move(value));

            }

            void update(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name,
                    Key const &key,
                    Value const &value) override
            {
                search(pool_name)
                        .search(schema_name)
                        .search(collection_name)
                        .update(key, value);

            }

            void update(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name,
                    Key const &key,
                    Value &&value) override
            {

                search(pool_name)
                        .search(schema_name)
                        .search(collection_name)
                        .update(key, std::move(value));
            }

            Value &search(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name,
                    Key const &key) override
            {

                return search(pool_name)
                        .search(schema_name)
                        .search(collection_name)
                        .search(key);

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
                return search(pool_name)
                        .search(schema_name)
                        .search(collection_name)
                        .searchBetween(
                                lower_bound,
                                upper_bound,
                                lower_bound_inclusive,
                                upper_bound_inclusive);

            }

            Value dispose(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name,
                    Key const &key) override
            {
                return search(pool_name)
                        .search(schema_name)
                        .search(collection_name)
                        .dispose(key);

            }

        public:


            void addBatch(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name,
                    const std::vector<typename container::AssociativeContainer<Key, Value>::KeyValuePair>& items) override
            {

                search(pool_name)
                        .search(schema_name)
                        .search(collection_name)
                        .addBatch(items);

            }

            std::vector<Value> disposeBatch(
                    const std::string &pool_name,
                    const std::string &schema_name,
                    const std::string &collection_name,
                    const std::vector<Key>& items) override
            {

                return  search(pool_name)
                        .search(schema_name)
                        .search(collection_name)
                        .disposeBatch(items);

            }

        public:

            size_t serialize(std::ostream& stream) const override {

                std::cout << "serialize";
                if (!stream.good()) {
                    throw std::runtime_error("Stream is not in good state");
                }

                size_t total_bytes = 0;

                size_t pool_count = m_pools->size();

                total_bytes += babrat::write(stream,pool_count);

                for (const auto& [pool_name, pool] : *m_pools) {
                    total_bytes += babrat::write(stream, pool_name);
                    total_bytes += pool.serialize(stream);
                }

                return total_bytes;
            }

            size_t deserialize(std::istream& stream) override {

                std::cout << "deserialize";

                if (!stream.good()) {
                    throw std::runtime_error("Stream is not in good state");
                }

                size_t total_bytes = 0;

                size_t pool_count;
                total_bytes += babrat::read(stream, pool_count);

                reset();

                for (size_t i = 0; i < pool_count; ++i) {

                    FlyString pool_name;
                    total_bytes += babrat::read(stream, pool_name);

                    pool pool(m_treeVariant);
                    total_bytes += babrat::read(stream, pool);

                    m_pools->insert(std::move(pool_name), std::move(pool));
                }

                return total_bytes;
            }

            size_t serializedSize() const noexcept override {
                size_t total_size = 0;

                for (const auto& [pool_name, pool] : *m_pools) {
                    total_size += sizeof(size_t) + pool_name.get().size();
                    total_size += pool.serializedSize();
                }

                return total_size;
            }


        public:

            void reset() override {
                delete m_pools;
                m_pools = TreeFactory::create<FlyString, pool, StringCompare>(m_treeVariant);
            }

        private:

            void add(
                    const std::string &pool_name,
                    SearchTreeVariant variant)
            {
                handlePoolOperation(pool_name, [this, &pool_name, &variant]() {
                    m_pools->insert(FlyString(pool_name), pool(variant));
                });
            }

            pool &search(const std::string &pool_name) {
                return handlePoolOperation(pool_name, [this, &pool_name]()
                        -> pool& {
                    return m_pools->search(FlyString(pool_name));
                });
            }

            void dispose(
                    const std::string &pool_name)
            {
                handlePoolOperation(pool_name, [this, &pool_name]() {
                    m_pools->erase(FlyString(pool_name));
                });
            }

        };


    }
}

