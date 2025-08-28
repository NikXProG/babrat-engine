#pragma once

#include <nlohmann/json.hpp>

#include <babrat/serialization/traits.hpp>

namespace babrat {

    namespace serialization {

        /**
          * @brief is the underlying abstraction for serialization
          */
        class Serializer {

        public:

            /**
              * @brief Helper class for serializing lists
              */
            class List {

                friend Serializer;

            private:

                Serializer &serializer;
                explicit List(Serializer &serializer) : serializer(serializer) {}

            public:

                /**
                  * @brief Serialize an element
                  * @tparam T Type of the value
                  * @param[in] value Value to serialize
                  */
                template <class T>
                void writeElement(const T &value)
                {
                    // writing list value to serializer
                    serializer.write(value);
                }

                /**
                  * @brief Serialize an object
                  * @tparam FUNC Callable type
                  * @param[in] f Function that writes object content
                  */
                template <class FUNC>
                void writeObject(FUNC f)
                {
                    // create object
                    serializer.onObjectBegin();
                    // function that accepts a serializer to create nested elements
                    f(serializer);
                    // closes object creation
                    serializer.onObjectEnd();
                }

            };

        public:

            virtual ~Serializer() noexcept = default;

        public:

            /**
              * @brief Serialize a named value
              *
              * @tparam T Type of the value
              * @param[in] name Field name
              * @param[in] value Value to serialize
              */
            template <class T>
            void writeValue(const char *name, const T &value) {
                // create value
                onValueBegin(name);
                // serializing value depending on serialization
                write(value);
                // closes value creation
                onValueEnd();
            }

            /**
              * @brief Serializes the value if it is not supplied with the default value of the type
              * @tparam T Type of the value
              * @param[in] name Field name
              * @param[in] value Value to serialize
              */
            template <class T>
            void writeInitValue(const char *name, const T &value) {
                // create value
                onValueBegin(name);
                // first checks if the type is equal to the default value
                if (!SerializationDefaultValue::IsDefault<T>(value)){
                    write(value);  // serializing value depending on serialization
                }
                // closes value creation
                onValueEnd();
            }

            /**
              * @brief Serializes the value if it is not supplied with default value parameter
              * @tparam T Type of the value
              * @param[in] name Field name
              * @param[in] value Value to serialize
              */
            template <class T>
            void writeInitValue(const char *name, const T &value, const T &defaultValue) {
                // create value
                onValueBegin(name);
                // first checks if the type is equal to the param default value
                if (value != defaultValue) {
                    write(value); // serializing value depending on serialization
                }
                // closes value creation
                onValueEnd();
            }

            /**
              * @brief Serialize a named object
              *
              * the function creates nested elements via the serializer object
              *
              * @note must have the signature:
              * @code{.cpp}
              * void(serialization::Serializer::Serializer& ser)
              * @endcode
              *
              *
              * @tparam FUNC Callable type
              * @param[in] name Field name
              * @param[in] f Function that writes object content
              */
            template <class FUNC>
            void writeObject(const char *name, FUNC f) {
                // create value
                onValueBegin(name);
                // create object
                onObjectBegin();
                // function that accepts this serializer to create nested elements
                f(*this);
                // closes object creation
                onObjectEnd();
                // closes value creation
                onValueEnd();
            }

            /**
              * @brief Serialize a named list
              *
              * function creates nested elements via list object
              *
              * @note must have the signature:
              * @code{.cpp}
              * void(serialization::Serializer::List& list, std::uint64_t index)
              * @endcode
              *
              * @tparam FUNC Callable type
              * @param[in] name Field name
              * @param[in] count Number of elements
              * @param[in] func Function that writes list elements
              */
            template <class FUNC>
            void writeList(const char *name, std::uint64_t count, FUNC func) {
                // create value
                onValueBegin(name);

                // create list with count element
                onListBegin(count);
                List list {*this};
                for (std::uint64_t i = 0; i < count; i++) {
                    func(list, i); // describes nested object creation
                }

                // closes list creation
                onListEnd();
                // closes value creation
                onValueEnd();
            }

        protected:

