#pragma once

#include <babrat/common/exception.hpp>

namespace babrat
{

    namespace wal
    {

        class BoundaryIndexWalException final : public WalException {

        public:

            explicit BoundaryIndexWalException();

        };

    } // namespace wal

} // namespace babrat
