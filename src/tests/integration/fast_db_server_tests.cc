#include "gtest/gtest.h"
#include "DbStorage.h"
/*#include "config_data_builder.h"
#include "data_caster.h"*/
#include "column_base_key.h"
#include "column_types/int32.h"
#include "column_types/dynamic_string.h"
/*#include "column_interpreter.h"*/
#include <limits.h>

class DBServerTest : public ::testing::Test {
protected:
    void SetUp() override {
        db_ = new  babrat::db_storage<int, int>();
    }

    void TearDown() override {
        delete db_;
    }

    babrat::db_storage<int, int>* db_{};
};

int main(int argc,char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST_F(DBServerTest, failedTestObtainDataDb) {

    constexpr std::string pool_test = "test_pool";
    constexpr std::string schema_test = "test_schema";
    constexpr std::string collection_test = "test_collection";

    // Attempt to retrieve non-existent data

    EXPECT_THROW({
                     db_->obtain(pool_test, schema_test, collection_test, 999);
                 }, babrat::data_not_initialized_error);


    db_->add_pool(pool_test);
    db_->add_schema(pool_test, schema_test);
    db_->add_collection(pool_test, schema_test, collection_test);

    // Trying to get data from an empty collection

    EXPECT_THROW({

                     db_->obtain(pool_test, schema_test, collection_test, 1);
                 }, babrat::data_not_initialized_error);


    // Invalid pool/scheme/collection names
    EXPECT_THROW({
                     db_->obtain("invalid_pool", schema_test, collection_test, 1);
                 }, babrat::data_not_initialized_error);

    EXPECT_THROW({
                     db_->obtain(pool_test, "invalid_schema", collection_test, 1);
                 }, babrat::data_not_initialized_error);

    EXPECT_THROW({
                     db_->obtain(pool_test, schema_test, "invalid_collection", 1);
                 }, babrat::data_not_initialized_error);

}

TEST_F(DBServerTest, failedTestUpdateDataDb) {

    constexpr std::string pool_test = "test_pool";
    constexpr std::string schema_test = "test_schema";
    constexpr std::string collection_test = "test_collection";

    // Attempt to update in non-existent structure
    EXPECT_THROW({
                     db_->update(pool_test, schema_test, collection_test, 1, 100);
                 }, babrat::data_not_initialized_error);

    db_->add_pool(pool_test);
    db_->add_schema(pool_test, schema_test);
    db_->add_collection(pool_test, schema_test, collection_test);

    // Attempt to update data via add method ( not supported )
    db_->add(pool_test, schema_test, collection_test, 3, 100);

    EXPECT_THROW({
                     db_->add(pool_test, schema_test, collection_test, 3, 100);
                 }, babrat::duplicate_key_error);


    // Attempt to update non-existent key
    EXPECT_THROW({
                     db_->update(pool_test, schema_test, collection_test, 1, 100);
                 }, babrat::data_not_initialized_error);


    // Add data and try to update a non-existent key
    db_->add(pool_test, schema_test, collection_test, 1, 100);
    EXPECT_THROW({
                     db_->update(pool_test, schema_test, collection_test, 2, 200);
                 }, babrat::data_not_initialized_error);

}

TEST_F(DBServerTest, testInsertDataDb) {
    constexpr std::string pool_test = "my_db";
    constexpr std::string schema_test = "public";
    constexpr std::string collection_test = "test_collection";

    // Initializing the structure
    db_->add_pool(pool_test);
    db_->add_schema(pool_test, schema_test);
    db_->add_collection(pool_test, schema_test, collection_test);

    // Inserting and checking numeric values
    for (int i = 0; i < 100; ++i) {
        db_->add(pool_test, schema_test, collection_test, i, i*10);
        auto value = db_->obtain(pool_test, schema_test, collection_test, i);
        EXPECT_EQ(value, i*10);
    }

    // Checking edge cases
    db_->add(pool_test, schema_test, collection_test, INT_MAX, 2);
    db_->add(pool_test, schema_test, collection_test, INT_MIN, 2);

    auto range = db_->obtain_between(pool_test, schema_test, collection_test,
                                     50, 60, true, false);
    EXPECT_EQ(range.size(), 10);

}


TEST_F(DBServerTest, testRemoveDataDb) {
    // Тестирование удаления данных
    constexpr std::string pool_test = "test_pool";
    constexpr std::string schema_test = "test_schema";
    constexpr std::string collection_test = "test_collection";

    db_->add_pool(pool_test);
    db_->add_schema(pool_test, schema_test);
    db_->add_collection(pool_test, schema_test, collection_test);

    // Добавляем и сразу удаляем
    db_->add(pool_test, schema_test, collection_test, 1,22 );
    db_->dispose(pool_test, schema_test, collection_test, 1);
    EXPECT_THROW({
                     db_->obtain(pool_test, schema_test, collection_test, 1);
                 }, babrat::db_storage_error);

    // Попытка удалить несуществующий ключ
    EXPECT_THROW({
                     db_->dispose(pool_test, schema_test, collection_test, 999);
                 }, babrat::db_storage_error);
}

TEST_F(DBServerTest, testObtainBetweenDb) {
    // Подробное тестирование получения диапазона
    constexpr std::string pool_test = "range_test";
    constexpr std::string schema_test = "test_schema";
    constexpr std::string collection_test = "test_collection";

    db_->add_pool(pool_test);
    db_->add_schema(pool_test, schema_test);
    db_->add_collection(pool_test, schema_test, collection_test);

    // Заполняем тестовыми данными
    for (int i = 0; i < 1000; i += 10) {
        db_->add(pool_test, schema_test, collection_test, i, i);
    }

    // Проверяем различные варианты границ
    auto test_range = [&](int low, int high, bool inc_low, bool inc_high, size_t expected) {
        auto result = db_->obtain_between(pool_test, schema_test, collection_test,
                                          low, high, inc_low, inc_high);
        EXPECT_EQ(result.size(), expected);
    };

    test_range(0, 100, true, true, 11);
    test_range(0, 100, true, false, 10);
    test_range(0, 100, false, true, 10);
    test_range(0, 100, false, false, 9);
    test_range(-10, 10, true, true, 2);
    test_range(1000, 2000, true, true, 0);

}
class DBServerParamTest : public DBServerTest,
                          public ::testing::WithParamInterface<std::tuple<int,int>> {};



TEST_P(DBServerParamTest, testWithDifferentDataTypes) {
    auto [first, second] = GetParam();

    db_->add_pool("test");
    db_->add_schema("test", "test");
    db_->add_collection("test", "test", "test");

    db_->add("test", "test", "test", 1, first);
    db_->add("test", "test", "test", 2,second);

    EXPECT_EQ(db_->obtain("test", "test", "test", 1), first);
    EXPECT_EQ(db_->obtain("test", "test", "test", 2), second);

}

INSTANTIATE_TEST_SUITE_P(
        DataTypeTests,
        DBServerParamTest,
        ::testing::Values(
                std::make_tuple(42, 32),
                std::make_tuple(0, 1111),
                std::make_tuple(-1, 1111)
        )
);