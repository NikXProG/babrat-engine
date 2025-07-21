#include "gtest/gtest.h"
#include "red_black_tree.h"
#include "b_tree.h"


template <typename TreeType>
class SearchTreeTest : public ::testing::Test {

protected:

    using TreePtr = std::unique_ptr<babrat::container::SearchTree<int, int>>;

    void SetUp() override {
        tree_ = std::make_unique<TreeType>();
    }

    TreePtr tree_;

    void fill_tree(size_t count = 1000) {
        for (int i = 0; i < count; ++i) {
            tree_->insert(i, i * 2);
        }
    }

    void fill_sample_data() {
        tree_->insert(1, 300);
        tree_->insert(3, 4);
        tree_->insert(5, 500);
        tree_->insert(7, 8);
        tree_->insert(9, 100);
    }


};


using KeyValuePair = typename babrat::container::AssociativeContainer<int, int>::KeyValuePair;


using TreeTypes = ::testing::Types<
        babrat::container::RedBlackTree<int, int>,
        babrat::container::BTree<int, int>
>;


TYPED_TEST_SUITE(SearchTreeTest, TreeTypes);


TYPED_TEST(SearchTreeTest, testMoveSemantics) {

    TypeParam sourceTree;
    for (int i = 0; i < 10; ++i) {
        sourceTree.insert(i, i * 10);
    }

    TypeParam movedTree1 = std::move(sourceTree);

    EXPECT_EQ(10, movedTree1.size());

}


TYPED_TEST(SearchTreeTest, InsertAndSearchWorks) {

    this->fill_tree();

    EXPECT_EQ(1000, this->tree_->size());
    EXPECT_EQ(500 * 2, this->tree_->search(500));
    EXPECT_EQ(0 * 2, this->tree_->search(0));
    EXPECT_EQ(999 * 2, this->tree_->search(999));
    EXPECT_THROW(this->tree_->search(1001), babrat::BabratContainerError);
}


TYPED_TEST(SearchTreeTest, RangeSearchReturnsCorrectElements) {
    this->fill_sample_data();

    auto result = this->tree_->searchBetween(3, 7, true, true);

    ASSERT_EQ(3, result.size());
    EXPECT_EQ((KeyValuePair{3, 4}), result[0]);
    EXPECT_EQ((KeyValuePair{5, 500}), result[1]);
    EXPECT_EQ((KeyValuePair{7, 8}), result[2]);
}


TYPED_TEST(SearchTreeTest, UpdateModifiesExistingValues) {
    this->tree_->insert(1, 100);
    this->tree_->insert(2, 200);

    this->tree_->update(1, 111);
    this->tree_->update(2, 222);

    EXPECT_EQ(111, this->tree_->search(1));
    EXPECT_EQ(222, this->tree_->search(2));
    EXPECT_THROW(this->tree_->update(3, 300),   babrat::BabratContainerError);
}


template <typename T>
class EmptyTreeTest : public SearchTreeTest<T> {};
TYPED_TEST_SUITE(EmptyTreeTest, TreeTypes);

TYPED_TEST(EmptyTreeTest, OperationsOnEmptyTreeThrowExceptions) {

    EXPECT_EQ(0, this->tree_->size());
    EXPECT_THROW(this->tree_->search(1), babrat::BabratContainerError);
    EXPECT_THROW(this->tree_->remove(1),  babrat::BabratContainerError);

}

TYPED_TEST(SearchTreeTest, IteratorsReturnElementsInSortedOrder) {

    std::vector<std::pair<int, int>> test_data = {
            {3, 30}, {1, 10}, {4, 40}, {2, 20}, {5, 50}
    };

    for (const auto& [k, v] : test_data) {
        this->tree_->insert(k, v);
    }

    auto it = this->tree_->begin();
    for (int i = 1; i <= 5; ++i) {
        EXPECT_EQ(i, (*it).key);
        EXPECT_EQ(i * 10, (*it).value);
        ++it;
    }

}