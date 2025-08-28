#pragma once


#include "b_tree.h"
#include "red_black_tree.h"
#include "persist_storage.h"
#include "db_server_traits.h"


namespace babrat {

    namespace storage {

        class TreeFactory {

        public:

            template <typename Key, typename Value, typename Compare = std::less<Key>>
            static container::SearchTree<Key, Value>* create(SearchTreeVariant variant) {
                switch (variant) {
                    case SearchTreeVariant::b_plus:
                        return new container::BTree<Key, Value, Compare>();
                    case SearchTreeVariant::red_black:
                        return new container::RedBlackTree<Key, Value, Compare>();
                    default:
                        throw std::invalid_argument("Unsupported tree type");
                }
            }

            template <typename Key, typename Value, typename Compare>
            static container::SearchTree<Key, Value>* copyTree(
                    container::SearchTree<Key, Value>* source,
                    SearchTreeVariant variant) {

                switch (variant) {
                    case SearchTreeVariant::b_plus:
                        return new container::BTree<Key, Value, Compare>(
                                *dynamic_cast<container::BTree<Key, Value, Compare>*>(source));
                    case SearchTreeVariant::red_black:
                        return new container::RedBlackTree<Key, Value, Compare>(
                                *dynamic_cast<container::RedBlackTree<Key, Value, Compare>*>(source));
                    default:
                        throw std::invalid_argument("Unsupported tree type");
                }

            }

            template <typename Key, typename Value, typename Compare>
            static container::SearchTree<Key, Value>* moveTree(
                    container::SearchTree<Key, Value>* source,
                    SearchTreeVariant variant) {

                switch (variant) {
                    case SearchTreeVariant::b_plus:
                        return new
                                container::BTree<Key, Value, Compare>(
                                std::move(*dynamic_cast<container::BTree<Key, Value, Compare> *>(source)));
                    case SearchTreeVariant::red_black:
                        return new
                                container::RedBlackTree<Key, Value, Compare>(
                                std::move(*dynamic_cast<container::RedBlackTree<Key, Value, Compare> *>(source)));
                    default:
                        throw std::invalid_argument("Unsupported tree type");
                }

            }

        };

    }

}

