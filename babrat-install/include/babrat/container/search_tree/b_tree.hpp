#pragma once


#include <babrat/container/search_tree/search_tree.hpp>

#include <absl/container/btree_map.h>

#include <babrat/container/common/exception.hpp>

namespace babrat{

    namespace container {

        /*
        template <int InnerSlots, int LeafSlots>
        struct BTreeTraits : tlx::btree_default_traits<size_t, size_t>
        {

            static const bool self_verify = false;
            static const bool debug = false;

            static const int leaf_slots = LeafSlots;
            static const int inner_slots = InnerSlots;

            static const size_t binsearch_threshold = 256 * 1024 * 1024;

        };
        */

        template<typename Key,
                typename Value,
                typename Compare = std::less<Key>>
        class BTree final : public SearchTree<Key,  Value> {

            using base_tree_type = absl::btree_map<Key, Value, Compare>;

            base_tree_type *tree_;

        public:

            BTree() : tree_(new base_tree_type()) {}

        public:

            void insert(const Key &key, const Value &value) override {
                if (tree_->contains(key)) {
                    throw NotExistsException();
                }
                tree_->insert({key, value});
            }

            void insert(const Key &key, Value &&value) override {
                checkKeyNotExists(key);
                tree_->insert({key, std::move(value)});
            }

            void update(const Key &key, const Value &value) override {
                auto it = findKey(key);
                it->second = value;
            }

            void update(const Key &key, Value &&value) override {
                auto it = findKey(key);
                it->second = std::move(value);
            }

            Value &search(const Key &key) override {
                return findKey(key)->second;
            }

            std::vector<typename AssociativeContainer<Key, Value>::KeyValuePair>
            searchBetween(Key const &lower_bound,
                          Key const &upper_bound,
                          bool lower_bound_inclusive,
                          bool upper_bound_inclusive) override {

                std::vector<typename AssociativeContainer<Key, Value>::KeyValuePair> result;

                auto begin = lower_bound_inclusive ? tree_->lower_bound(lower_bound) : tree_->upper_bound(lower_bound);
                auto end = upper_bound_inclusive ? tree_->upper_bound(upper_bound) : tree_->lower_bound(upper_bound);

                for (auto it = begin; it != end; ++it) {
                    result.emplace_back(it->first, it->second);
                }

                return result;
            }


            Value remove(const Key &key) override {
                auto it = findKey(key);
                Value v = std::move(it->second);
                tree_->erase(it);
                return v;
            }

            void insertBatch(
                    const std::vector<typename AssociativeContainer<Key, Value>::KeyValuePair> &items) override {


                for (const auto &[key, value] : items) {
                    insert(key, value);
                }

            }

            std::vector<Value> disposeBatch(
                    const std::vector<Key> &item_keys) override {

                auto vector_values = std::vector<Value>();
                vector_values.reserve(item_keys.size());

                for (const auto& key : item_keys) {
                    vector_values.push_back(remove(key));
                }

                return vector_values;

            }

            void erase(const Key &key) override {
                auto it = findKey(key);
                tree_->erase(it);
            }


            [[nodiscard]] size_t size() const override {
                return tree_->size();
            }

            void reset() override {
                tree_->clear();
            }



        public:

            class BTreeIterator : public SearchTree<Key, Value>::IteratorBase {

                typename base_tree_type::iterator it_;

            public:

                explicit BTreeIterator(typename base_tree_type::iterator it) : it_(it) {}

            public:

                typename SearchTree<Key, Value>::IteratorBase &operator++() override {
                    ++it_;
                    return *this;
                }

                typename AssociativeContainer<Key, Value>::KeyValuePair operator*() override {
                    return {it_->first, it_->second};
                }

                bool operator!=(const SearchTree<Key, Value>::IteratorBase &other) const override {
                    const auto *derived = dynamic_cast<const BTreeIterator *>(&other);
                    return derived && it_ != derived->it_;
                }
            };

            class BTreeConstIterator : public SearchTree<Key, Value>::ConstIteratorBase {

                typename base_tree_type::const_iterator it_;

