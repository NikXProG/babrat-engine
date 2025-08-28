#include "gtest/gtest.h"
#include "wal_default.h"
#include <filesystem>
#include <random>

#define SAVE_CATALOG_TEST true

namespace fs = std::filesystem;

std::string dataStr(uint64_t index) {
    if (index % 2 == 0) {
        return std::format("data-\"{}\"", index);
    }
    return std::format("data-'{}'", index);
}

std::string jsonDataStr(std::uint64_t index) {
    return std::format(R"({{"data": {}}})", index);
}

void log(const fs::path& test_dir, babrat::wal::Options options, int N){

    babrat::wal::WALManager wal(test_dir, options);

    std::uint64_t last = wal.lastIndex();

    for(std::uint64_t i = last; i <= last + N; i++){
        auto str =  options.log_format != babrat::wal::LogFormat::JSON ?
                    dataStr(i) :
                    jsonDataStr(i);

        std::vector<std::uint8_t> vect(str.begin(), str.end());

        try{
            wal.write(i, vect);
        } catch(const babrat::wal::boundary_index_error&){

        }
    }

}

class WALManagerTest : public ::testing::Test {

protected:

    void SetUp() override {
        test_dir_ = "testing";
        options_.segment_size = 1024*1024;
        options_.segment_cache_size = 5;
        options_.no_sync = true;
        options_.log_format = babrat::wal::LogFormat::PIPE;
    }

    void TearDown() override {
        if (!SAVE_CATALOG_TEST) fs::remove_all(test_dir_);
    }

    fs::path test_dir_;
    babrat::wal::Options options_;
};

TEST_F(WALManagerTest, IndexCheckStateTest) {

    const char* const test_name =
            testing::UnitTest::GetInstance()->current_test_info()->name();

    fs::path that_test_dir = test_dir_ / test_name;

    babrat::wal::WALManager wal(that_test_dir, options_);

    EXPECT_EQ(wal.firstIndex(), 0);
    EXPECT_EQ(wal.lastIndex(), 0);

}

TEST_F(WALManagerTest, WriteAndReadTest) {

    const char* const test_name =
            testing::UnitTest::GetInstance()->current_test_info()->name();

    fs::path that_test_dir = test_dir_ / test_name;

    const int N = 5000;

    log(that_test_dir, options_, N);

    babrat::wal::WALManager wal(that_test_dir, options_);

    auto last = wal.lastIndex();

    std::cout << last << std::endl;

    for (int i = 1; i <= last; ++i) {
        std::cout << wal.read(i).data() << std::endl;
        EXPECT_NO_THROW(wal.read(i));
    }

}
TEST_F(WALManagerTest, ConcurrentWriteAndRead) {

    const char* const test_name =
            testing::UnitTest::GetInstance()->current_test_info()->name();

    fs::path that_test_dir = test_dir_ / test_name;

    const int N = 100;

    options_.log_format = babrat::wal::LogFormat::BINARY;


    babrat::wal::WALManager wal(that_test_dir, options_);


    auto writer = [&](int start) {

        for (int i = start; i <= start + N; ++i) {

            auto str = dataStr(i);

            std::vector<uint8_t> data(str.begin(), str.end());

            try{
                wal.write(i, data);
                wal.sync();
            }catch(const babrat::wal::boundary_index_error&){

            }catch(const std::exception&){
                throw;
            }
        }

    };

    auto last = wal.lastIndex();

    std::thread t1(writer, last);

    std::thread t2(writer, last+N);

    std::thread t3(writer, last+N*2);

    t1.join();
    t2.join();
    t3.join();




}

