#include <babrat/wal/common/boundary_index_exception.hpp>

namespace babrat
{

    namespace wal
    {

        BoundaryIndexWalException::BoundaryIndexWalException() :
            WalException("Index reached the boundary limit"){}

    } // namespace wal

} // namespace babrat