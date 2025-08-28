#pragma once

#ifndef BABRAT_SRC_CORE_CONTAINER_SEARCH_TREE_INDEX_RED_BLACK_H
#define BABRAT_SRC_CORE_CONTAINER_SEARCH_TREE_INDEX_RED_BLACK_H

#include <map>

#include "search_tree.h"

#include "babrat_container_error.h"

namespace babrat {

    namespace container {

        template<typename Key,
                typename Value,
                typename Compare = std::less<Key>>
        class RedBlackTree : public SearchTree<Key,  Value> {

            using base_tree_type = std::map<Key, Value, Compare>;

            std::map<Key, Value, Compare> * tree_;

        public:

            RedBlackTree() : tree_(new std::map<Key, Value, Compare>()){}

        public:


            void insert(
                    Key const &key,
                    Value const &value) override{
                checkKeyNotExists(key);
                tree_->insert({key, value});
            }


            void insert(
                    Key const &key,
                    Value &&value) override{
                checkKeyNotExists(key);
                tree_->insert({key, std::move(value)});
            }

            void update(Key const &key, Value const &value) override {
                auto it = findKey(key);
                it->second = value;
            }

            void update(Key const &key, Value &&value) override {
                auto it = findKey(key);
                it->second = std::move(value);
            }

            Value &search(Key const &key) override {
                return findKey(key)->second;
            }

            Value remove(Key const &key) override {
                auto it = findKey(key);
                Value v = std::move(it->second);
                tree_->erase(it);
                return v;
            }

            void insertBatch(
                    const std::vector<typename AssociativeContainer<Key, Value>::KeyValuePair>& items) override {

                for (const auto &[key, value] : items) {
                    tree_->insert({key, value});
                }

            }

            std::vector<Value> disposeBatch(
                    const std::vector<Key>& item_keys) override {

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


            size_t size() const override {
                return tree_->size();
            }

            void reset() override
            {
                tree_->clear();
            }

        public:

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

        public:

            class RedBlackTreeIterator : public SearchTree<Key, Value>::IteratorBase {

                typename base_tree_type::iterator it_;

            public:

                explicit RedBlackTreeIterator(typename base_tree_type::iterator it) : it_(it) {}

            public:

                typename SearchTree<Key, Value>::IteratorBase& operator++() override {
                    ++it_;
                    return *this;
                }

                typename AssociativeContainer<Key, Value>::KeyValuePair operator*() override {
                    return {it_->first, it_->second};
                }

                bool operator!=(const SearchTree<Key,Value>::IteratorBase& other) const override {
                    const auto* derived = dynamic_cast<const RedBlackTreeIterator*>(&other);
                    return derived && it_ != derived->it_;
                }

            };

            class RedBlackTreeConstIterator : public SearchTree<Key, Value>::ConstIteratorBase {

                typename base_tree_type::const_iterator it_;

            public:

                explicit RedBlackTreeConstIterator(typename base_tree_type::const_iterator it) : it_(it) {}

            public:

                typename SearchTree<Key, Value>::ConstIteratorBase& operator++() override {
                    ++it_;
                    return *this;
                }

                typename AssociativeContainer<Key, Value>::KeyValuePair operator*() const override {
                    return {it_->first, it_->second};
                }

                bool operator!=(const SearchTree<Key,Value>::ConstIteratorBase& other) const override {
                    const auto* derived = dynamic_cast<const RedBlackTreeConstIterator*>(&other);
                    return derived && it_ != derived->it_;
                }
            };

            typename SearchTree<Key, Value>::Iterator begin() override {
                return typename SearchTree<Key, Value>::Iterator(
                        std::make_unique<RedBlackTreeIterator>(tree_->begin())
                );
            }

            typename SearchTree<Key, Value>::Iterator end() override {
                return typename SearchTree<Key, Value>::Iterator(
                        std::make_unique<RedBlackTreeIterator>(tree_->end())
                );
            }

            typename SearchTree<Key, Value>::ConstIterator cbegin() const override {
                return typename SearchTree<Key, Value>::ConstIterator(
                        std::make_unique<RedBlackTreeConstIterator>(tree_->begin())
                );
            }

            typename SearchTree<Key, Value>::ConstIterator cend() const override {
                return typename SearchTree<Key, Value>::ConstIterator(
                        std::make_unique<RedBlackTreeConstIterator>(tree_->end())
                );
            }


        private:

            void clear() {
                delete tree_;
                tree_ = nullptr;
            }

            void copyFrom(
                    RedBlackTree const &other) {

                tree_ =
                        new std::map<Key, Value, Compare>(
                                *dynamic_cast<std::map<Key, Value, Compare> *>(other.tree_));

            }

            void moveFrom(
                    RedBlackTree &&other) {
                tree_ =
                        new std::map<Key, Value, Compare>(
                                std::move(*dynamic_cast<std::map<Key, Value, Compare> *>(other.tree_)));
            }

        public:

            ~RedBlackTree() override {
                clear();
            }

            RedBlackTree(RedBlackTree const &other) {
                copyFrom(other);
            }

            RedBlackTree &operator=(
                    RedBlackTree const &other) {

                if (this != &other) {
                    clear();
                    copyFrom(other);
                }

                return *this;

            }

            RedBlackTree(RedBlackTree &&other) noexcept {
                moveFrom(std::move(other));
            }

            RedBlackTree &operator=(
                    RedBlackTree &&other) noexcept {

                if (this != &other) {
                    clear();
                    moveFrom(std::move(other));
                }

                return *this;
            }

        public:

            size_t serialize(std::ostream& stream) const override {

                const auto start_pos = stream.tellp();

                const size_t size = tree_->size();

                babrat::write(stream, size);

                for (const auto& [key, value] : *tree_) {
                    babrat::write(stream, key);
                    babrat::write(stream, value);
                }

                return static_cast<size_t>(stream.tellp() - start_pos);

            }

            size_t deserialize(std::istream& stream) override {

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

                for (const auto& [key, value] : *tree_) {
                    total_size += serializedSizeFor(key);
                    total_size += serializedSizeFor(value);
                }


                return total_size;
            }


        private:

            auto findKey(const Key& key) {
                auto it = tree_->find(key);
                if (it == tree_->end()) {
                    throw BabratContainerError("Key not found");
                }
                return it;
            }

            void checkKeyNotExists(const Key& key) {
                if (tree_->contains(key)) {
                    throw BabratContainerError("Key already exists");
                }
            }

        };

    }


}




#endif //BABRAT_SRC_CORE_CONTAINER_SEARCH_TREE_INDEX_RED_BLACK_H
