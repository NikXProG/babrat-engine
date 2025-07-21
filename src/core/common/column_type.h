#ifndef BABRAT_COLUMN_TYPE_H
#define BABRAT_COLUMN_TYPE_H
#include "column_visitor.h"
#include "column.h"

#include "serializable.h"
#include "magic_enum/magic_enum.hpp"
#include "column.h"
#include "babrat_id.h"
#include <charconv>
#include <cxxabi.h>

#include <iostream>
#define ENUM_TO_STRING(NAME) magic_enum::enum_name(NAME)

namespace babrat {

    template <typename NumType>
    NumType safe_numeric_parse(const std::string& value) {
        NumType result;
        auto [ptr, ec] = std::from_chars(value.data(), value.data() + value.size(), result);
        if (ec != std::errc() || ptr != value.data() + value.size()) {
            throw std::runtime_error("Invalid numeric format");
        }
        return result;
    }


    template <typename T>
    constexpr std::string_view type_name() {

        if constexpr (std::is_same_v<T, bool>) return "bool";
        else if constexpr (std::is_same_v<T, int>) return "int";
        else if constexpr (std::is_same_v<T, unsigned int>) return "unsigned int";
        else if constexpr (std::is_same_v<T, std::string>) return "string";
        else {
            return []() {
                int status = 0;
                char* demangled = abi::__cxa_demangle(typeid(T).name(), 0, 0, &status);
                std::string result = (status == 0) ? demangled : typeid(T).name();
                free(demangled);
                return result;
            }();
        }
    }


    template<typename T>
    class ColumnType : public Column {

    protected:

        IdxType m_columnIndex;

        BabratType m_columnType;

        std::optional<T> m_columnValue;

    public:

        explicit ColumnType(
                std::optional<T> columnValue,
                IdxType columnIndex,
                BabratType columnType) :
                m_columnValue(std::move(columnValue)),
                m_columnIndex(columnIndex),
                m_columnType(columnType) {}

    public:


        bool equals(const Column& other) const override {

          if (m_columnType != other.getType() ||
            m_columnIndex != other.getIndex()) {
              return false;
          }

          const auto* otherColumn =
                  dynamic_cast<const ColumnType<T>*>(&other);

          if (!otherColumn){
              return false;
          }

          return valueEquals(*otherColumn);

        }

        bool valueEquals(const ColumnType<T>& other) const {
            return m_columnValue == other.m_columnValue;
        }

        bool isNull() const override {
            return !m_columnValue.has_value();
        }

    public:

        std::optional<T> getValue() const {
            return m_columnValue;
        }

        BabratType getType() const override {
            return m_columnType;
        }

        IdxType getIndex() const override {
            return m_columnIndex;
        }

    public:

        static std::unique_ptr<ColumnType> create(
                std::optional<T> value,
                IdxType index,
                BabratType type) {
            return std::make_unique<ColumnType>(
                    value,
                    index,
                    type);
        }

    public:


        size_t serialize(std::ostream& stream) const override {
            size_t written = babrat::write(stream, m_columnValue.has_value());
            if (m_columnValue.has_value()) {
                written += babrat::write(stream, m_columnValue.value());
            }
            return written;
        }

        std::size_t deserialize(std::istream& stream) override {

            bool has_value = false;
            size_t written = babrat::read(stream, has_value);

            if (has_value) {
                T val;
                written += babrat::read(stream, val);
                m_columnValue = std::move(val);

            } else {
                m_columnValue = std::nullopt;
            }

            return written;
        }

        [[nodiscard]] size_t serializedSize() const noexcept override {
            if (!m_columnValue.has_value()) {
                return sizeof(bool);
            }
            return sizeof(bool) + sizeof(T);
        }

    public:


        void accept(ColumnVisitor& visitor) const override {
            if (m_columnValue.has_value()) {
                visitor.visit(m_columnValue.value());
            }
        }


    };

    using IntColumn = ColumnType<int>;

    using StringColumn = ColumnType<std::string>;

    using UIntColumn = ColumnType<unsigned int>;

    template<typename T>
    using Optional = std::optional<T>;

    template<typename T>
    class ColumnFactory {
        using Creator = std::function<std::unique_ptr<Column>(std::optional<T>, IdxType)>;

