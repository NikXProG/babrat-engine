#pragma once

#include <iostream>

#include <boost/flyweight.hpp>

namespace babrat {

    // All objects in the database must be serializable at every level.
    // Otherwise, it will break persistence.
    class Serializable {

    public:

        virtual size_t serialize(std::ostream& stream) const = 0;

        virtual size_t deserialize(std::istream& stream) = 0;

    public:

        virtual size_t serializedSize() const noexcept = 0;

    public:

        virtual ~Serializable() noexcept = default;

    };

    // Returns the byte count for different data types (used in serializableSize).
    template <typename T>
    size_t serializedSizeFor(const T& value) {
        // If an entity inherits from Serializable, its size is determined accordingly.
        // Otherwise, default cases.
        if constexpr (std::is_base_of_v<Serializable, T>) {
            return value.serializedSize();
        } else if constexpr (std::is_same_v<T, std::string>) {
            return sizeof(uint32_t) + value.size();
        } else {
            return sizeof(T);
        }
    }

    // Provides static methods for deserializing objects from an input stream.
    // Specializations must implement `read()` for custom types.
    template <typename T, typename U = void>
    class stream_reader
    {

    public:
        static size_t read(std::istream& is, T& value);

    };

    // Provides static methods for serializing objects to an output stream.
    // Specializations must implement `write()` for custom types.
    template<typename T, typename U = void>
    class stream_writer
    {

    public:

        static size_t write(std::ostream& os, const T& value);

    };

    // concept checking complex case for serialization objects
    template <typename T>
    using only_if_serializable =
            std::enable_if_t<std::is_base_of_v<Serializable, T>>;


    template<typename T>
    class stream_writer<T, only_if_serializable<T>>
    {
    public:

        static size_t write(std::ostream& os, const T& value){
            return value.serialize(os);
        }

    };

    ///reader for hard obj
    template<typename T>
    class stream_reader<T, only_if_serializable<T>> {
    public:

        static size_t read(std::istream& is, T& value) {

            return value.deserialize(is);
        }
    };

    ///writer for base variant
    template<typename T, typename U>
    size_t stream_writer<T, U>::write(std::ostream& os, const T& value)
    {
        const auto pos = os.tellp();
        os.write(reinterpret_cast<const char*>(&value), sizeof(value));
        return static_cast<std::size_t>(os.tellp() - pos);
    }

    ///reader for base variant
    template<typename T, typename U>
    size_t stream_reader<T, U>::read(std::istream& is, T& value)
    {

        const auto pos = is.tellg();
        is.read(reinterpret_cast<char*>(&value), sizeof(value));
        return static_cast<std::size_t>(is.tellg() - pos);
    }

    template<typename T>
    using only_if_string =
            std::enable_if_t<std::is_same_v<T, std::string>>;

    template<typename T>
    class stream_writer<T, only_if_string<T>>
    {
    public:

        static std::size_t write(std::ostream& os, const T& value);

    };


    template<typename T>
    class stream_reader<T, only_if_string<T>>
    {

    public:

        static std::size_t read(std::istream& is, T& value);

    };

    template<typename T>
    std::size_t stream_writer<T, only_if_string<T>>::write(std::ostream& os, const T& value)
    {
        const auto pos = os.tellp();
        const auto len = static_cast<std::uint32_t>(value.size());
        os.write(reinterpret_cast<const char*>(&len), sizeof(len));
        if (len > 0)
            os.write(value.data(), len);
        return static_cast<std::size_t>(os.tellp() - pos);
    }

    template<typename T>
    std::size_t stream_reader<T, only_if_string<T>>::read(std::istream& is, T& value)
    {
        const auto pos = is.tellg();
        std::uint32_t len = 0;
        is.read(reinterpret_cast<char*>(&len), sizeof(len));
        value.resize(len);
        if (len > 0)
            is.read(&value[0], len);
        return static_cast<std::size_t>(is.tellg() - pos);
    }

    template<>
    class stream_writer<boost::flyweight<std::string>, void> {
    public:
        static std::size_t write(std::ostream& os, const boost::flyweight<std::string>& value) {
            return stream_writer<std::string>::write(os, value.get());
        }
    };

    template<>
    class stream_reader<boost::flyweight<std::string>, void> {
    public:
        static std::size_t read(std::istream& is, boost::flyweight<std::string>& value) {
            std::string temp;
            size_t bytes = stream_reader<std::string>::read(is, temp);
            value = boost::flyweight<std::string>(std::move(temp));
            return bytes;
        }
    };


    template<typename T>
    concept String = std::is_same_v<T, std::string>;


    template<typename T>
    concept Container = !String<T> && requires(T a) {
        typename T::value_type;
        typename T::reference;
        typename T::const_reference;
        typename T::iterator;
        typename T::const_iterator;
        typename T::size_type;
        { a.begin() } -> std::same_as<typename T::iterator>;
        { a.end() } -> std::same_as<typename T::iterator>;
        { a.cbegin() } -> std::same_as<typename T::const_iterator>;
        { a.cend() } -> std::same_as<typename T::const_iterator>;
        { a.clear() } -> std::same_as<void>;
    };

    template<typename T>
    concept SequenceContainer =
    Container<T> && requires(T a, typename T::size_type count) {
        { a.resize(count) } -> std::same_as<void>;
    };

    template<typename T>
    concept AssociativeContainer =
    Container<T> && requires {
        typename T::key_type;
    };



    template<typename T>
    requires SequenceContainer<T>
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

    ///automapper for stream_writer
    template<typename T>
    std::size_t write(std::ostream& os, const T& value)
    {
        return stream_writer<T>::write(os, value);
    }

    ///automapper for stream_reader
    template<typename T>
    std::size_t read(std::istream& os, T& value)
    {
        return stream_reader<T>::read(os, value);
    }


} // namespace babrat