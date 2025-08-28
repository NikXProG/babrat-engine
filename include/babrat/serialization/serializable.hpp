#pragma once

#include <iostream>
#include "babrat/common/babrat_api.hpp"
#include <boost/flyweight.hpp>
#include "babrat/common/concepts/string.hpp"
#include "babrat/common/concepts/serializable.hpp"
#include "babrat/common/concepts/container.hpp"

namespace babrat {

    /**
     * @brief Base class for all serializable objects
     *
     * All objects in the database must be serializable at every level.
     * Otherwise, persistence will be broken.
     */
    class Serializable {

    public:

        /**
          * @brief Serializes the object into a stream
          * @param[in,out] ostream Output stream for writing (position will be modified)
          * @return Number of bytes written
          */
        BABRAT_API virtual size_t serialize(std::ostream& ostream) const = 0;

        /**
          * @brief Deserializes the object from a stream
          * @param[in,out] istream Input stream for reading (position will be modified)
          * @return Number of bytes read
          */
        BABRAT_API virtual size_t deserialize(std::istream& istream) = 0;

    public:

        /**
          * @brief Returns the size of the serialized object
          * @return Size in bytes
          */
        BABRAT_API virtual size_t serializedSize() const noexcept = 0;

    public:

        virtual ~Serializable() noexcept = default;

    };

    /**
     * @brief Returns the size of serialized value
     * @tparam T Type of the value
     * @param[in] value Value to calculate size for
     * @return Size in bytes
     *
     * If an entity inherits from Serializable, its size is determined accordingly.
     * Otherwise, default cases are used.
     */
    template <typename T>
    size_t serializedSizeFor(const T& value) {

        if constexpr (std::is_base_of_v<Serializable, T>) {
            // object inherits from serializable
            return value.serializedSize();
        } else if constexpr (is_string_v<T>) {
            // case size string (uint32 type) + size string
            return sizeof(uint32_t) + value.size();
        } else {
            //default size
            return sizeof(T);
        }
    }

    //===--------------------------------------------------------------------===//
    // Base templates
    //===--------------------------------------------------------------------===//

    /**
      * @class stream_reader
      * @brief Template class for reading objects from a stream
      * @tparam T Type of object to read
      * @tparam U Helper parameter for SFINAE
      *
      * Provides static methods for deserializing objects from an input stream.
      * Specializations must implement read() for custom types.
      */
    template <typename T, typename U = void>
    class stream_reader
    {

    public:

        /**
          * @brief Reads an object from a stream
          * @param[in,out] istream Input stream (position will be modified)
          * @param[out] value Reference to object to read into
          * @return Number of bytes read
          */
        static size_t read(std::istream& istream, T& value);

    };

    /**
      * @class stream_writer
      * @brief Template class for writing objects to a stream
      * @tparam T Type of object to write
      * @tparam U Helper parameter for SFINAE
      *
      * Provides static methods for serializing objects to an output stream.
      * Specializations must implement write() for custom types.
      */
    template<typename T, typename U = void>
    class stream_writer
    {

    public:

        /**
          * @brief Writes an object to a stream
          * @param[in,out] ostream Output stream (position will be modified)
          * @param[in] value Object to write
          * @return Number of bytes written
          */
        static size_t write(std::ostream& ostream, const T& value);

    };

    /**
      * @brief Specialization of stream_writer for std::string
      * @tparam T std::string type
      */
    template<typename T>
    class stream_writer<T, only_if_string<T>>
    {
    public:

        /**
          * @brief Writes a string to stream
          * @param[in,out] ostream Output stream (position will be modified)
          * @param[in] value String to write
          * @return Number of bytes written
          */
        static std::size_t write(std::ostream& ostream, const T& value);

    };

    /**
      * @brief Specialization of stream_reader for std::string
      * @tparam T std::string type
      */
    template<typename T>
    class stream_reader<T, only_if_string<T>>
    {

    public:

        /**
          * @brief Reads a string from stream
          * @param[in,out] istream Input stream (position will be modified)
          * @param[out] value String to read into
          * @return Number of bytes read
          */
        static std::size_t read(std::istream& istream, T& value);

    };

    /**
      * @brief Specialization of stream_writer for serializable object
      * @tparam T serializable object type
      */
    template<typename T>
    class stream_writer<T, only_if_serializable<T>>
    {
    public:

        /**
          * @brief Writes a serializable object to stream
          * @param[in,out] ostream Output stream (position will be modified)
          * @param[in] value serializable object to write
          * @return Number of bytes written
          */
        static std::size_t write(std::ostream& ostream, const T& value);

    };

    /**
      * @brief Specialization of stream_reader for serializable object
      * @tparam T serializable object type
      */
    template<typename T>
    class stream_reader<T, only_if_serializable<T>>
    {

    public:

        /**
          * @brief Reads a serializable object from stream
          * @param[in,out] istream Input stream (position will be modified)
          * @param[out] value serializable object to read into
          * @return Number of bytes read
          */
        static std::size_t read(std::istream& istream, T& value);

    };

    //===--------------------------------------------------------------------===//
    // Specializations
    //===--------------------------------------------------------------------===//

    // Implementation of primary template writer
    template<typename T, typename U>
    size_t stream_writer<T, U>::write(std::ostream& ostream, const T& value)
    {
        const auto pos = ostream.tellp();
        ostream.write(reinterpret_cast<const char*>(&value), sizeof(value));
        return static_cast<std::size_t>(ostream.tellp() - pos);
    }

