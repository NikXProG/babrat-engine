#pragma once

#ifndef BABRAT_COLUMN_H
#define BABRAT_COLUMN_H

#include "serializable.h"
#include "column_visitor.h"
#include "babrat_id.h"

namespace babrat{

    enum class BabratType : uint8_t {
        INT2,
        INTEGER,
        INT8,
        UINT32,
        UINT8,
        TEXT,
        VARCHAR,
        FLOAT,
        BOOL,
        SERIAL,
        NUMERIC,
        DECIMAL,
    };

    class Column : public Serializable  {

    public:

        ~Column() noexcept override = default;

    public:

        virtual BabratType getType() const = 0;

        virtual IdxType getIndex() const = 0;

    public:

        virtual bool equals(const Column& other) const = 0;

        virtual bool isNull() const = 0;

    public:

        virtual void accept(ColumnVisitor& visitor) const = 0;


    };


}

/*
public:

virtual void insert(const std::string& value) = 0;*/
/*        [[nodiscard]] virtual bool operation_by(std::string op,
                                                Column* other) const = 0;*/




#endif //BABRAT_COLUMN_H
