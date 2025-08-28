#include "wal_mask.h"
#include <sstream>
#include <regex>



namespace babrat {

    namespace storage {

        WALMask::WALMask(std::uint64_t timestamp,
                Operation type,
                std::string data) :
        m_timestamp(timestamp),
        m_type(type),
        m_data(std::move(data)) {}


        babrat::storage::Operation babrat::storage::WALMask::getType() const {
            return m_type;
        }

        const std::string& WALMask::getData() const {
            return m_data;
        }

        std::uint64_t babrat::storage::WALMask::getTimestamp() const {
            return m_timestamp;
        }


        std::string operationToString(Operation op){

            switch (op) {
                case Operation::DELETE:
                    return "DELETE";
                case Operation::UPDATE:
                    return "UPDATE";
                case Operation::INSERT:
                    return "INSERT";
            }

            throw std::runtime_error("Unknown operation type");

        }

        Operation stringToOperation(const std::string& token){

            if (token == "INSERT"){
                return Operation::INSERT;
            }else if (token == "DELETE"){
                return Operation::DELETE;
            }else if (token == "UPDATE") {
                return Operation::UPDATE;
            }
            else throw std::runtime_error("Unknown operation type");

        }


        std::vector<uint8_t> WALMask::encode() const{
            std::ostringstream oss;
            oss << m_timestamp << "|" << operationToString(m_type) << "|" << m_data;
            auto str = oss.str();
            return {str.begin(), str.end()};
        }

        void WALMask::decode(const std::vector<uint8_t>& encoded) {
            if (encoded.empty()) {
                throw std::runtime_error("Empty encoded data");
            }

            const std::string str(encoded.begin(), encoded.end());

            const std::regex pattern(R"((\d+)\s*\|\s*(\w+)\s*\|\s*(.*))");
            std::smatch matches;

            if (!std::regex_match(str, matches, pattern) || matches.size() != 4) {
                throw std::runtime_error("Invalid WAL format");
            }

            try {
                m_timestamp = std::stoull(matches[1].str());
            } catch (const std::exception& e) {
                throw std::runtime_error("Invalid timestamp format");
            }

            m_type = stringToOperation(matches[2].str());
            m_data = matches[3].str();
        }


    }
}




