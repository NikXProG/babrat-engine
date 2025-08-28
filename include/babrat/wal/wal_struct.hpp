#pragma once

#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

namespace babrat  {

    namespace wal {

        class BPos {

        public:

            size_t pos;
            size_t end;

        };

        class Segment {

        public:

            uint64_t index;
            fs::path path;
            std::vector<uint8_t> ebuf;
            std::vector<BPos> epos;

        public:

            Segment(std::uint64_t idx, fs::path p);

        public:

            ~Segment();

        };

        class WriteBatch {

        private:

            struct Entry {
                uint64_t index;
                size_t size;
            };

        public:

            void write(uint64_t index, const std::vector<uint8_t>& data);

        public:

            ~WriteBatch();

        public:

            void clear();

        public:

            std::vector<Entry> entries;
            std::vector<uint8_t> datas;

        };

    } // namespace wal

} // namespace babrat

