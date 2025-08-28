#pragma once

#include <sstream>
#include "babrat/serialization/serializable.hpp"
#include <boost/beast/core/detail/base64.hpp>

namespace babrat{

    namespace storage {

        class WalEncoder {
        public:
            template<typename... Args>
            static std::string encode(Args&&... args) {
                std::ostringstream stream;

                (babrat::write(stream, std::forward<Args>(args)), ...);

                std::string plain = stream.str();

                std::string encoded;
                encoded.resize(boost::beast::detail::base64::encoded_size(plain.size()));

                auto encoded_size = boost::beast::detail::base64::encode(
                        encoded.data(), plain.data(), plain.size()
                );
                encoded.resize(encoded_size);
                return encoded;
            }
        };

    }
} // namespace babrat