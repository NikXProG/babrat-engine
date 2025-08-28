#include <babrat/container/common/exception.hpp>

namespace babrat
{

    namespace container
    {

        NotExistsException::NotExistsException() :
            ContainerException("Key already exists") {}

        NotFoundException::NotFoundException() :
            ContainerException("Key not found") {}

    } // namespace container

} // namespace babrat