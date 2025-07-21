#ifndef BABRAT_INT32_H
#define BABRAT_INT32_H
#include "../../common/column.h"
#include "column_type.h"

class Int : public ColumnType<int> {

public:

    explicit Int(int val) : ColumnType<int>(val) {}

};


#endif //BABRAT_INT32_H
