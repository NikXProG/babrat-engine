
#ifndef BABRAT_TYPES_H
#define BABRAT_TYPES_H

enum class SqlBaseType {
    BOOLEAN,
    INT,
    BIGINT,
    DOUBLE,
    VARCHAR
};


#include <memory>
#include <string>
#include <unordered_map>

class SqlType {

private:

     SqlBaseType m_baseType;

public:

    explicit SqlType(SqlBaseType baseType) : m_baseType(baseType) {}

public:

    SqlBaseType baseType() const {
        return m_baseType;
    }

    virtual std::string toString() const = 0;

};

class SqlPrimitiveType : SqlType {

    using base_map_type = std::unordered_map<SqlBaseType, SqlPrimitiveType>;

    SqlBaseType baseType;

    static inline const std::unordered_map<SqlBaseType, std::shared_ptr<SqlPrimitiveType>> TYPES = {
        {SqlBaseType::BOOLEAN, std::make_shared<SqlPrimitiveType>(SqlBaseType::BOOLEAN)},
        {SqlBaseType::INT, std::make_shared<SqlPrimitiveType>(SqlBaseType::INT)},
        {SqlBaseType::BIGINT, std::make_shared<SqlPrimitiveType>(SqlBaseType::BIGINT)},
        {SqlBaseType::DOUBLE, std::make_shared<SqlPrimitiveType>(SqlBaseType::DOUBLE)},
        {SqlBaseType::VARCHAR, std::make_shared<SqlPrimitiveType>(SqlBaseType::VARCHAR)}
    };

public:

    SqlPrimitiveType(const SqlBaseType baseType) :
            SqlType(baseType){}

public:

    SqlPrimitiveType(const SqlPrimitiveType&) = delete;
    SqlPrimitiveType& operator=(const SqlPrimitiveType&) = delete;

    static const std::shared_ptr<SqlPrimitiveType>& of(SqlBaseType type) {
        return TYPES.at(type);
    }

public:

    std::string toString() const override {
        switch(baseType) {
            case SqlBaseType::INT: return "INT";
            case SqlBaseType::VARCHAR: return "VARCHAR";
            case SqlBaseType::BIGINT: return "BIGINT";
            case SqlBaseType::DOUBLE: return "DOUBLE";
            case SqlBaseType::BOOLEAN: return "BOOLEAN";
            default: return "UNKNOWN";
        }
    }

};

class SqlPrimitiveTypes {

    static const std::shared_ptr<SqlPrimitiveType>& BOOLEAN() {
        return SqlPrimitiveType::of(SqlBaseType::BOOLEAN);
    }


};
#endif //BABRAT_TYPES_H
