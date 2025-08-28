#pragma once

#include "schema.hpp"
#include <babrat/storage/common/tree_utils.hpp>
#include "babrat/serialization/serializable.hpp"
#include <babrat/storage/db_enums.hpp>
#include <babrat/storage/common/handlers.hpp>

namespace babrat {

    namespace storage {

        template <typename Key,typename Value, typename Compare>
        class pool final : public Serializable
        {

        public:

            typedef schema<Key,Value,Compare> schema;

        private:

            container::SearchTree<FlyString, schema>* schemas_;

            SearchTreeVariant variant_;

        public:

            size_t serialize(std::ostream& stream) const override {
                size_t total_bytes = 0;

                total_bytes += babrat::write(stream, variant_);

                if (schemas_) {
                    total_bytes += schemas_->serialize(stream);
                } else {
                    throw std::runtime_error("Cannot serialize null schemas tree");
                }

                return total_bytes;
            }

            size_t deserialize(std::istream& stream) override {

                size_t total_bytes = 0;

                SearchTreeVariant variant;
                total_bytes += read(stream, variant);

                if (!schemas_) {
                    schemas_ = createTree<FlyString,schema,StringCompare>(variant_);
                }

                total_bytes += schemas_->deserialize(stream);
                variant_ = variant;

                return total_bytes;
            }

            [[nodiscard]] size_t serializedSize() const noexcept override {
                size_t total_size = 0;

                total_size += sizeof(SearchTreeVariant);

                if (schemas_) {
                    total_size += schemas_->serializedSize();
                }

                return total_size;
            }

        public:

            explicit pool(SearchTreeVariant variant = SearchTreeVariant::B_PLUS) : variant_(variant) {
                schemas_ = createTree<FlyString,schema,StringCompare>(variant);
            }

        private:

            void clear()
            {
                delete schemas_;
                schemas_ = nullptr;
            }

            void copy_from(
                    pool const &other)
            {
                schemas_ = copyTree<FlyString,schema, StringCompare>(
                        other.schemas_,
                        variant_ = other.variant_);
            }

            void move_from(
                    pool &&other)
            {
                schemas_ = moveTree<FlyString,schema,StringCompare>(
                        other.schemas_,
                        variant_ = other.variant_);
            }

        public:

            ~pool() override
            {
                clear();
            }


            pool(pool const &other)
            {
                copy_from(other);
            }

            pool &operator=(
                    pool const &other)
            {
                if (this != &other)
                {
                    clear();
                    copy_from(other);
                }

                return *this;
            }

            pool(pool &&other) noexcept
            {
                move_from(std::move(other));
            }

            pool &operator=(
                    pool &&other) noexcept
            {
                if (this != &other)
                {
                    clear();
                    move_from(std::move(other));
                }

                return *this;
            }


        public:

            void add(
                    const std::string &schema_name,
                    SearchTreeVariant variant)
            {
                handleSchemaOperation(schema_name, [this, &schema_name, &variant]() {
                    schemas_->insert(FlyString(schema_name), schema(variant));
                });

            }


            schema &search(
                    const std::string &schema_name)
            {
                return handleSchemaOperation(schema_name,
                    [this, &schema_name]() -> schema& {
                    return schemas_->search(FlyString(schema_name));
                });
            }

            void dispose(
                    const std::string &schema_name)
            {

                handleSchemaOperation(schema_name, [this, &schema_name]() {
                    schemas_->erase(FlyString(schema_name));
                });

            }

            void reset()
            {
                schemas_->reset();
            }

        };

    }



}


