#ifndef BABRAT_BOOLEAN_Hsz
#define BABRAT_BOOLEAN_H
#include "../../common/column.h"
#include "column_type.h"

class Bool : public ColumnType<bool> {

public:

    explicit Bool(bool val) : ColumnType<bool>(val) {}

};

#endif //BABRAT_BOOLEAN_H
