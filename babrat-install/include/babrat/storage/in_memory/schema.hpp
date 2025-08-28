#pragma once

#include "collection.hpp"
#include <babrat/storage/common/db_utils.hpp>

namespace babrat {

    namespace storage {


        template<typename Key,
                 typename Value,
                 typename Compare = std::less<Key>>
        class schema final : public Serializable {

        public:

            typedef collection<Key,Value, Compare> collection;

        private:

            SearchTreeVariant variant_;

            container::SearchTree<FlyString, collection>* collections_;

        private:

            void clear()
            {
                delete collections_;
                collections_ = nullptr;
            }

            void copy_from(
                    schema const &other)
            {
                collections_ = copyTree<FlyString, collection, StringCompare>(
                                other.collections_,
                                variant_ = other.variant_);
            }

            void move_from(
                    schema &&other)
            {
                collections_ = moveTree<FlyString, collection, StringCompare>(
                                other.collections_,
                                variant_ = other.variant_);

            }

        public:

            size_t serialize(std::ostream& stream) const override {
                size_t total_bytes = 0;

                total_bytes += babrat::write(stream, variant_);

                if (collections_) {
                    total_bytes += collections_->serialize(stream);
                } else {
                    throw std::runtime_error("Cannot serialize null collections tree");
                }

                return total_bytes;
            }

            size_t deserialize(std::istream& stream) override {

                size_t total_bytes = 0;

                SearchTreeVariant variant;
                total_bytes += babrat::read(stream, variant);

                if (!collections_) {
                    collections_ = createTree<FlyString,collection,StringCompare>(variant);
                }

                total_bytes += collections_->deserialize(stream);
                variant_ = variant;

                return total_bytes;
            }

            [[nodiscard]] size_t serializedSize() const noexcept override {
                size_t total_size = 0;

                total_size += sizeof(SearchTreeVariant);

                if (collections_) {
                    total_size += collections_->serializedSize();
                }

                return total_size;
            }

        public:

            explicit schema(
                    SearchTreeVariant variant = SearchTreeVariant::b_plus):
                    variant_(variant)
            {
                collections_ = createTree<FlyString,collection, StringCompare>(variant);
            }

        public:


            void add(
                    const std::string &collection_name,
                    SearchTreeVariant variant)
            {

                handleCollectionOperation(collection_name,
                            [this, &collection_name, &variant]() {
                                collections_->insert(
                                        FlyString(collection_name),
                                        collection(variant));
                            });

            }

            collection &search(
                    const std::string &collection_name)
            {
                return handleCollectionOperation(collection_name,
                            [this, &collection_name]() -> collection& {
                                return collections_->search(FlyString(collection_name));
                            });

            }


            void dispose(
                    const std::string &collection_name)
            {
                handleCollectionOperation(collection_name,
                            [this, &collection_name]() {
                                collections_->erase(FlyString(collection_name));
                            });

            }

            void reset()
            {
                collections_->reset();
            }


        public:

            ~schema() override
            {
                clear();
            }

            schema(
                    schema const &other)
            {
                copy_from(other);
            }

            schema &operator=(
                    schema const &other)
            {
                if (this != &other)
                {
                    clear();
                    copy_from(other);
                }

                return *this;
            }

            schema(
                    schema &&other) noexcept
            {
                move_from(std::move(other));
            }

            schema &operator=(
                    schema &&other) noexcept
            {
                if (this != &other)
                {
                    clear();
                    move_from(std::move(other));
                }

                return *this;
            }


        };


    }

}
