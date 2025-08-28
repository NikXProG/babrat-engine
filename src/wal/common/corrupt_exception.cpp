#include <babrat/wal/common/corrupt_exception.hpp>

namespace babrat
{

    namespace wal
    {

        CorruptWalException::CorruptWalException() :
            WalException("Corrupt data"){}

    } // namespace wal

} // namespace babrat