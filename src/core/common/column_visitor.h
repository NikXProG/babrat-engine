#pragma once

#include <functional>

#ifndef BABRAT_COLUMN_BASE_VISITOR_H
#define BABRAT_COLUMN_BASE_VISITOR_H


class ColumnVisitor {

public:

    virtual ~ColumnVisitor() = default;

public:

    virtual void visit(
            int val) = 0;

    virtual void visit(
            unsigned int val) = 0;

    virtual void visit(
            const std::string& val) = 0;

    virtual void visit(
            const char* val) = 0;

    virtual void visit(
            bool val) = 0;

};


class PrintVisitor : public ColumnVisitor {

    std::ostream& os;

public:

    explicit PrintVisitor(std::ostream& os) : os(os) {}

public:

    void visit(int val) override {
        os << val << " ";
    }

    void visit(unsigned int val) override {
        os << val << " ";
    }

    void visit(const std::string& val) override {
        os << val << " ";
    }

    void visit(const char* val) override {
        os << val << " ";
    }

    void visit(bool val) override {
        os << (val ? "true" : "false") << " ";
    }
};



#endif //BABRAT_COLUMN_BASE_VISITOR_H
