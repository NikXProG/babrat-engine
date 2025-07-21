#pragma once

#ifndef BABRAT_CONVERT_COLUMN_H
#define BABRAT_CONVERT_COLUMN_H
#include "column_type.h"

namespace babrat {

    namespace convert {

        std::string to_string(BabratType type) {
            switch(type) {
                case BabratType::INTEGER:
                    return "INTEGER";
                case BabratType::VARCHAR:
                    return "VARCHAR";
                case BabratType::TEXT:
                    return "TEXT";
                case BabratType::BOOL:
                    return "BOOL";
                case BabratType::UINT32:
                    return "UINT32";
            }

            throw std::out_of_range("Invalid database type value : casting forbidden type");
        }


        BabratType string_to_type(std::string_view str) {

            if (str == "INTEGER"){
                return BabratType::INTEGER;
            }
            if (str =="VARCHAR"){
                return  BabratType::VARCHAR;
            }
            if (str =="TEXT"){
                return BabratType::TEXT;
            }
            if (str =="BOOL"){
                return BabratType::BOOL;
            }
            if (str =="UINT32"){
                return  BabratType::UINT32;
            }

            throw std::out_of_range("invalid oid string value : casting forbidden type");

        }

/*
        Column* to_column(int value) {
            return new ColumnType<int>(value);
        }

        Column* to_column(unsigned int value) {
            return new ColumnType<unsigned int>(value);
        }

        Column* to_column(bool value) {
            return new ColumnType<bool>(value);
        }

        Column* to_column(const std::string& value) {
            return new ColumnType<std::string>(value);
        }*/


        template<typename T>
        [[nodiscard]] ColumnType<T>* get_column_by(Column* base) {
            if (!base) {
                throw std::invalid_argument("get_column_by: base is nullptr");
            }

            if (auto* result = dynamic_cast<ColumnType<T>*>(base)) {
                return result;
            }

            throw std::runtime_error(
                    "Type mismatch: expected " + std::string(typeid(ColumnType<T>).name()) +
                    ", got " + std::string(typeid(*base).name())
            );

        }


        template<typename T>
        [[nodiscard]] T const& get(ColumnType<T>* column) {
            return column->getValue().value();
        }

        template<typename T>
        [[nodiscard]] T const& get(Column* base) {
            return get(get_column_by<T>(base));
        }


    }


}

#endif //BABRAT_CONVERT_COLUMN_H