        static inline std::unordered_map<BabratType, Creator> creators_ = {
                {BabratType::BOOL, [](std::optional<T> value, IdxType index) {
                    return std::make_unique<ColumnType<T>>(value, index, BabratType::BOOL);
                }},
                {BabratType::INTEGER, [](std::optional<T> value, IdxType index) {
                    return std::make_unique<ColumnType<T>>(value, index, BabratType::INTEGER);
                }},
                // Добавьте другие типы по аналогии
        };

    public:
        static std::unique_ptr<Column> create(std::optional<T> value, IdxType index) {
            auto type = deduceType<T>();
            if (auto it = creators_.find(type); it != creators_.end()) {
                return it->second(value, index);
            }
            throw std::runtime_error("Unsupported type");
        }

    private:
        static BabratType deduceType() {
            if constexpr (std::is_same_v<T, bool>) return BabratType::BOOL;
            if constexpr (std::is_same_v<T, int>) return BabratType::INTEGER;
            if constexpr (std::is_same_v<T, unsigned int>) return BabratType::UINT32;
            if constexpr (std::is_same_v<T, std::string>) return BabratType::TEXT;
            throw std::runtime_error("Cannot deduce type for T");
        }
    };


    /*    void insert(const std::string& value) override {

        try {

            if constexpr (std::is_same_v<T, bool>) {
                std::cout << value_.has_value() << std::endl;
                value_ = (value == "true" || value == "1");
            } else if constexpr (std::is_same_v<T, int>) {
                value_ = safe_numeric_parse<int>(value);
            } else if constexpr (std::is_same_v<T, unsigned int>) {
                value_ = safe_numeric_parse<unsigned int>(value);
            } else if constexpr (std::is_same_v<T, std::string>) {
                value_ = value;
            }else{
                throw std::runtime_error("Type not implemented");
            }

        }catch (const std::exception& e) {
            throw std::runtime_error(
                    std::string("Failed to set value '") + value +
                    "' for type '" + type_name<T>().data() + "': " + e.what()
            );
        }

    }*/

  /*  class ColumnFactory {

        using Creator = std::function<Column*()>;

        static inline std::unordered_map<BabratType, Creator> creators_ = {
                {BabratType::BOOL, []{
                    return new ColumnType<bool>(); }},
                {BabratType::TEXT, []{
                    return new ColumnType<std::string>(); }},
                {BabratType::INTEGER, []{
                    return new ColumnType<int>(); }},
                {BabratType::UINT32, []{
                    return new ColumnType<unsigned int>(); }}
        };

    public:

        static Column* create(BabratType type) {

            if (auto it = creators_.find(type); it != creators_.end()) {
                return it->second();
            }

            throw std::runtime_error("Unsupported type");
        }



    };*/
/*

    template <>
    struct stream_reader<Column*> {
        static size_t read(std::istream& is, Column*& ptr) {

            BabratType type;
            size_t read = babrat::read(is, type);

            ptr = ColumnFactory::create(type);

            read += ptr->deserialize(is);
            return read;
        }
    };

*/


    /*  bool operation_by(std::string op, Column* other) const override {

          if (!other || getType() != other->getType()) {
              return false;
          }

          const auto* otherColumn = dynamic_cast<const ColumnType<T>*>(other);
          if (!otherColumn) {
              return false;
          }

          if (!value_.has_value() || !otherColumn->value_.has_value()) {
              if (op == "=") {
                  return !value_.has_value() && !otherColumn->value_.has_value();
              } else if (op == "<>") {
                  return value_.has_value() != otherColumn->value_.has_value();
              }
              return false;
          }

          const T& this_val = *value_;
          const T& other_val = *otherColumn->value_;

          if (op == ">") {
              return this_val > other_val;
          } else if (op == "<") {
              return this_val < other_val;
          } else if (op == "=") {
              return this_val == other_val;
          } else if (op == "<>") {
              return this_val != other_val;
          } else if (op == ">=") {
              return this_val >= other_val;
          } else if (op == "<=") {
              return this_val <= other_val;
          }

          throw std::runtime_error("Unsupported operator: " + op);
      }*/



    template <>
    struct stream_writer<Column*> {
        static size_t write(std::ostream& os, const Column* ptr) {
            size_t written = babrat::write(os, ptr->getType());
            written += ptr->serialize(os);
            return written;
        }
    };

}


#endif //BABRAT_COLUMN_TYPE_H

