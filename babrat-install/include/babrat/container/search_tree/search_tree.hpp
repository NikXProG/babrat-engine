#pragma once

#include <vector>

#include <babrat/container/associative_container.hpp>
#include "babrat/serialization/serializable.hpp"

namespace babrat {

    namespace container {

        template<typename Key,typename Value>
        class SearchTree : public AssociativeContainer<Key, Value>, public Serializable
        {

        public:

            virtual std::vector<typename AssociativeContainer<Key, Value>::KeyValuePair> searchBetween(
                    Key const &lower_bound,
                    Key const &upper_bound,
                    bool lower_bound_inclusive,
                    bool upper_bound_inclusive) = 0;

            virtual void insertBatch(
                    const std::vector<typename AssociativeContainer<Key, Value>::KeyValuePair>& items) = 0;

            virtual std::vector<Value> disposeBatch(
                    const std::vector<Key>& item_keys) = 0;


        public:

            class IteratorBase {
            public:
                virtual ~IteratorBase() = default;
                virtual IteratorBase& operator++() = 0;
                virtual  typename AssociativeContainer<Key, Value>::KeyValuePair operator*() = 0;
                virtual bool operator!=(const IteratorBase& other) const = 0;
            };

            class ConstIteratorBase {
            public:
                virtual ~ConstIteratorBase() = default;
                virtual ConstIteratorBase& operator++() = 0;
                virtual typename AssociativeContainer<Key, Value>::KeyValuePair operator*() const = 0;
                virtual bool operator!=(const ConstIteratorBase& other) const = 0;
            };

            class Iterator {

                std::unique_ptr<IteratorBase> m_impl;

            public:

                explicit Iterator(std::unique_ptr<IteratorBase> impl) :
                    m_impl(std::move(impl)) {}

            public:

                Iterator& operator++() { m_impl->operator++(); return *this; }

                typename AssociativeContainer<Key, Value>::KeyValuePair operator*() {
                    return m_impl->operator*();
                }

                bool operator!=(const Iterator& other) const {
                    return m_impl->operator!=(*other.m_impl);
                }

            };

            class ConstIterator {

                std::unique_ptr<ConstIteratorBase> m_impl;

            public:

                explicit ConstIterator(std::unique_ptr<ConstIteratorBase> impl) :
                    m_impl(std::move(impl)) {}

            public:

                ConstIterator& operator++() { m_impl->operator++(); return *this; }

                typename AssociativeContainer<Key, Value>::KeyValuePair operator*() const {
                    return m_impl->operator*();
                }

                bool operator!=(const ConstIterator& other) const {
                    return m_impl->operator!=(*other.m_impl);
                }

            };

            virtual Iterator begin() = 0;
            virtual Iterator end() = 0;
            virtual ConstIterator cbegin() const = 0;
            virtual ConstIterator cend() const = 0;

        public:

            virtual void reset() = 0;
            virtual void erase(Key const &key) = 0;

        };

    } // namespace container

} // namespace babrat

