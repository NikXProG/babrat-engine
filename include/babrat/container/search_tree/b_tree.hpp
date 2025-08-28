#pragma once


#include <babrat/container/search_tree/search_tree.hpp>

#include <absl/container/btree_map.h>

#include <babrat/container/common/exception.hpp>

namespace babrat{

    namespace container {

    /**
      * @brief adapter over `absl::btree_map<Key, Value, Compare>` container.
      * @tparam Key The type of keys stored in the tree. Must be comparable.
      * @tparam Value The type of values associated with each key.
      * @tparam Compare Comparison function object type (default: std::less<Key>).
      *
      * @details This class implements a balanced search tree (B-Tree) that maintains
      * its elements in sorted order according to the Compare function.
      *
      * @see For more common about B-plus trees, see:
      * <a href="https://en.wikipedia.org/wiki/B%2B_tree">B+ tree on Wikipedia</a>
      */
    template<typename Key, typename Value, typename Compare = std::less<Key>>
        class BTree final : public SearchTree<Key,  Value> {

            using base_tree_type = absl::btree_map<Key, Value, Compare>;

            base_tree_type *m_tree;

        public:

            BTree() : m_tree(new base_tree_type()) {}

        public:

            /**
              * @brief Inserts a new key-value pair into the container.
              *
              * @details The method checks if the key already exists in the container.
              * If the key exists, throws NotExistsException. Otherwise, inserts the new pair.
              *
              * @param[in] key The key to insert (const reference).
              * @param[in] value The value to insert (const reference).
              *
              * @throw NotExistsException If the key already exists in the container.
              */
            void insert(const Key &key, const Value &value) override {
                checkKeyNotExists(key);
                m_tree->insert({key, value});
            }

            /**
              * @brief Inserts a new key-value pair into the container with value movement.
              *
              * The method first checks if the key exists, and if not,
              * inserts the pair while efficiently moving the value into the container's storage.
              *
              * @param[in] key The key to insert (const reference).
              * @param[in,out] value The value to insert (rvalue reference).
              *
              * @throw NotExistsException If the key already exists in the container
              */
            void insert(const Key &key, Value &&value) override {
                checkKeyNotExists(key);
                m_tree->insert({key, std::move(value)});
            }

            /**
              * @brief Updates the value associated with an existing key (copy version).
              *
              * @details Replaces the current value associated with the specified key with a new value.
              * The operation preserves the key and only updates the value component.
              *
              * @param[in] key The key whose value should be updated (const reference).
              * @param[in] value The new value to associate with the key (const reference).
              *
              * @throw NotFoundException If the key doesn't exist in the container.
              */
            void update(const Key &key, const Value &value) override {
                auto it = findKey(key);
                it->second = value;
            }

            /**
              * @brief Updates the value associated with an existing key (move version).
              *
              * @details Moves the new value into the container's storage for the specified key,
              * avoiding unnecessary copies for movable types.
              *
              * @param[in] key The key whose value should be updated (const reference).
              * @param[in,out] value The new value to associate with the key (rvalue reference).
              *
              * @throw NotFoundException If the key doesn't exist in the container.
              */
            void update(const Key &key, Value &&value) override {
                auto it = findKey(key);
                it->second = std::move(value);
            }

            /**
              * @brief Searches for and returns a reference to the value associated with a key.
              *
              * @param[in] key The key to search for (const reference).
              * @return Value& Reference to the value associated with the key.
              *
              * @throw NotFoundException If the key doesn't exist in the container.
              */
            Value &search(const Key &key) override {
                return findKey(key)->second;
            }

