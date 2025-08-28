#include <../../include/babrat/common/error/stream_error.hpp>

namespace babrat {

    StreamError::StreamError(const std::string &message) :
            IOError(message){}

} // namespace babrat
