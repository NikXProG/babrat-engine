#pragma once

#include <babrat/common/exception.hpp>

namespace babrat{

    namespace wal {

        class ClosedFileWalException final : public WalException {

        public:

            explicit ClosedFileWalException();

        };

    } // namespace wal

} // namespace babrat