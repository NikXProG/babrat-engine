#ifndef BABRAT_WAL_FORMAT_H
#define BABRAT_WAL_FORMAT_H
#include "wal.h"

namespace babrat::wal  {

    class FormatHandler {

    public:

        virtual ~FormatHandler() = default;

    public:

        virtual int loadNextEntry(const std::vector<uint8_t>& data) const = 0;

        virtual std::pair<std::vector<uint8_t>, BPos> appendEntry(
                std::vector<uint8_t> dst,
                std::uint64_t index,
                const std::vector<uint8_t>& data) const = 0;

    public:

        virtual std::vector<uint8_t> readFormat(
                const std::vector<std::uint8_t>& data) = 0;

    };


    class BinaryFormat : public FormatHandler {

    private:

        static std::pair<uint64_t, int> readUvarint(const std::vector<uint8_t>& buf);

    public:

        ~BinaryFormat() noexcept = default;

    public:

        int loadNextEntry(const std::vector<uint8_t>& data) const override;

        std::pair<std::vector<uint8_t>, BPos> appendEntry(
                std::vector<uint8_t> dst,
                uint64_t index,
                const std::vector<uint8_t>& data) const override;

    public:

        std::vector<std::uint8_t> readFormat(const std::vector<std::uint8_t>& data) override;

    };

    class JsonFormat : public FormatHandler {

    public:

        int loadNextEntry(const std::vector<uint8_t>& data) const override;

        std::pair<std::vector<uint8_t>, BPos> appendEntry(
                std::vector<uint8_t> dst,
                std::uint64_t index,
                const std::vector<uint8_t>& data) const override;

    public:

        std::vector<std::uint8_t> readFormat(const std::vector<std::uint8_t>& data) override;


    };

    class FormatFactory {

    public:

        static std::unique_ptr<FormatHandler> create(babrat::wal::LogFormat format);

    };

    class PipeFormat : public FormatHandler {

    public:

        int loadNextEntry(const std::vector<uint8_t>& data) const override;

        std::pair<std::vector<uint8_t>, BPos> appendEntry(
                std::vector<uint8_t> dst,
                std::uint64_t index,
                const std::vector<uint8_t>& data) const override;

    public:

        std::vector<std::uint8_t> readFormat(const std::vector<std::uint8_t>& data) override;


    };

}


#endif //BABRAT_WAL_FORMAT_H