    // Implementation of primary template reader
    template<typename T, typename U>
    size_t stream_reader<T, U>::read(std::istream& istream, T& value)
    {

        const auto pos = istream.tellg();
        istream.read(reinterpret_cast<char*>(&value), sizeof(value));
        return static_cast<std::size_t>(istream.tellg() - pos);
    }



    // Implementation of string writer
    template<typename T>
    std::size_t stream_writer<T, only_if_string<T>>::write(std::ostream& ostream, const T& value)
    {
        const auto pos = ostream.tellp();
        const auto len = static_cast<std::uint32_t>(value.size());
        ostream.write(reinterpret_cast<const char*>(&len), sizeof(len));
        if (len > 0)
            ostream.write(value.data(), len);
        return static_cast<std::size_t>(ostream.tellp() - pos);
    }

    // Implementation of string reader
    template<typename T>
    std::size_t stream_reader<T, only_if_string<T>>::read(std::istream& istream, T& value)
    {
        const auto pos = istream.tellg();
        std::uint32_t len = 0;
        istream.read(reinterpret_cast<char*>(&len), sizeof(len));
        value.resize(len);
        if (len > 0)
            istream.read(&value[0], len);
        return static_cast<std::size_t>(istream.tellg() - pos);
    }

    // Implementation of flyweight string writer
    template<>
    class stream_writer<boost::flyweight<std::string>, void> {
    public:
        static std::size_t write(std::ostream& ostream, const boost::flyweight<std::string>& value) {
            return stream_writer<std::string>::write(ostream, value.get());
        }
    };

    // Implementation of flyweight string reader
    template<>
    class stream_reader<boost::flyweight<std::string>, void> {
    public:
        static std::size_t read(std::istream& istream, boost::flyweight<std::string>& value) {
            std::string temp;
            size_t bytes = stream_reader<std::string>::read(istream, temp);
            value = boost::flyweight<std::string>(std::move(temp));
            return bytes;
        }
    };

    // Implementation of serializable object writer
    template<typename T>
    std::size_t stream_writer<T, only_if_serializable<T>>::write(std::ostream& ostream, const T& value)
    {
        return value.serialize(ostream);
    }

    // Implementation of serializable object reader
    template<typename T>
    std::size_t stream_reader<T, only_if_serializable<T>>::read(std::istream& istream, T& value)
    {
        return value.deserialize(istream);
    }

    // Implementation of sequence container writer
    template<typename T> requires SequenceContainer<T>
    class stream_writer<T, void> {
    public:
        static std::size_t write(std::ostream& os, const T& value) {
            const auto pos = os.tellp();
            const auto len = static_cast<std::uint32_t>(value.size());
            os.write(reinterpret_cast<const char*>(&len), sizeof(len));
            auto size = static_cast<std::size_t>(os.tellp() - pos);
            for (const auto& item : value) {
                size += stream_writer<std::remove_cvref_t<decltype(item)>>::write(os, item);
            }
            return size;
        }
    };

    // Implementation of associative container writer
    template<typename T> requires AssociativeContainer<T>
    class stream_writer<T, void> {
    public:
        static std::size_t write(std::ostream& os, const T& value) {
            const auto pos = os.tellp();
            const auto len = static_cast<std::uint32_t>(value.size());
            os.write(reinterpret_cast<const char*>(&len), sizeof(len));
            auto size = static_cast<std::size_t>(os.tellp() - pos);

            for (const auto& [key, val] : value) {
                size += stream_writer<std::remove_cvref_t<decltype(key)>>::write(os, key);
                size += stream_writer<std::remove_cvref_t<decltype(val)>>::write(os, val);
            }
            return size;
        }
    };

    // Implementation of sequence container reader
    template<typename T> requires SequenceContainer<T>
    class stream_reader<T, void> {
    public:
        static std::size_t read(std::istream& is, T& value) {
            const auto pos = is.tellg();

            // Read size
            std::uint32_t len = 0;
            is.read(reinterpret_cast<char*>(&len), sizeof(len));
            value.resize(len);
            std::size_t size = sizeof(len);

            // Read elements
            for (auto& item : value) {
                size += stream_reader<std::remove_cvref_t<decltype(item)>>::read(is, item);
            }

            return static_cast<std::size_t>(is.tellg() - pos);
        }
    };

    // Implementation of associative container reader
    template<typename T> requires AssociativeContainer<T>
    class stream_reader<T, void> {
    public:
        static std::size_t read(std::istream& is, T& value) {
            const auto pos = is.tellg();
            value.clear();

            // Read size
            std::uint32_t len = 0;
            is.read(reinterpret_cast<char*>(&len), sizeof(len));

            std::size_t size = sizeof(len);

            // Read key-value pairs
            for (std::uint32_t i = 0; i < len; ++i) {
                typename T::key_type key;
                typename T::mapped_type val;

                size += stream_reader<std::remove_cvref_t<decltype(key)>>::read(is, key);
                size += stream_reader<std::remove_cvref_t<decltype(val)>>::read(is, val);

                value.emplace(std::move(key), std::move(val));
            }

            return static_cast<std::size_t>(is.tellg() - pos);
        }
    };

    //===--------------------------------------------------------------------===//
    // Helper functions
    //===--------------------------------------------------------------------===//

    /**
      * @brief automapper for stream_writer
      */
    template<typename T>
    std::size_t write(std::ostream& os, const T& value)
    {
        return stream_writer<T>::write(os, value);
    }

    /**
      * @brief automapper for stream_reader
      */
    template<typename T>
    std::size_t read(std::istream& os, T& value)
    {
        return stream_reader<T>::read(os, value);
    }


} // namespace babrat