            public:

                explicit BTreeConstIterator(
                        typename base_tree_type::const_iterator it) : it_(it) {}

            public:

                typename SearchTree<Key, Value>::ConstIteratorBase &operator++() override {
                    ++it_;
                    return *this;
                }

                typename AssociativeContainer<Key, Value>::KeyValuePair operator*() const override {
                    return {it_->first, it_->second};
                }

                bool operator!=(const typename SearchTree<Key, Value>::ConstIteratorBase &other) const override {
                    const auto *derived = dynamic_cast<const BTreeConstIterator *>(&other);
                    return derived && it_ != derived->it_;
                }
            };


            typename SearchTree<Key, Value>::Iterator begin() override {
                return typename SearchTree<Key, Value>::Iterator(
                        std::make_unique<BTreeIterator>(tree_->begin())
                );
            }

            typename SearchTree<Key, Value>::Iterator end() override {
                return typename SearchTree<Key, Value>::Iterator(
                        std::make_unique<BTreeIterator>(tree_->end())
                );
            }

            typename SearchTree<Key, Value>::ConstIterator cbegin() const override {
                return typename SearchTree<Key, Value>::ConstIterator(
                        std::make_unique<BTreeConstIterator>(tree_->begin())
                );
            }

            typename SearchTree<Key, Value>::ConstIterator cend() const override {
                return typename SearchTree<Key, Value>::ConstIterator(
                        std::make_unique<BTreeConstIterator>(tree_->end())
                );
            }

        private:

            void clear() {
                delete tree_;
                tree_ = nullptr;
            }

            void copyFrom(
                    BTree const &other) {

                tree_ =
                        new absl::btree_map<Key, Value, Compare>(
                                *dynamic_cast<absl::btree_map<Key, Value, Compare> *>(other.tree_));

            }

            void moveFrom(
                    BTree &&other) {
                tree_ =
                        new absl::btree_map<Key, Value, Compare>(
                                std::move(*dynamic_cast<absl::btree_map<Key, Value, Compare> *>(other.tree_)));
            }

        public:

            ~BTree() override {
                clear();
            }


            BTree(BTree const &other) {
                copyFrom(other);
            }

            BTree &operator=(
                    BTree const &other) {

                if (this != &other) {
                    clear();
                    copyFrom(other);
                }

                return *this;

            }

            BTree(BTree &&other) noexcept {
                moveFrom(std::move(other));
            }

            BTree &operator=(
                    BTree &&other) noexcept {

                if (this != &other) {
                    clear();
                    moveFrom(std::move(other));
                }

                return *this;
            }

        private:

            auto findKey(const Key& key) {
                auto it = tree_->find(key);
                if (it == tree_->end()) {
                    throw NotFoundException();
                }
                return it;
            }

            void checkKeyNotExists(const Key& key) {
                if (tree_->contains(key)) {
                    throw NotExistsException();
                }
            }


        public:

            size_t serialize(std::ostream &stream) const override {

                const auto start_pos = stream.tellp();

                const size_t size = tree_->size();

                babrat::write(stream, size);

                for (const auto &[key, value]: *tree_) {
                    babrat::write(stream, key);
                    babrat::write(stream, value);
                }

                return static_cast<size_t>(stream.tellp() - start_pos);

            }

            size_t deserialize(std::istream &stream) override {
                const auto start_pos = stream.tellg();

                tree_->clear();

                size_t size = 0;
                babrat::read(stream, size);

                for (size_t i = 0; i < size; ++i) {
                    Key key;
                    Value value;

                    babrat::read(stream, key);
                    babrat::read(stream, value);

                    tree_->insert({std::move(key), std::move(value)});
                }


                return static_cast<size_t>(stream.tellg() - start_pos);

            }

            [[nodiscard]] size_t serializedSize() const noexcept override {

                size_t total_size = sizeof(size_t);

                for (const auto &[key, value]: *tree_) {
                    total_size += serializedSizeFor(key);
                    total_size += serializedSizeFor(value);
                }


                return total_size;
            }

        };

        }

}