            /**
              * @brief Searches for all key-value pairs within a specified key range.
              *
              * @param[in] lower_bound The lower bound of the search range.
              * @param[in] upper_bound The upper bound of the search range.
              * @param[in] lower_bound_inclusive Whether the lower bound is inclusive (true) or exclusive (false).
              * @param[in] upper_bound_inclusive Whether the upper bound is inclusive (true) or exclusive (false).
              * @return std::vector<KeyValuePair> Vector containing all matching key-value pairs.
              */
            std::vector<typename AssociativeContainer<Key, Value>::KeyValuePair>
            searchBetween(Key const &lower_bound,
                          Key const &upper_bound,
                          bool lower_bound_inclusive,
                          bool upper_bound_inclusive) override {

                std::vector<typename AssociativeContainer<Key, Value>::KeyValuePair> result;

                auto begin = lower_bound_inclusive ? m_tree->lower_bound(lower_bound) : m_tree->upper_bound(lower_bound);
                auto end = upper_bound_inclusive ? m_tree->upper_bound(upper_bound) : m_tree->lower_bound(upper_bound);

                for (auto it = begin; it != end; ++it) {
                    result.emplace_back(it->first, it->second);
                }

                return result;
            }

            /**
              * @brief Removes and returns the value associated with the specified key.
              *
              * @param[in] key The key to remove.
              * @return Value The removed value.
              *
              * @throw NotFoundException If the key doesn't exist in the container.
              */
            Value remove(const Key &key) override {
                auto it = findKey(key);
                Value v = std::move(it->second);
                m_tree->erase(it);
                return v;
            }

            /**
              * @brief Inserts multiple key-value pairs in batch.
              *
              * @param[in] items Vector of key-value pairs to insert.
              *
              * @throw NotExistsException If any key already exists in the container.
              */
            void insertBatch(
            const std::vector<typename AssociativeContainer<Key, Value>::KeyValuePair> &items) override {

                for (const auto &[key, value] : items) {
                    insert(key, value);
                }

            }

            /**
              * @brief Removes multiple keys and returns their values in batch.
              *
              * @param[in] item_keys Vector of keys to remove.
              * @return std::vector<Value> Vector of removed values in corresponding order.
              *
              * @throw NotFoundException If any key doesn't exist in the container.
              */
            std::vector<Value> disposeBatch(
            const std::vector<Key> &item_keys) override {

                auto vector_values = std::vector<Value>();
                vector_values.reserve(item_keys.size());

                for (const auto& key : item_keys) {
                    vector_values.push_back(remove(key));
                }

                return vector_values;

            }

            /**
              * @brief Removes a key-value pair from the container without returning the value.
              *
              * @param[in] key The key to erase.
              *
              * @throw NotFoundException If the key doesn't exist in the container.
              */
            void erase(const Key &key) override {
                auto it = findKey(key);
                m_tree->erase(it);
            }


            /**
              * @brief Returns the number of elements in the container.
              *
              * @return size_t The number of key-value pairs in the container.
              */
            size_t size() const override {
                return m_tree->size();
            }

            /**
              * @brief Clears all elements from the container.
              */
            void reset() override {
                m_tree->clear();
            }

        public:

            /**
              * @brief Iterator class for BTree implementing mutable iteration.
              *
              * @details Provides forward iteration capability over the BTree's key-value pairs.
              * The iterator maintains the current position in the tree and supports:
              * - Prefix increment (operator++)
              * - Dereferencing to get key-value pairs (operator*)
              * - Inequality comparison (operator!=)
              */
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

            /**
              * @brief Const iterator class for BTree implementing read-only iteration.
              *
              * @details Provides forward iteration capability over the BTree's key-value pairs
              * without allowing modification of the tree contents.
              */
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

            /**
              * @brief Returns an iterator to the first element of the tree.
              * @return Iterator pointing to the first key-value pair.
              */
            typename SearchTree<Key, Value>::Iterator begin() override {
                return typename SearchTree<Key, Value>::Iterator(
                        std::make_unique<BTreeIterator>(m_tree->begin())
                );
            }

            /**
              * @brief Returns an iterator to the element following the last element.
              * @return Iterator pointing to the past-the-end element.
              */
            typename SearchTree<Key, Value>::Iterator end() override {
                return typename SearchTree<Key, Value>::Iterator(
                        std::make_unique<BTreeIterator>(m_tree->end())
                );
            }

            /**
              * @brief Returns a const iterator to the first element of the tree.
              * @return ConstIterator pointing to the first key-value pair.
              */
            typename SearchTree<Key, Value>::ConstIterator cbegin() const override {
                return typename SearchTree<Key, Value>::ConstIterator(
                        std::make_unique<BTreeConstIterator>(m_tree->begin())
                );
            }

