#pragma once

#include <babrat/statement/sql_statement.hpp>

namespace babrat {

    namespace statement {

        class DropStatement : public SqlStatement {

            std::string m_name;
            bool m_isExists;

        public:

            explicit DropStatement(
                    std::string name,
                    bool isExists) :
                    m_name(std::move(name)),
                    m_isExists(isExists){

                if (m_name.empty()) {
                    throw std::invalid_argument("Database name cannot be empty");
                }

            }

        public:

            std::string const& getName() const & {
                return m_name;
            }

            std::string getName() && {
                return std::move(m_name);
            }

            bool isExists() const {
                return m_isExists;
            }


        public:

            DropStatement(const DropStatement&) = delete;
            DropStatement& operator=(const DropStatement&) = delete;

        public:

            DropStatement(DropStatement&&) = default;
            DropStatement& operator=(DropStatement&&) = default;

        public:

            ~DropStatement() override = default;


        };

    } // statement


} // babrat