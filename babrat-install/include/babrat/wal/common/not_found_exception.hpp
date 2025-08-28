#pragma once

#include <babrat/common/exception.hpp>

namespace babrat {

    namespace wal {

        class NotFoundWalException final : public WalException {

        public:

            explicit NotFoundWalException();

        };

    } // namespace wal

} // namespace babrat