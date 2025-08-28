#include <babrat/wal/common/not_found_exception.hpp>

namespace babrat
{

    namespace wal
    {

        NotFoundWalException::NotFoundWalException() :
            WalException("Not found index"){}

    } // namespace wal

} // namespace babrat