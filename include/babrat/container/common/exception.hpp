#pragma once

#include <babrat/common/exception.hpp>

namespace babrat
{

    namespace container
    {

        /**
         * @brief exception type falls key not found in container
         */
        class NotFoundException : public ContainerException
        {

        public:

            BABRAT_API explicit NotFoundException();

        };

        /**
         * exception type falls key is not exist in a container
         */
        class NotExistsException : public ContainerException
        {

        public:

             BABRAT_API explicit NotExistsException();

        };

    } // namespace container

} // namespace babrat