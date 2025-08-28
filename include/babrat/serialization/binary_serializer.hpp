#pragma once

#include <babrat/serialization/serializer.hpp>

namespace babrat {

    namespace serialization {

        class BinarySerializer : public Serializer {

            std::ostream& ostream;

        public:

            explicit BinarySerializer(std::ostream& ostream) : ostream(ostream){

            }

        private:

            template<typename T>
            void pushValue(const T& value)
            {
                babrat::write(ostream, value);
            }

        protected:

            void write(bool value) final {
                pushValue(value);
            }

            void write(uint8_t value) final {
                pushValue(value);
            }

            void write(int8_t value) final {
                pushValue(value);
            }

            void write(uint16_t value) final {
                pushValue(value);
            }

            void write(int16_t value) final {
                pushValue(value);
            }

            void write(uint32_t value) final {
                pushValue(value);
            }

            void write(int32_t value) final {
                pushValue(value);
            }

            void write(uint64_t value) final {
                pushValue(value);
            }

            void write(int64_t value) final {
                pushValue(value);
            }

            void write(const std::string& value) final {
                pushValue(value);
            }

            void write(const char* value) final {
                // wrapping const char *
                pushValue(std::string(value));
            }

            void write(float value) final {
                pushValue(value);
            }

            void write(double value) final {
                pushValue(value);
            }

        protected:

            void onListBegin(std::uint64_t count) final
            {
                pushValue(count);
            }

            void onListEnd() final { }

        protected:

            void onObjectBegin() final {}

            void onObjectEnd() final {}

        protected:

            void onValueEnd() final {}

            void onValueBegin(const char *name [[maybe_unused]]) final {}

        };

    }


}