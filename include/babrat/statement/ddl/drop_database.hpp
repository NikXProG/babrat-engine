#pragma once

#include <babrat/statement/ddl/drop_statement.hpp>
#include <babrat/statement/ddl/ddl_statement.hpp>
#include <babrat/common/string_accumulator.hpp>

namespace babrat {

    namespace statement {

        class DropDatabase : public DropStatement, public IDdlStatement  {

            bool m_isForce;

        public:

            explicit DropDatabase(
                    const std::string& dbName,
                    bool isExists,
                    bool isForce) :
                    DropStatement(dbName, isExists),
                    m_isForce(isForce) {}

        public:

            using DropStatement::getName;
            using DropStatement::isExists;

        public:

            bool isForce() const {
                return m_isForce;
            }

        public:

            std::string toString() const override {
                return StringAccumulator("DropDatabase")
                        .accumulate("name", getName())
                        .accumulate("isExists", isExists())
                        .accumulate("isForce", isForce())
                        .str();
            }


        };


    } // namespace statement

} // namespace babrat

