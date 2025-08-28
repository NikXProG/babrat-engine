#ifndef BABRAT_CHAR_H
#define BABRAT_CHAR_H

#include "../../common/column.h"
#include "column_type.h"

class Char : public ColumnType<std::string> {

    size_t fixed_length;

public:

    explicit Char(const std::string& val, size_t length = 1)
            : ColumnType<std::string>(val), fixed_length(length)
            {

        if (val.size() > fixed_length) {
            throw std::invalid_argument("String is longer than fixed length");
        }

        this->getValue().resize(fixed_length, ' ');

    }

public:

    size_t inline getLength() const { return fixed_length; }

};

#endif //BABRAT_CHAR_H
