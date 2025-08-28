#include <babrat/wal/common/out_of_range_exception.hpp>

namespace babrat
{

    namespace wal
    {

        OutOfRangeWalException::OutOfRangeWalException() :
            WalException("Out of range index")
        {}


    } // namespace wal

} // namespace babrat

