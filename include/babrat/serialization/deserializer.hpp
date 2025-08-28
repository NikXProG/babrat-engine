#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <type_traits>

using json = nlohmann::ordered_json;

namespace babrat {

    namespace serialization {

        class Deserializer {

        public:

            virtual ~Deserializer() noexcept = default;

            template <typename T>
            inline void ReadProperty(const char *name, T &ret) {
                onPropertyBegin(name);
                ret = read<T>();
                onPropertyEnd();
            }

            // Read and return a value
            template <typename T>
            inline T ReadProperty(const char *name) {
                onPropertyBegin(name);
                auto ret = read<T>();
                onPropertyEnd();
                return ret;
            }

        protected:

            virtual std::int8_t readSignedInt8() = 0;
            virtual std::uint8_t readUnsignedInt8() = 0;
            virtual std::int16_t readSignedInt16() = 0;
            virtual std::uint16_t readUnsignedInt16() = 0;
            virtual std::int32_t readSignedInt32() = 0;
            virtual std::uint32_t readUnsignedInt32() = 0;
            virtual std::int64_t readSignedInt64() = 0;
            virtual std::uint64_t readUnsignedInt64() = 0;
            virtual float readFloat() = 0;
            virtual double readDouble() = 0;
            virtual std::string readString() = 0;
            virtual bool readBool() = 0;

        protected:

            virtual void onPropertyBegin(const char *name [[maybe_unused]]) = 0;
            virtual void onPropertyEnd() = 0;

        protected:

            virtual void onObjectBegin() = 0;
            virtual void onObjectEnd() = 0;

        protected:

            virtual std::uint64_t onListBegin() = 0;
            virtual void onListEnd() = 0;

        private:

            // Primitive types

	        // Deserialize a bool
	        template <typename T = void>
	        inline typename std::enable_if<std::is_same<T, bool>::value, T>::type read() {
		        return readBool();
	        }

	        // Deserialize a int8_t
	        template <typename T = void>
	        inline typename std::enable_if<std::is_same<T, int8_t>::value, T>::type read() {
		        return readSignedInt8();
	        }

	        // Deserialize a uint8_t
	        template <typename T = void>
	        inline typename std::enable_if<std::is_same<T, uint8_t>::value, T>::type read() {
		        return readUnsignedInt8();
	        }

	        // Deserialize a int16_t
	        template <typename T = void>
	        inline typename std::enable_if<std::is_same<T, int16_t>::value, T>::type read() {
		        return readSignedInt16();
	        }

	        // Deserialize a uint16_t
	        template <typename T = void>
	        inline typename std::enable_if<std::is_same<T, uint16_t>::value, T>::type read() {
		        return readUnsignedInt16();
	        }

	        // Deserialize a int32_t
	        template <typename T = void>
	        inline typename std::enable_if<std::is_same<T, int32_t>::value, T>::type read() {
		        return readSignedInt32();
	        }

	        // Deserialize a uint32_t
	        template <typename T = void>
	        inline typename std::enable_if<std::is_same<T, uint32_t>::value, T>::type read() {
		        return readUnsignedInt32();
	        }

	        // Deserialize a int64_t
	        template <typename T = void>
	        inline typename std::enable_if<std::is_same<T, int64_t>::value, T>::type read() {
		        return readSignedInt64();
	        }

	        // Deserialize a uint64_t
	        template <typename T = void>
	        inline typename std::enable_if<std::is_same<T, uint64_t>::value, T>::type read() {
		        return readUnsignedInt64();
	        }

	        // Deserialize a float
	        template <typename T = void>
	        inline typename std::enable_if<std::is_same<T, float>::value, T>::type read() {
		        return readFloat();
	        }

	        // Deserialize a double
	        template <typename T = void>
	        inline typename std::enable_if<std::is_same<T, double>::value, T>::type read() {
		        return readDouble();
	        }

	        // Deserialize a string
	        template <typename T = void>
	        inline typename std::enable_if<std::is_same<T, std::string>::value, T>::type read() {
		        return readString();
	        }

        };

        class JsonDeserializer : public Deserializer {

        private:

            struct JsonStackElement {
                const json* val;
                size_t index = 0;
                explicit JsonStackElement(const json* val) : val(val) {}
            };

        private:

            json m_commonJson;
            std::string current_tag;
            std::vector<JsonStackElement> stack;

        public:

            explicit JsonDeserializer(json&& commonJson) :
                m_commonJson(std::move(commonJson)) {
                stack.emplace_back(&m_commonJson);
            }

        private:

            inline JsonStackElement &Current() {
                return stack.back();
            };

            inline void Push(json& json) {
                stack.emplace_back(&json);
            }

            inline void Pop() {
                stack.pop_back();
            }

        public:

            ~JsonDeserializer() noexcept override = default;

        private:

