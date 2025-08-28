#include <babrat/wal/wal_format.hpp>
#include <boost/beast/core/detail/base64.hpp>
#include <nlohmann/json.hpp>

#include "babrat/wal/common/corrupt_exception.hpp"

#include <iostream>
#include <sstream>

using json = nlohmann::ordered_json;

static constexpr int JSON_INDENT = 4;

namespace babrat {

    namespace wal {
        
        /* FACTORY FORMAT */

        std::unique_ptr<FormatHandler> FormatFactory::create(
                LogFormat format) {

            switch(format) {
                case LogFormat::Binary: return std::make_unique<BinaryFormat>();
                case LogFormat::Json: return std::make_unique<JsonFormat>();
                case LogFormat::Pipe: return std::make_unique<PipeFormat>();
            }

            throw std::runtime_error("Unknown log format");

        }

        /* BINARY FORMAT */

        std::pair<uint64_t, int> BinaryFormat::readUvarint(const std::vector<uint8_t> &buf) {
            uint64_t x = 0;
            int s = 0;
            const size_t MaxVarintLen64 = 10;

            for (size_t i = 0; i < buf.size(); ++i) {
                auto& b = buf[i];
                if ( i == MaxVarintLen64) {
                    return {0, -(i+1)};
                }
                if (b < 0x80) {

                    if (i == MaxVarintLen64-1 && b > 1) {
                        return {0, -(i+1)};
                    }

                    return {x | (uint64_t(b) << s), i + 1};

                }
                x |= uint64_t(b & 0x7f) << s;
                s += 7;
            }
            return {0, 0};
        }


        int BinaryFormat::loadNextEntry(
                const std::vector<uint8_t> &data) const {
            auto [size, n] = readUvarint(data);
            if (n <= 0) throw std::runtime_error("corrupt uvarint");

            if (static_cast<size_t>(n) > data.size()
                || (data.size() - static_cast<size_t>(n)) < static_cast<size_t>(size)) {
                        throw std::runtime_error("insufficient data");

            }

            return n + static_cast<int>(size);
        }

        std::pair<std::vector<uint8_t>, BPos> BinaryFormat::appendEntry(
                std::vector<uint8_t> dst,
                uint64_t index [[maybe_unused]],
                const std::vector<uint8_t> &data) const {

            size_t pos = dst.size();
            uint64_t len = data.size();

            while (len >= 0x80) {
                dst.push_back(static_cast<uint8_t>(len) | 0x80);
                len >>= 7;
            }
            dst.push_back(static_cast<uint8_t>(len));

            dst.insert(dst.end(), data.begin(), data.end());

            return {dst, BPos{pos, dst.size()}};
        }

        std::vector<std::uint8_t> BinaryFormat::readFormat(const std::vector<std::uint8_t> &data) {
            auto [size, n] = readUvarint(data);

            if (n <= 0) throw std::runtime_error("corrupt data");

            if (data.size() - static_cast<size_t>(n) < size) throw std::runtime_error("corrupt data size");

            std::vector<uint8_t> new_data;

            new_data.resize(size);
            std::copy(
            data.begin() + static_cast<std::vector<uint8_t>::difference_type>(n),
            data.begin() + static_cast<std::vector<uint8_t>::difference_type>(n) +
                static_cast<std::vector<uint8_t>::difference_type>(size),
            new_data.begin()
            );

            return new_data;
        }


        /* JSON FORMAT */