            //===--------------------------------------------------------------------===//
            // logic for writing complex types
            //===--------------------------------------------------------------------===//

            template <class K, class V>
            void write(const std::pair<K, V> &pair) {
                onObjectBegin();
                write("key", pair.first);
                write("value", pair.second);
                onObjectEnd();
            }

            template <class T>
            void write(const std::vector<T> &vec) {
                onListBegin(vec.size());
                for (auto &item : vec) {
                    write(item);
                }
                onListEnd();
            }

        protected:

            //===--------------------------------------------------------------------===//
            // logic of recording basic types
            //===--------------------------------------------------------------------===//

            virtual void write(std::uint8_t value) = 0;
            virtual void write(std::int8_t value) = 0;
            virtual void write(std::uint16_t value) = 0;
            virtual void write(std::int16_t value) = 0;
            virtual void write(std::uint32_t value) = 0;
            virtual void write(std::int32_t value) = 0;
            virtual void write(std::uint64_t value) = 0;
            virtual void write(std::int64_t value) = 0;
            virtual void write(float value) = 0;
            virtual void write(double value) = 0;
            virtual void write(const std::string& value) = 0;
            virtual void write(const char *str) = 0;
            virtual void write(bool value) = 0;

        protected:

            //===--------------------------------------------------------------------===//
            // creates a value with name that can be adjusted until there is a close end
            //===--------------------------------------------------------------------===//

            virtual void onValueBegin(const char *name) = 0;
            virtual void onValueEnd() = 0;

        protected:

            //===--------------------------------------------------------------------===//
            // creates a list with count list (init) that can be adjusted until there is a close end
            //===--------------------------------------------------------------------===//

            virtual void onListBegin(std::uint64_t count) = 0;
            virtual void onListEnd() = 0;

        protected:

            //===--------------------------------------------------------------------===//
            // creates a object that can be adjusted until there is a close end
            //===--------------------------------------------------------------------===//

            virtual void onObjectBegin() = 0;
            virtual void onObjectEnd() = 0;

        };

        /**
          * @brief Wrapper function to write value
          *
          * @tparam T Type of the value
          *
          * @param[in] serializer Reference to the serializer object
          * @param[in] keyName Field name
          * @param[in] value Value to serialize
          */
        template <typename T>
        void serializeValue(
            Serializer& serializer,
            const char* keyName,
            const T& value)
        {
            serializer.writeValue(keyName, value);
        }

        /**
          * @brief Function to write value if it is not supplied with the default value of the type
          *
          * @tparam T Type of the value
          *
          * @param[in] serializer Reference to the serializer object
          * @param[in] keyName Field name
          * @param[in] value Value to serialize
          */
        template <typename T>
        void serializeInitValue(
            Serializer& serializer,
            const char* keyName,
            const T& value)
        {
            serializer.writeInitValue(keyName, value);
        }

        /**
          * @brief Function wrapper for serialize a named object
          *
          * the function creates nested elements via the serializer object
          *
          * @note must have the signature:
          * @code{.cpp}
          * void(serialization::Serializer::Serializer& ser)
          * @endcode
          *
          * @param[in] serializer Reference to the serializer object
          * @param[in] keyName Field name
          * @param[in] func Function that writes object content
          */
        template <typename FUNC>
        void serializeObj(
            Serializer& serializer,
            const char* keyName,
            FUNC func)
        {
            serializer.writeObject(keyName, func);
        }

        /**
          * @brief Function wrapper for serialize a named list
          *
          * function creates nested elements via list object
          *
          * @note must have the signature:
          * @code{.cpp}
          * void(serialization::Serializer::List& list, std::uint64_t index)
          * @endcode
          *
          * @param[in] serializer Reference to the serializer object
          * @param[in] keyName Field name
          * @param[in] count Number of elements
          * @param[in] func Function that writes list elements
          */
        template <typename FUNC>
        void serializeList(
            Serializer& serializer,
            const char* keyName,
            std::uint64_t count,
            FUNC func)
        {
            serializer.writeList(keyName, count, func);
        }


    } // namespace serialization

} // namespace babrat