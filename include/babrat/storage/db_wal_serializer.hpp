#pragma once


#include "common/convert_operation.hpp"
#include "common/wal_encoder.hpp"

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include <babrat/storage/interfaces/wal_struct.hpp>


namespace babrat {

    namespace storage {


        /*
        struct WalMeta {

            std::uint64_t timestamp;
            WalLevelType level;
            WalOperationType type;


        };


        class DbWalFormatter {

        public:

        public:

            virtual ~DbWalFormatter() = default;

        public:

            virtual void encode(
                    const WalMeta& data,
                    const std::vector<uint8_t>& serializedData,
                    std::vector<uint8_t>& walData) = 0;

            virtual void decode(
                const std::vector<uint8_t>& walData,
                WalMeta& meta,
                std::vector<uint8_t>& serializedData) = 0;

        };

        class JsonWalFormatter : public DbWalFormatter {

        public:

            ~JsonWalFormatter() override = default;

        public:

            void encode(
                const WalMeta& data,
                const std::string& serializeData,
                std::vector<uint8_t>& walData) override
            {
                json j;

                // timestamp std::uint64 ( 10-10-10-20:20:20 => time sponch seconds )
                j["ts"] = data.timestamp;

                j["lvl"] = EnumToString<WalLevelType>(data.level);
                j["op"] = EnumToString<WalOperationType>(data.type);

                std::string encodedData;

                encodedData.resize(
                    boost::beast::detail::base64::encoded_size(serializeData.size()));

                boost::beast::detail::base64::encode(encodedData.data(),
                                                  serializeData.data(),
                                                   serializeData.size());

                //binary data
                j["bin"] = encodedData;

                std::string json = j.dump();

                walData.insert(
                    walData.end(),
                    json.begin(),
                    json.end());

            };

            void decode(
                const std::vector<uint8_t>& walData,
                WalMeta& meta,
                const std::string& serializedData) override
            {

                std::string jsonStr(walData.begin(), walData.end());

                // parsing JSON
                json j = json::parse(jsonStr);

                meta.timestamp = j["ts"].get<uint64_t>();
                meta.level = StringToEnum<WalLevelType>(j["lvl"].get<std::string>());
                meta.type = StringToEnum<WalOperationType>(j["op"].get<std::string>());


                auto encodedData = j["bin"].get<std::string>();


                const std::size_t max_size =
                    boost::beast::detail::base64::decoded_size(encodedData.size());

                std::string result;
                result.resize(max_size);


                boost::beast::detail::base64::decode(
                    result.data(),
                    encodedData.data(),
                    encodedData.size()
                );



            }

        };


        class WALMask final : public IWALStruct {

        public:

            BABRAT_API WALMask();

            BABRAT_API explicit WALMask(
                    std::uint64_t timestamp,
                    WalOperationType type,
                    std::string data);

        public:

            ~WALMask() noexcept override = default;

        public:

            BABRAT_API WalOperationType getType() const override;

            BABRAT_API const std::string &getData() const override;

            BABRAT_API std::uint64_t getTimestamp() const override;

        public:

            BABRAT_API void setType(WalOperationType type) override;

            BABRAT_API void setData(const std::string& data)  override;

            BABRAT_API void setTimestamp(std::uint64_t timestamp) override;

        public:

            BABRAT_API void encode(std::vector<uint8_t>& encoded) const override;

            BABRAT_API void decode(std::vector<uint8_t>& decoded) override;#1#


            void write(
                std::uint64_t timestamp,
                WalOperationType type,
                const std::vector<uint8_t>& data) override;
                #1#

        private:


            WalOperationType m_type;
            std::uint64_t m_timestamp;
            std::string m_data;

        };*/

    } // namespace storage

} // namespace babrat