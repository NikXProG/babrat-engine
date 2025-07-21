#include "../include/wal_format.h"
#include <boost/beast/core/detail/base64.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


/* FACTORY FORMAT */

std::unique_ptr<babrat::wal::FormatHandler> babrat::wal::FormatFactory::create(
        babrat::wal::LogFormat format) {

    switch(format) {
        case babrat::wal::LogFormat::BINARY: return std::make_unique<BinaryFormat>();
        case babrat::wal::LogFormat::JSON: return std::make_unique<JsonFormat>();
        case babrat::wal::LogFormat::PIPE: return std::make_unique<PipeFormat>();
    }

    throw std::runtime_error("Unknown log format");

}

/* BINARY FORMAT */

std::pair<uint64_t, int> babrat::wal::BinaryFormat::readUvarint(const std::vector<uint8_t> &buf) {
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


int babrat::wal::BinaryFormat::loadNextEntry(
        const std::vector<uint8_t> &data) const {
    auto [size, n] = readUvarint(data);
    if (n <= 0) throw std::runtime_error("corrupt uvarint");
    if (uint64_t(data.size() - n) < size) throw std::runtime_error("insufficient data");
    return n + static_cast<int>(size);
}

std::pair<std::vector<uint8_t>, babrat::wal::BPos> babrat::wal::BinaryFormat::appendEntry(
        std::vector<uint8_t> dst,
        uint64_t index,
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

std::vector<std::uint8_t> babrat::wal::BinaryFormat::readFormat(const std::vector<std::uint8_t> &data) {
    auto [size, n] = readUvarint(data);

    if (n <= 0) throw std::runtime_error("corrupt data");
    if (uint64_t(data.size() - n) < size) throw std::runtime_error("corrupt data size");

    std::vector<uint8_t> new_data;

    new_data.resize(size);
    std::copy(
            data.begin() + n,
            data.begin() + n + size,
            new_data.begin()
    );

    return new_data;
}


/* JSON FORMAT */
int babrat::wal::JsonFormat::loadNextEntry(const std::vector<uint8_t> &data) const {

    auto it = std::find(data.begin(), data.end(), '\n');
    if (it == data.end()) {
        throw corrupt_error();
    }


    std::string line(data.begin(), it);

    try {

        auto j = json::parse(line);


        if (!j.contains("data") || !j["data"].is_string()) {
            throw corrupt_error();
        }

        return static_cast<int>(std::distance(data.begin(), it) + 1);
    }
    catch (const json::exception&) {
        throw corrupt_error();
    }

}

std::pair<std::vector<uint8_t>, babrat::wal::BPos> babrat::wal::JsonFormat::appendEntry(
        std::vector<uint8_t> dst,
        uint64_t index,
        const std::vector<uint8_t> &data) const {

    std::vector<uint8_t> result = dst;
    size_t pos = result.size();


    json j;
    j["index"] = std::to_string(index);


    std::string encoded;
    encoded.resize(boost::beast::detail::base64::encoded_size(data.size()));
    encoded.resize(boost::beast::detail::base64::encode(
            &encoded[0],
            data.data(),
            data.size()
    ));

    j["data"] = "$" + encoded;


    std::string json_str = j.dump() + "\n";


    result.insert(result.end(), json_str.begin(), json_str.end());

    return {result, {pos, result.size()}};
}

std::vector<std::uint8_t> babrat::wal::JsonFormat::readFormat(const std::vector<std::uint8_t> &data) {

    try {

        std::string json_str(data.begin(), data.end());

        auto j = json::parse(json_str);


        if (!j.contains("data") || !j["data"].is_string()) {
            throw corrupt_error();
        }

        std::string s = j["data"].get<std::string>();

        if (s.empty()) {
            throw corrupt_error();
        }

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

        throw corrupt_error();
    }
    catch (const json::exception &) {
        throw corrupt_error();
    }

}


/* PIPE FORMAT */


int babrat::wal::PipeFormat::loadNextEntry(const std::vector<uint8_t> &data) const {
    auto it = std::find(data.begin(), data.end(), '\n');
    if (it == data.end()) {
        throw std::runtime_error("corrupt pipe entry");
    }

    return static_cast<int>(std::distance(data.begin(), it) + 1);
}

std::pair<std::vector<uint8_t>, babrat::wal::BPos> babrat::wal::PipeFormat::appendEntry(
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

std::vector<std::uint8_t> babrat::wal::PipeFormat::readFormat(const std::vector<std::uint8_t> &data) {

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