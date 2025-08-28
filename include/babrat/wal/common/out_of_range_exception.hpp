#pragma once

#include <babrat/common/exception.hpp>

namespace babrat {

    namespace wal {

        class OutOfRangeWalException final : public WalException {

        public:

            explicit OutOfRangeWalException();

        };

    } // namespace wal

} // namespace babrat
