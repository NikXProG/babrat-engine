#pragma once

#include <babrat/common/exception.hpp>

namespace babrat {

    namespace wal {

        class CorruptWalException final : public WalException {

        public:

            explicit CorruptWalException();

        };

    } // namespace wal

} // namespace babrat