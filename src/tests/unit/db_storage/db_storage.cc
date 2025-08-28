#include "gtest/gtest.h"
#include "database_storage.h"
#include "convert_column.h"
#include <random>


std::uint64_t get_current_timestamp(){
    return babrat::convert::timestamp_to_uint64(std::chrono::system_clock::now());
}


class DBServerTest : public ::testing::Test {

protected:

    void SetUp() override {
        db_ = new babrat::storage::DatabaseStorage();
    }

    void TearDown() override {
        delete db_;
    }

    babrat::storage::Storage<TypeColumn, std::vector<TypeColumn>>* db_{};

};

TEST_F(DBServerTest, addMetadata){

    db_->addPool("sdsdsdssssdsd",
                 babrat::storage::search_tree_variant::b_plus);

    db_->addSchema("sdsdsdssssdsd",
                   "sdsds",
                   babrat::storage::search_tree_variant::b_plus);

    db_->addCollection("sdsdsdssssdsd",
                       "sdsds",
                       "sdsds",
                       babrat::storage::search_tree_variant::b_plus);

    db_->add("sdsdsdssssdsd",
             "sdsds",
             "sdsds",
             babrat::convert::to_column(4000),
             {
                     babrat::convert::to_column(22),
                     babrat::convert::to_column(1001),
                     babrat::convert::to_column(1001)
             });

    db_->persist(
            get_current_timestamp());

    db_->restore(
            get_current_timestamp(),true);

    auto value = db_->search("sdsdsdssssdsd",
             "sdsds",
             "sdsds",
             babrat::convert::to_column(4000));

    std::cout << babrat::convert::get<int>(value[2]) << std::endl;


}