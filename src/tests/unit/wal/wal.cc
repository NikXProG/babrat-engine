#include "gtest/gtest.h"
#include "wal_default.h"
#include <filesystem>
#include <random>

std::string dataStr(uint64_t index) {
    if (index % 2 == 0) {
        return std::format("data-\"{}\"", index);
    }
    return std::format("data-'{}'", index);
}

std::string jsonDataStr(std::uint64_t index) {
    return std::format(R"({{"data": {}}})", index);
}


void log(const std::string& directory, babrat::wal::Options&& options, int N){

    std::unique_ptr<babrat::wal::WAL> wal =
            std::make_unique<babrat::wal::WALManager>(
                    directory,
                    std::forward<babrat::wal::Options>(options));

    auto l = wal->lastIndex();

    for(auto i = l; i <= l+N; i++){

        auto str = jsonDataStr(i);

        std::vector<std::uint8_t> vect(str.begin(), str.end());

        try{
            wal->write(i, vect);
        }catch(const babrat::wal::boundary_index_error& ex){
        }

    }

}

void write_thread(babrat::wal::WALManager* wal, uint64_t start_index, int count) {
    for (uint64_t i = start_index; i < start_index + count; ++i) {
        std::string str = "data for " + std::to_string(i);
        std::vector<uint8_t> data(str.begin(), str.end());

        try {
            wal->write(i, data);
        } catch (const babrat::wal::boundary_index_error& ex) {

        }
    }
}


TEST(WalJsonTest, writeTest){

    log("wal_directory",
        babrat::wal::Options(
                512,
                2,
                babrat::wal::LogFormat::JSON),
        500000);

}

class WALTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_path_ = fs::current_path() / "wal_test";
        fs::remove_all(test_path_);
        fs::create_directories(test_path_);
    }

    void TearDown() override {
        /*if (fs::remove_all(test_path_);*/
    }

    void testFirstLast(
            babrat::wal::WALManager& wal,
            uint64_t expected_first,
            uint64_t expected_last) {

        auto first = wal.firstIndex();
        auto last = wal.lastIndex();

        ASSERT_EQ(first, expected_first);
        ASSERT_EQ(last, expected_last);

        for (uint64_t i = first; i <= last; ++i) {
            auto dt = wal.read(i);

            std::vector<std::uint8_t> vector(dt.begin(), dt.end());

            std::cout << vector.data() << std::endl;

        }



    }

    fs::path test_path_;
};

TEST_F(WALTest, BasicOperations) {


    babrat::wal::Options opts = babrat::wal::Options(
            512, 20, babrat::wal::LogFormat::JSON);

    const int N = 100;

    babrat::wal::WALManager wal(test_path_.string(), opts);

    ASSERT_EQ(wal.firstIndex(), 0);
    ASSERT_EQ(wal.lastIndex(), 0);

    for (int i = 1; i <= N; i++) {

        EXPECT_THROW(wal.write(i - 1, {}), babrat::wal::boundary_index_error);

        auto str = dataStr(i);

        std::vector<uint8_t> data(str.begin(), str.end());
        wal.write(i, data);

        auto read_data = wal.read(i);
        std::string read_str(read_data.begin(), read_data.end());

        std::cout << read_str << " : " << dataStr(i) << std::endl;
        ASSERT_EQ(read_str, dataStr(i));

    }

    ASSERT_EQ(wal.firstIndex(), 1);
    ASSERT_EQ(wal.lastIndex(), N);

    EXPECT_THROW(wal.read(0), babrat::wal::not_found_error);

    for (int i = 1; i <= N; i++) {
        auto read_data = wal.read(i);
        std::string read_str(read_data.begin(), read_data.end());
        ASSERT_EQ(read_str, dataStr(i));
    }

    std::vector<int> indices(N);
    std::iota(indices.begin(), indices.end(), 1);
    std::shuffle(indices.begin(), indices.end(), std::mt19937{std::random_device{}()});

    for (int i: indices) {
        auto read_data = wal.read(i);
        std::string read_str(read_data.begin(), read_data.end());
        ASSERT_EQ(read_str, dataStr(i));
    }

    wal.close();

    EXPECT_THROW(wal.write(1, {}), babrat::wal::closed_file_error);
    EXPECT_THROW(wal.read(1), babrat::wal::closed_file_error);
    EXPECT_THROW(wal.firstIndex(), babrat::wal::closed_file_error);
    EXPECT_THROW(wal.lastIndex(), babrat::wal::closed_file_error);

    babrat::wal::WALManager wal_reopened(test_path_.string(), opts);

    for (int i = 1; i <= N; i++) {
        auto read_data = wal_reopened.read(i);
        std::string read_str(read_data.begin(), read_data.end());
        ASSERT_EQ(read_str, dataStr(i));
    }

    for (int i = N + 1; i <= N + 50; i++) {
        auto str = dataStr(i);

        std::vector<uint8_t> data(str.begin(), str.end());
        wal_reopened.write(i, data);

        auto read_data = wal_reopened.read(i);
        std::string read_str(read_data.begin(), read_data.end());
        ASSERT_EQ(read_str, dataStr(i));
    }


    testFirstLast(wal_reopened, 1, N + 50);

    babrat::wal::WriteBatch batch;

    batch.write(1, {});
    batch.write(2, {});
    batch.write(3, {});

    EXPECT_THROW(wal_reopened.writeBatch(&batch), babrat::wal::boundary_index_error);

    batch.clear();


    for (int i = N + 51; i <= N + 150; i++) {
        std::string str = dataStr(i);
        batch.write(i, {str.begin(), str.end()});

        if (i % 10 == 0) {
            wal_reopened.writeBatch(&batch);
            batch.clear();
        }
    }

    EXPECT_THROW(wal_reopened.truncateFront(0), babrat::wal::out_of_range_error);
    EXPECT_THROW(wal_reopened.truncateFront(N + 151), babrat::wal::out_of_range_error);

    testFirstLast(wal_reopened, 1, N + 150);

    wal_reopened.truncateFront(81);
    testFirstLast(wal_reopened, 81, N + 150);

    auto new_str = dataStr(N + 151);
    std::vector<uint8_t> new_data(new_str.begin(), new_str.end());

    wal_reopened.write(N + 151, new_data);
    testFirstLast(wal_reopened, 81, N + 151);

    wal_reopened.truncateBack(N + 151 - 80);
    testFirstLast(wal_reopened, 81, N + 151 - 80);

    for (int i = 81; i <= N + 151 - 80; i++) {
        auto read_data = wal_reopened.read(i);
        std::string read_str(read_data.begin(), read_data.end());
        ASSERT_EQ(read_str, dataStr(i));
    }

    wal_reopened.close();
    babrat::wal::WALManager wal_after_truncate(test_path_.string(), opts);
    testFirstLast(wal_after_truncate, 81, N + 151 - 80);

    for (int i = 81; i <= N + 151 - 80; i++) {
        auto read_data = wal_after_truncate.read(i);
        std::string read_str(read_data.begin(), read_data.end());
        ASSERT_EQ(read_str, dataStr(i));
    }


}