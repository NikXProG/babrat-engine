#pragma once

#include "convert_timestamp.h"

#include "wal_mask.h"

#include "wal_default.h"

#include "serializable.h"

#include "not_implemented.h"

#include <utility>

namespace babrat {

    namespace storage {

        class WalEncoder {
        public:
            template<typename... Args>
            static std::string encode(Args&&... args) {
                std::ostringstream stream;

                (babrat::write(stream, std::forward<Args>(args)), ...);

                std::string plain = stream.str();

                std::string encoded;
                encoded.resize(boost::beast::detail::base64::encoded_size(plain.size()));

                auto encoded_size = boost::beast::detail::base64::encode(
                        encoded.data(), plain.data(), plain.size()
                );
                encoded.resize(encoded_size);
                return encoded;
            }
        };

        class IDatabaseWalManager : public Serializable {

        public:

            ~IDatabaseWalManager() override = default;

        public:

            virtual void write(const Encodable& entry) = 0;

            virtual void read(Encodable& entry) = 0;

        public:

            virtual std::uint64_t firstIndex() = 0;

            virtual std::uint64_t lastIndex() = 0;

        public:

            virtual void truncateRange(std::uint64_t low_limit, std::uint64_t upper_limit) = 0;

            virtual void truncateFront(std::uint64_t index) = 0;

            virtual void truncateBack(std::uint64_t index) = 0;


        };

        /**
         * Manages Write-Ahead Logging (WAL) operations for the database.
         * Implements Serializable interface for persistence.
         */
        class DatabaseWalManager : public IDatabaseWalManager {

            std::uint64_t m_indexWal;
            std::unique_ptr<wal::WAL> m_wal;

        public:

            explicit DatabaseWalManager(std::unique_ptr<wal::WAL> wal) :
                m_wal(std::move(wal)) {
                m_indexWal = m_wal->lastIndex();
            }

            explicit DatabaseWalManager(std::string path,
                                        const wal::Options& options) :
                    DatabaseWalManager(wal::WalFactory::create(
                            std::move(path), options)){}

            explicit DatabaseWalManager(std::string path) :
                    DatabaseWalManager(wal::WalFactory::create(std::move(path))){}

            DatabaseWalManager() :
                DatabaseWalManager(wal::WalFactory::create()){}

        public:

            std::uint64_t firstIndex() override {
                return m_wal->firstIndex();
            }

            std::uint64_t lastIndex() override {
                return m_wal->lastIndex();
            }

        public:

            void truncateRange(std::uint64_t low_limit, std::uint64_t upper_limit) {
                throw not_implemented("truncateRange", "truncateRange() is not implemented");
            }

            void truncateFront(std::uint64_t index) {
                throw not_implemented("truncateFront", "truncateFront() is not implemented");
            }

            void truncateBack(std::uint64_t index) {
                throw not_implemented("truncateBack", "truncateBack() is not implemented");
            }

        public:

            void write(const Encodable& data) override {
                if (!m_wal) {
                    throw std::runtime_error("WAL is not initialized");
                }

                try{
                    m_wal->write(m_indexWal++, data.encode());
                }catch(const wal::boundary_index_error& ex){

                }

            }

            void read(Encodable& data) override {
                if (!m_wal) {
                    throw std::runtime_error("WAL is not initialized");
                }
                auto first = m_wal->firstIndex();
                auto last = m_wal->lastIndex();

                auto vec = m_wal->read(m_indexWal);

                data.decode(vec);

            }

        public:


            size_t serialize(std::ostream& stream) const override {

                if (!stream.good()) {
                    throw std::runtime_error("Stream is not in good state");
                }

                size_t total_bytes = 0;

                total_bytes += babrat::write(stream, m_indexWal);

                return total_bytes;

            }

            size_t deserialize(std::istream& stream) override {

                if (!stream.good()) {
                    throw std::runtime_error("Stream is not in good state");
                }

                size_t total_bytes = 0;

                total_bytes += babrat::read(stream, m_indexWal);

                return total_bytes;
            }

            [[nodiscard]] size_t serializedSize() const noexcept override {
                return sizeof(m_indexWal);
            }

        public:

            DatabaseWalManager(const DatabaseWalManager&) = delete;
            DatabaseWalManager& operator=(const DatabaseWalManager&) = delete;

            DatabaseWalManager(DatabaseWalManager&&) = default;
            DatabaseWalManager& operator=(DatabaseWalManager&&) = default;



        };

    }

}
