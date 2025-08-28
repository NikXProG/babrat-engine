

#include "babrat/common/exception.hpp"

#include <sstream>
#include <regex>

#include <../../include/babrat/common/error/not_implemented_error.hpp>
#include <babrat/storage/db_wal_serializer.hpp>

namespace babrat {

    namespace storage {

        /*WALMask::WALMask()
        {
            // default realize not initialized field
        }

        WALMask::WALMask(
                std::uint64_t timestamp,
                WalOperationType type,
                std::string data) :
         m_type(type),
        m_timestamp(timestamp),
        m_data(std::move(data)) {}

        WalOperationType WALMask::getType() const {
            return m_type;
        }

        const std::string& WALMask::getData() const {
            return m_data;
        }

        std::uint64_t WALMask::getTimestamp() const {
            return m_timestamp;
        }

        void WALMask::setData(const std::string& data)
        {
            m_data = data;
        }

        void WALMask::setTimestamp(std::uint64_t timestamp)
        {
            m_timestamp = timestamp;
        }

        void WALMask::setType(WalOperationType type)
        {
            m_type = type;
        }

        std::string operationToString(WalOperationType type)
        {

          switch (type) {
            case WalOperationType::INSERT:
              return "INSERT";
            case WalOperationType::DELETE:
              return "DELETE";
            case WalOperationType::UPDATE:
              return "UPDATE";
            default:
              throw babrat::UnknownTypeException("operationType");
          }

        }*/

        /*
        void WALMask::encode(std::vector<uint8_t>& encoded) const {

            if (!encoded.empty()) {
                throw std::runtime_error("WAL mask encoded is not empty");
            }

            if (m_type == WalOperationType::INVALID) {
                throw std::runtime_error(
                    "type is not defined, must be different from type `INVALID` (define it via `setType`)");
            }


            babrat::encode(encoded, m_timestamp);
            #1#

            babrat::encode(encoded, operationToString(m_type));

            babrat::encode(encoded, 300);

            std::cout << "300" << std::endl;


            std::cout << encoded.data() << std::endl;

        }

        void WALMask::decode(std::vector<uint8_t>& decoded) {

            if (decoded.empty()) {
                throw std::runtime_error("Empty encoded data");
            }


            uint8_t type_val;
            babrat::decode(decoded, type_val);
            m_type = static_cast<WalOperationType>(type_val);

            babrat::decode(decoded,  m_data);#1#

        }
        */


    } // namespace storage

} // namespace babrat




