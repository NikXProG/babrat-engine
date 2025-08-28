
#include <babrat/wal/wal_struct.hpp>

namespace fs = std::filesystem;

namespace babrat  {

    namespace wal {

        Segment::Segment(std::uint64_t idx, fs::path p) :
            index(idx),
            path(std::move(p)) {}


        Segment::~Segment()
        {
            ebuf.clear();
            epos.clear();
        }

        void WriteBatch::write(uint64_t index, const std::vector<uint8_t>& data)
        {
            entries.push_back({index, data.size()});
            datas.insert(datas.end(), data.begin(), data.end());
        }

        WriteBatch::~WriteBatch()
        {
            clear();
        }

        void WriteBatch::clear()
        {
            entries.clear();
            datas.clear();
        }

    } // namespace wal

} // namespace babrat
