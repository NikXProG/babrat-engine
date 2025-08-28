#pragma once

#include <babrat/common/exception.hpp>
#include <babrat/storage/common/db_utils.hpp>
#include <babrat/container/search_tree/search_tree.hpp>
#include <babrat/storage/interfaces/storage.hpp>

namespace babrat {

    namespace storage {


        template<typename Key,
                typename Value,
                typename Compare = std::less<Key>>
        class collection final : public Serializable {

            container::SearchTree<Key, Value>* data_{};

            SearchTreeVariant variant_{};

        public:

            using iterator = typename container::SearchTree<Key, Value>::Iterator;
            using const_iterator = typename container::SearchTree<Key, Value>::ConstIterator;

        public:

            typedef Compare key_compare;

        public:

            iterator begin() {
                return data_->begin();
            }

            iterator end() {
                return data_->end();
            }

            const_iterator cbegin() const {
                return data_->cbegin();
            }

            const_iterator cend() const {
                return data_->cend();
            }

        public:

            size_t serialize(std::ostream& stream) const override {

                size_t total_bytes = 0;

                total_bytes += babrat::write(stream, variant_);

                if (data_) {
                    total_bytes += data_->serialize(stream);
                } else {
                    throw std::runtime_error("Cannot serialize null tree");
                }

                return total_bytes;

            }

            size_t deserialize(std::istream& stream) override {

                size_t total_bytes = 0;

                SearchTreeVariant variant;
                total_bytes += babrat::read(stream, variant);

                if (data_) {
                    clear();
                }

                data_ = createTree<Key, Value, key_compare>(variant);

                variant_ = variant;
                total_bytes += data_->deserialize(stream);

                return total_bytes;
            }

            [[nodiscard]] size_t serializedSize() const noexcept override {

                size_t total_size = sizeof(SearchTreeVariant);

                if (data_) {
                    total_size += data_->serializedSize();
                }

                return total_size;

            }

        public:

            explicit collection(
                    SearchTreeVariant variant = SearchTreeVariant::b_plus):
                    variant_(variant)
            {
                data_ = createTree<Key, Value, key_compare>(variant);
            }

        private:

            void clear()
            {
                delete data_;
                data_ = nullptr;
            }


            void copy_from(
                    collection const &other)
            {
                data_ = copyTree<Key, Value, key_compare>(
                        other.data_,
                        variant_ = other.variant_);
            }

            void move_from(
                    collection &&other)
            {
                data_ = moveTree<Key, Value, key_compare>(
                        other.data_,
                        variant_ = other.variant_);
            }

        public:

            ~collection() override
            {
                clear();
            }

            collection(
                    collection const &other)
            {
                copy_from(other);
            }

            collection &operator=(
                    collection const &other)
            {
                if (this != &other)
                {
                    clear();
                    copy_from(other);
                }

                return *this;
            }

            collection(
                    collection &&other) noexcept
            {
                move_from(std::move(other));
            }

            collection &operator=(
                    collection &&other) noexcept
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
                    Key const &key,
                    Value const &value)
            {

                data_->insert(key, value);
              /*  handleKeyCollectionOperation([this,&key, &value]() {
                    data_->insert(key, value);
                });
*/
            }

            void add(
                    Key const &key,
                    Value &&value)
            {
                data_->insert(key, std::move(value));

/*
                handleKeyCollectionOperation([this,&key, &value]() {
                    data_->insert(key, std::move(value));
                });*/

            }

            void addBatch(
                    const std::vector<typename container::AssociativeContainer<Key, Value>::KeyValuePair>& items)
            {
                handleKeyCollectionOperation([this,&items]() {
                    data_->insertBatch(items);
                });
            }


            Value& search(
                    Key const &key)
            {

                return handleKeyCollectionOperation([this, &key]() -> Value& {
                    return data_->search(key);
                });

            }

            void update(
                    Key const &key,
                    Value const &value)
            {
                handleKeyCollectionOperation([this, &key, &value]() {
                    data_->update(key, value);
                });

            }

            void update(
                    Key const &key,
                    Value &&value)
            {
                handleKeyCollectionOperation([this, &key, &value]() {
                    data_->update(key, std::move(value));
                });

            }

            Value dispose(
                    Key const &key)
            {
                return handleKeyCollectionOperation([this, &key]() {
                    return data_->remove(key);
                });

            }

            std::vector<Value> disposeBatch(
                    const std::vector<Key>& item_keys)
            {
                return handleKeyCollectionOperation([this, &item_keys]() {
                    return data_->disposeBatch(item_keys);
                });

            }


            void reset()
            {
                data_->reset();
            }


            std::vector<typename container::AssociativeContainer<Key, Value>::KeyValuePair>
            searchBetween(Key const &lower_bound,
                          Key const &upper_bound,
                          bool lower_bound_inclusive,
                          bool upper_bound_inclusive) {
                return data_->searchBetween(
                        lower_bound,
                        upper_bound,
                        lower_bound_inclusive,
                        upper_bound_inclusive);
            }

        private:




        };

    }
}

