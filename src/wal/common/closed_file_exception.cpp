#include <babrat/wal/common/closed_file_exception.hpp>

namespace babrat
{

    namespace wal
    {

        ClosedFileWalException::ClosedFileWalException() :
            WalException("File-log is closed"){}

    } // namespace wal

} // namespace babrat