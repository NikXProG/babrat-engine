#ifndef BABRAT_WAL_EXCEPTIONS_H
#define BABRAT_WAL_EXCEPTIONS_H
#include <stdexcept>

namespace babrat::wal {

    class boundary_index_error : public std::out_of_range {

    public:

        explicit boundary_index_error() : std::out_of_range("Index reached the boundary limit") {}

    };

    class corrupt_error : public std::runtime_error {

    public:

        corrupt_error() : std::runtime_error("corrupt data") {}

    };

    class closed_file_error : public std::runtime_error{

    public:

        explicit closed_file_error() : std::runtime_error("ClosedFileError: file-log is closed"){}

    };

    class not_found_error : public std::invalid_argument{

    public:

        explicit not_found_error() : std::invalid_argument("NotFoundError: not found index"){}

    };

    class out_of_range_error : public std::invalid_argument{

    public:

        explicit out_of_range_error() : std::invalid_argument("OutOfRangeError: out of range index"){}

    };

}
#endif //BABRAT_WAL_EXCEPTIONS_H
