#ifndef BABRAT_DYNAMIC_STRING_H
#define BABRAT_DYNAMIC_STRING_H

#include "../../common/column.h"
#include "column_type.h"/*
class Varchar : public ColumnType<std::string> {

    size_t max_length{};

public:

    explicit Varchar(const std::string& val) :
    ColumnType<std::string>(val) {

    }

    explicit Varchar(const std::string& val, size_t max_len)
            : ColumnType<std::string>(""), max_length(max_len)
    {
        setValue(val);
    }


    void setValue(std::string val) {
        if (max_length > 0 && val.size() > max_length) {
            // Либо обрезаем строку:
            val.resize(max_length);

            // Либо выбрасываем исключение (раскомментировать если нужно):
            // throw std::invalid_argument("Value too long for this Varchar");
        }
        this->getValue() = std::move(val);
    }



    [[nodiscard]] size_t getStorageSize()  {
        return this->getValue().size() +
               1;
    }


    [[nodiscard]] size_t getMaxLength() const { return max_length; }
    [[nodiscard]] size_t getLength() { return this->getValue().size(); }

};*/




#endif //BABRAT_DYNAMIC_STRING_H