    int JsonFormat::loadNextEntry(const std::vector<uint8_t>& data) const {
            int brace_balance = 0;
            bool inside_string = false;
            auto it = data.begin();
            auto line_end = data.end();

            for (; it != data.end(); ++it) {

                const auto c = *it;

                if (c == '"') {
                    inside_string = !inside_string;
                    continue;
                }


                if (!inside_string) {
                    if (c == '{') {
                        brace_balance++;
                    } else if (c == '}') {
                        brace_balance--;

                        // searching line end json obj
                        if (brace_balance == 0) {
                            line_end = ++it; // auto increment for new line char
                            break;
                        }
                    }
                }
            }

            if (brace_balance != 0 || line_end == data.end()) {
                throw CorruptWalException();
            }

            std::string line(data.begin(), line_end);

            try {

                auto j = json::parse(line);

                if (!j.contains("id") || !j.contains("dt")) {
                    throw CorruptWalException();
                }

                return static_cast<int>(
                    std::distance(data.begin(), line_end) + 1);

            } catch (const json::exception&) {
                throw CorruptWalException();
            }
        }

        std::pair<std::vector<uint8_t>, BPos> JsonFormat::appendEntry(
                std::vector<uint8_t> dst,
                uint64_t index,
                const std::vector<uint8_t> &data) const
        {
            std::vector<uint8_t> result = dst;
            size_t pos = result.size();

            json j;

            // log index
            j["id"] = index;

            try {
                // wal data
                j["dt"] = json::parse(data.begin(), data.end());
            } catch (...) {
                // default string format for invalid data
                j["dt"] = std::string(data.begin(), data.end());
            }

            // dump data + record separator (for iteration between records wal)  + new line
            std::string json_str = j.dump(JSON_INDENT) + '\n';

            result.insert(result.end(), json_str.begin(), json_str.end());

            return {result, {pos, result.size()}};
        }

        std::vector<std::uint8_t> JsonFormat::readFormat(const std::vector<std::uint8_t> &data) {

            try {

                std::string json_str(data.begin(), data.end());

                auto j = json::parse(json_str);

                auto s = j["dt"].get<std::string>();
                std::cout << s << '\n';

                return {s.begin(), s.end()};

                if (s.empty()) {
                    throw CorruptWalException();
                }

                /*
                if (s[0] == '$') {

                    std::string decoded;
                    decoded.resize(boost::beast::detail::base64::decoded_size(s.size() - 1));

                    auto result = boost::beast::detail::base64::decode(
                            &decoded[0],
                            s.data() + 1,
                            s.size() - 1
                    );

                    decoded.resize(result.first);

                    return {decoded.begin(), decoded.end()};
                } else if (s[0] == '+') {

                    return {s.begin() + 1, s.end()};
                }
                */

                throw CorruptWalException();
            }
            catch (const json::exception &) {
                throw CorruptWalException();
            }

        }


        /*PIPE FORMAT*/

        int PipeFormat::loadNextEntry(const std::vector<uint8_t> &data) const {
            auto it = std::find(data.begin(), data.end(), '\n');
            if (it == data.end()) {
                throw std::runtime_error("corrupt pipe entry");
            }

            return static_cast<int>(std::distance(data.begin(), it) + 1);
        }

        std::pair<std::vector<uint8_t>, BPos> PipeFormat::appendEntry(
                std::vector<uint8_t> dst,
                uint64_t index,
                const std::vector<uint8_t> &data) const {

            size_t pos = dst.size();

            const auto index_str = std::to_string(index);

            dst.insert(dst.end(), index_str.begin(), index_str.end());
            dst.push_back('|');

            dst.insert(dst.end(), data.begin(), data.end());
            dst.push_back('\n');

            return {dst, {pos, dst.size()}};
        }

        std::vector<std::uint8_t> PipeFormat::readFormat(const std::vector<std::uint8_t> &data) {

            auto it = std::find(data.begin(), data.end(), '\n');

            if (it == data.end()) {
                throw std::runtime_error("corrupt pipe data");
            }

            std::string line(data.begin(), it);

            auto delim_pos = line.find('|');

            if (delim_pos == std::string::npos) {
                throw std::runtime_error("invalid pipe format");
            }

            std::string data_str = line.substr(delim_pos + 1);

            return {data_str.begin(), data_str.end()};
        }


    } // namespace wal

} // namespace babrat
