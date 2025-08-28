#ifndef BABRAT_CREATE_DATABASE_H
#define BABRAT_CREATE_DATABASE_H
#include <fmt/format.h>

#include <babrat/statement/ddl/create_statement.hpp>
#include <babrat/common/string_accumulator.hpp>
#include <babrat/statement/ddl/ddl_statement.hpp>

namespace babrat {

    namespace statement {

        class CreateDatabase final : public CreateStatement, public IDdlStatement {

        public:

            explicit CreateDatabase(
                    const std::string& dbName,
                    bool isNotExists) :
                    CreateStatement(dbName, isNotExists){}

        public:

            using CreateStatement::getName;
            using CreateStatement::isNotExists;

        public:

            std::string toString() const override {
                return StringAccumulator("CreateDatabase")
                        .accumulate("name", getName())
                        .accumulate("isNotExists", isNotExists())
                        .str();
            }

        };

    } // namespace statement

} // namespace babrat

#endif //BABRAT_CREATE_DATABASE_H
