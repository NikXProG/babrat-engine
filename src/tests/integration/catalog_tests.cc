#include "gtest/gtest.h"
#include "convert_column.h"
#include "catalog.h"
/*
class DBServerTest : public ::testing::Test {

protected:
    void SetUp() override {
        db_ = new babrat::DatabaseStorage<Column*, std::vector<Column*>, Traits>();
    }

    void TearDown() override {
        delete db_;
    }

    babrat::DatabaseStorage<Column*, std::vector<Column*>, Traits>* db_{};
};


std::string str_tolower (std::string s)
{
    std::transform(s. begin(), s.end(), s.begin(),
        [] (unsigned char c) { return std::tolower(c); }
    );
    return s;
}*/

TEST(fdsd, failedTestObtainDataDb) {
/*
    auto* catalog = new Catalog(db_);
    catalog->register_defaults();

    auto vect = db_->searchBetween(
            DB_DEFAULT_NAME,
            CATALOG_NAME,
            CATALOG_SCHEMA_NAME,
            make_column(5),
            make_column(30),
            true,
            true);

    std::ostringstream os;

    for ( auto& it : vect){
        for( auto q : it.value){
            if ( q->getType() == typeid(int)){
                if (auto* int_col = dynamic_cast<ColumnType<int>*>(q)) {
                    os << int_col->getValue() << " ";
                }
            }
            else if (q->getType() == typeid(std::string)) {
                if (auto* varchar_col = dynamic_cast<ColumnType<std::string>*>(q)) {
                    os << varchar_col->getValue() << " ";
                }
            }
            else if (q->getType() == typeid(bool)) {
                if (auto* varchar_col = dynamic_cast<ColumnType<bool>*>(q)) {
                    os << (varchar_col->getValue() ? "true" : "false") << " ";
                }
            }

        }
        os << std::endl;
    }

    std::cout << os.str();*/

}
