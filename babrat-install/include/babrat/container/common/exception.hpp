#pragma once

#include <babrat/common/exception.hpp>

namespace babrat
{

    namespace container
    {

        class NotFoundException : public ContainerException
        {

        public:

            explicit NotFoundException();

        };


        class NotExistsException : public ContainerException
        {

        public:

            explicit NotExistsException();

        };



        class CreateIfExistsException : public ContainerException
        {

        private:

            std::string contextCall;

        public:

            explicit CreateIfExistsException(const std::string& contextCall);

        public:

            const std::string& getContextCall() const;

        };



    } // namespace container

} // namespace babrat