            const json& next() {

                auto& cur = Current();


                if (cur.val->is_object()) {

                    auto it = cur.val->find(current_tag);

                    if (it == cur.val->end()) {
                        throw std::runtime_error("Property '" + std::string(current_tag) + "' not found");
                    }

                    return it.value();
                }

                if (cur.val->is_array()) {

                    if (cur.index >= cur.val->size()) {
                        throw std::runtime_error("Array index out of bounds");
                    }

                    return (*cur.val)[cur.index++];
                }

                throw std::runtime_error("GetNextValue: not object/array");

            }

        private:

            template<typename T>T readInt(const char* nameType) {
                const auto& val = next();

                if (!val.is_number_integer()) {
                    throw OutOfRangeException("Expected integer for {0}", nameType);
                }

                try {
                    return val.get<T>();
                } catch (const json::exception& e) {
                    throw babrat::UnknownTypeException("Value out of range for {0}: {1}",
                                                     nameType, e.what());
                }
            }

        protected:

            bool readBool() final
            {
                auto val = next();
                if (!val.is_boolean()) {
                    throw std::runtime_error(
                    std::format("Property '{}' expected bool",
                        current_tag));
                }
                return val.get<bool>();
            }

            double readDouble() final
            {
                auto val = next();
                if (!val.is_number()) {
                    throw std::runtime_error(
                    std::format("Property '{}' expected double", current_tag));
                }
                return val.get<double>();

            }

            float readFloat() final
            {

                const auto& val = next();

                if (!val.is_number()) {
                    throw std::runtime_error(
                    std::format("Property '{}' expected float", current_tag));
                }
                return val.get<float>();
            }

            std::string readString() final
            {
                auto val = next();
                if (!val.is_string()) {
                    throw std::runtime_error(
                    std::format("Property '{}' expected string", current_tag));
                }
                return val.get<std::string>();
            }

            std::int8_t readSignedInt8() final
            {
                return readInt<std::int8_t>("int8_t");
            }

            std::uint8_t readUnsignedInt8() final
            {
                return readInt<std::uint8_t>("uint8_t");
            }

            std::int16_t readSignedInt16() final
            {
                return readInt<std::int16_t>("int16_t");
            }

            std::uint16_t readUnsignedInt16() final
            {
                return readInt<std::uint16_t>("uint16_t");
            }

            std::int32_t readSignedInt32() final
            {
                return readInt<std::int32_t>("int32_t");
            }

            std::uint32_t readUnsignedInt32() final
            {
                return readInt<std::uint32_t>("uint32_t");
            }

            std::int64_t readSignedInt64() final
            {
                return readInt<std::int64_t>("int64_t");
            }

            std::uint64_t readUnsignedInt64() final
            {
                return readInt<std::uint64_t>("uint64_t");
            }

        protected:

            void onPropertyBegin(const char* tag) final {
                current_tag = tag;
            }

            void onPropertyEnd() final {}

        protected:

            std::uint64_t onListBegin() final {
                auto val = next();
                if (!val.is_array()) {
                    throw std::runtime_error("array expected");
                }
                stack.emplace_back(&val);
                return val.size();
            }

            void onListEnd() final {
                stack.pop_back();
            }


        protected:

            void onObjectBegin() final {
                auto val = next();
                if (!val.is_object()) {
                    throw std::runtime_error("dsdas");
                }
                stack.emplace_back(&val);
            }

            void onObjectEnd() final {
                stack.pop_back();
            }

        };

        class BinaryDeserializer : public Deserializer {

            std::istream& istream;

        public:

            explicit BinaryDeserializer(std::istream &istream) : istream(istream) {

            }

        private:

            template <typename T>
            T readData() {

                T value;
                size_t bytes_read = stream_reader<T>::read(istream, value);

                if (bytes_read == 0 || istream.fail()) {
                    throw std::runtime_error("Failed to read value from stream");
                }

                return value;
            }

        protected:

            bool readBool() final
            {
                return readData<bool>();
            }

            double readDouble() final
            {
                return readData<double>();
            }

            float readFloat() final
            {
                return readData<float>();
            }

            std::string readString() final
            {
                return readData<std::string>();
            }

            std::int8_t readSignedInt8() final
            {
                return readData<std::int8_t>();
            }

            std::uint8_t readUnsignedInt8() final
            {
                return readData<std::uint8_t>();
            }

            std::int16_t readSignedInt16() final
            {
                return readData<std::int16_t>();
            }

            std::uint16_t readUnsignedInt16() final
            {
                return readData<std::uint16_t>();
            }

            std::int32_t readSignedInt32() final
            {
                return readData<std::int32_t>();
            }

            std::uint32_t readUnsignedInt32() final
            {
                return readData<std::uint32_t>();
            }

            std::int64_t readSignedInt64() final
            {
                return readData<std::int64_t>();
            }

            std::uint64_t readUnsignedInt64() final
            {
                return readData<std::uint64_t>();
            }

        protected:

            void onPropertyBegin(const char*) final {}

            void onPropertyEnd() final {}

        protected:

            std::uint64_t onListBegin() final
            {
                return readUnsignedInt64();
            }

            void onListEnd() final {}

        protected:

            void onObjectBegin() final {}

            void onObjectEnd() final {}

        };


    } // namespace serialization

} // namespace babrat

