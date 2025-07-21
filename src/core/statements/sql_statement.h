#ifndef BABRAT_SQL_STATEMENT_H
#define BABRAT_SQL_STATEMENT_H
#include <string>

//base sql model
class SqlStatement {

public:

    virtual ~SqlStatement() = default;

public:

    virtual std::string toString() const = 0;

};


#endif //BABRAT_SQL_STATEMENT_H
