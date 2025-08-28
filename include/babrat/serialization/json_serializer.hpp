#pragma once

#include <babrat/serialization/serializer.hpp>

namespace babrat {

    namespace serialization {

        class JsonSerializer : public Serializer {

            private:

                std::string current_name;

            private:

                std::vector<json*> stack;

            private:

                bool m_skipIfEmpty;

            private:

                inline json* current() {
                    return stack.back();
                };

            public:

            explicit JsonSerializer(json& commonJson, bool skipIfEmpty = false)
                : m_skipIfEmpty(skipIfEmpty) {
                commonJson = json::object();
                stack.push_back(&commonJson);
            }

            public:

                void write(std::uint8_t value) final {
                    pushValue(value);
                }

                void write(std::int8_t value) final {
                    pushValue(value);
                }

                void write(std::uint16_t value)  final  {
                    pushValue(value);
                }

                void write(std::int16_t value) final  {
                    pushValue(value);
                }

                void write(std::uint32_t value) final {
                    pushValue(value);
                }

                void write(std::int32_t value) final {
                    pushValue(value);
                }

                void write(std::uint64_t value) final {
                    pushValue(value);
                }

                void write(std::int64_t value) final {
                    pushValue(value);
                }

                void write(float value) final {
                    pushValue(value);
                }

                void write(double value) final {
                    pushValue(value);
                }

                void write(const std::string& value) final
                {
                    if (m_skipIfEmpty && value.empty()) {
                        return;
                    }
                    pushValue(value);
                }

                void write(const char* value) final
                {
                    if (m_skipIfEmpty && strlen(value) == 0) {
                        return;
                    }
                    pushValue(value);
                }

                void write(bool value) final {
                    pushValue(value);
                }

            private:

                void pushValue(const json& jsonValue)
                {

                    auto cur = current();
                    if (cur->is_array()) {
                        cur->push_back(jsonValue);
                    } else if (cur->is_object()) {
                        (*cur)[current_name] = jsonValue;
                    } else {
                        throw std::runtime_error("Cannot add value to non-array/object json value");
                    }


                }

            public:

                void onValueBegin(const char *name) final {
                    current_name = name;
                }

                void onValueEnd() final {}

            public:

                void onListBegin(std::uint64_t count) final {
                    auto new_array = json::array();
                    if (!(count == 0 && m_skipIfEmpty)) {
                        pushValue(new_array);
                    }
                    stack.push_back(&current()->back());
                }

                void onListEnd() final {
                    stack.pop_back();
                }

            public:

                void onObjectBegin() final {
                    auto new_object = json::object();
                    pushValue(new_object);
                    stack.push_back(&current()->back());
                }

                void onObjectEnd() final {
                    auto cur = current();
                    stack.pop_back();
                    if (cur->empty() && m_skipIfEmpty && !stack.empty()) {
                        // remove empty object from parent
                        auto parent = current();
                        if (parent->is_array()) {
                            for (auto it = parent->begin(); it != parent->end(); ++it) {
                                if (&(*it) == cur) {
                                    parent->erase(it);
                                    break;
                                }
                            }
                        } else if (parent->is_object()) {
                            for (auto it = parent->begin(); it != parent->end(); ++it) {
                                if (&it.value() == cur) {
                                    parent->erase(it.key());
                                    break;
                                }
                            }
                        }
                    }
                }
            };

    }  // namespace serialization

} // namespace babrat