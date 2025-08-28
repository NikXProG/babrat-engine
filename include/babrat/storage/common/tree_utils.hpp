#pragma once

#include <babrat/storage/db_enums.hpp>
#include <babrat/container/search_tree/b_tree.hpp>
#include <babrat/container/search_tree/red_black_tree.hpp>

#include <babrat/common/exception.hpp>

namespace babrat {

    namespace storage {

        template <typename Key, typename Value, typename Compare = std::less<Key>>
        static container::SearchTree<Key, Value>* createTree(SearchTreeVariant variant) {
            switch (variant) {
                case SearchTreeVariant::B_PLUS:
                    return new container::BTree<Key, Value, Compare>();
                case SearchTreeVariant::RED_BLACK:
                    return new container::RedBlackTree<Key, Value, Compare>();
            }
            //
            // throw UnknownTypeException(variant,
            //     "Unsupported tree type for `createTree()` method");

        }

        template <typename Key, typename Value, typename Compare>
        static container::SearchTree<Key, Value>* copyTree(
                container::SearchTree<Key, Value>* source,
                SearchTreeVariant variant) {

            switch (variant) {
                case SearchTreeVariant::B_PLUS:
                    return new container::BTree<Key, Value, Compare>(
                            *dynamic_cast<container::BTree<Key, Value, Compare>*>(source));
                case SearchTreeVariant::RED_BLACK:
                    return new container::RedBlackTree<Key, Value, Compare>(
                            *dynamic_cast<container::RedBlackTree<Key, Value, Compare>*>(source));
            }

            /*throw UnknownTypeException(variant,
                "Unsupported tree type for `copyTree()` method");*/

        }

        template <typename Key, typename Value, typename Compare>
        static container::SearchTree<Key, Value>* moveTree(
                container::SearchTree<Key, Value>* source,
                SearchTreeVariant variant) {

            switch (variant) {
                case SearchTreeVariant::B_PLUS:
                    return new
                            container::BTree<Key, Value, Compare>(
                            std::move(*dynamic_cast<container::BTree<Key, Value, Compare> *>(source)));
                case SearchTreeVariant::RED_BLACK:
                    return new
                            container::RedBlackTree<Key, Value, Compare>(
                            std::move(*dynamic_cast<container::RedBlackTree<Key, Value, Compare> *>(source)));
            }

            /*throw UnknownTypeException(variant,
                "Unsupported tree type for `moveTree()` method");*/
        }

    } // namespace storage

} // namespace storage