            /**
              * @brief Returns a const iterator to the element following the last element.
              * @return ConstIterator pointing to the past-the-end element.
              */
            typename SearchTree<Key, Value>::ConstIterator cend() const override {
                return typename SearchTree<Key, Value>::ConstIterator(
                        std::make_unique<BTreeConstIterator>(m_tree->end())
                );
            }

        private:

            void clear() {
                delete m_tree;
                m_tree = nullptr;
            }

            void copyFrom(
                    BTree const &other) {

                m_tree =
                        new absl::btree_map<Key, Value, Compare>(
                                *dynamic_cast<absl::btree_map<Key, Value, Compare> *>(other.m_tree));

            }

            void moveFrom(
                    BTree &&other) {
                m_tree =
                        new absl::btree_map<Key, Value, Compare>(
                                std::move(*dynamic_cast<absl::btree_map<Key, Value, Compare> *>(other.m_tree)));
            }

        public:

            //===--------------------------------------------------------------------===//
            // Five of rule
            //===--------------------------------------------------------------------===//

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

            // Internal helper method to find a key iterator with error checking.
            auto findKey(const Key& key) {
                auto it = m_tree->find(key);
                if (it == m_tree->end()) {
                    throw NotFoundException();
                }
                return it;
            }

            // Checks if a key does not exist in the container.
            void checkKeyNotExists(const Key& key) {
                if (m_tree->contains(key)) {
                    throw NotExistsException();
                }
            }

        public:


            /**
              * @brief Serializes the container's contents to an output stream.
              *
              * @details Writes all key-value pairs to the specified output stream in a binary format.
              * The serialization format consists of:
              * 1. The total number of elements (size_t)
              * 2. Each key-value pair sequentially (using babrat::write)
              *
              * @param[in,out] stream The output stream to write to. The stream must be writable
              *                       and in a good state before calling this method.
              *
              * @return size_t The total number of bytes written to the stream.
              */
            size_t serialize(std::ostream &stream) const override {

                const auto start_pos = stream.tellp();

                const size_t size = m_tree->size();

                babrat::write(stream, size);

                for (const auto &[key, value]: *m_tree) {
                    babrat::write(stream, key);
                    babrat::write(stream, value);
                }

                return static_cast<size_t>(stream.tellp() - start_pos);

            }

            /**
              * @brief Deserializes the container's contents from an input stream.
              *
              * @details Reads key-value pairs from the specified input stream in binary format,
              * reconstructing the container state. The expected format must match the serialization format:
              * 1. The total number of elements (size_t)
              * 2. Each key-value pair sequentially (using babrat::read)
              *
              * @param[in,out] stream The input stream to read from. The stream must be readable
              *                       and in a good state before calling this method.
              *
              * @return size_t The total number of bytes read from the stream.
              */
            size_t deserialize(std::istream &stream) override {
                const auto start_pos = stream.tellg();

                m_tree->clear();

                size_t size = 0;
                babrat::read(stream, size);

                for (size_t i = 0; i < size; ++i) {
                    Key key;
                    Value value;

                    babrat::read(stream, key);
                    babrat::read(stream, value);

                    m_tree->insert({std::move(key), std::move(value)});
                }


                return static_cast<size_t>(stream.tellg() - start_pos);

            }

            /**
              * @brief Calculates the serialized size of the container's contents in bytes.
              *
              * @details Computes the total size required to serialize the container, including:
              * 1. The size header (sizeof(size_t))
              * 2. Each serialized key-value pair (using serializedSizeFor)
              *
              * @return size_t The total number of bytes needed for serialization.
              */
            size_t serializedSize() const noexcept override {

                size_t total_size = sizeof(size_t);

                for (const auto &[key, value]: *m_tree) {
                    total_size += serializedSizeFor(key);
                    total_size += serializedSizeFor(value);
                }

                return total_size;
            }

        };

    } // namespace container

} // namespace babrat
