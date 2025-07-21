

#ifndef BABRAT_DB_TYPES_H
#define BABRAT_DB_TYPES_H

#include <iostream>

class DbTypeBase {

public:

    virtual ~DbTypeBase() = default;

public:

    virtual void print() const = 0;

public:

    virtual DbTypeBase* clone() const = 0;

public:

    virtual bool equals(const DbTypeBase* other) const = 0;
    virtual bool isNull() const = 0;
    virtual void setNull() = 0;

};



template<typename T>
class NullableType : public DbTypeBase {
protected:

    std::optional<T> value;

public:
    NullableType() = default;

    explicit NullableType(const T& val) : value(val) {}

    bool isNull() const override {
        return !value;
    }

    void setNull() override {
        value.reset();
    }

    [[nodiscard]] const T& getValue() const {
        if (isNull()) throw std::runtime_error("Value is null");
        return value.value();
    }

    void setValue(const T& val) { value = val; }

};

class DbInt : public NullableType<int> {
public:
    using NullableType<int>::NullableType;

    void print() const override {
        if (isNull()) std::cout << "DbInt: NULL" << std::endl;
        else std::cout << "DbInt: " << getValue() << std::endl;
    }

    DbTypeBase* clone() const override {
        auto copy = new DbInt();
        if (!isNull()) copy->setValue(getValue());
        return copy;
    }

    bool equals(const DbTypeBase* other) const override {
        if (auto o = dynamic_cast<const DbInt*>(other)) {
            if (isNull() && o->isNull()) return true;
            if (isNull() || o->isNull()) return false;
            return getValue() == o->getValue();
        }
        return false;
    }
};

class DbUint : public NullableType<unsigned int> {

public:

    using NullableType<unsigned int>::NullableType;

    void print() const override {
        if (isNull()) std::cout << "DbUint: NULL" << std::endl;
        else std::cout << "DbUint: " << getValue() << std::endl;
    }


    DbTypeBase* clone() const override {
        auto copy = new DbUint();
        if (!isNull()) copy->setValue(getValue());
        return copy;
    }


    bool equals(const DbTypeBase* other) const override {
        if (auto o = dynamic_cast<const DbUint*>(other)) {
            if (isNull() && o->isNull()) return true;
            if (isNull() || o->isNull()) return false;
            return getValue() == o->getValue();
        }
        return false;
    }
};

class DbString : public NullableType<std::string> {
public:
    using NullableType<std::string>::NullableType;

    void print() const override {
        if (isNull()) std::cout << "DbString: NULL" << std::endl;
        else std::cout << "DbString: \"" << getValue() << "\"" << std::endl;
    }


    DbTypeBase* clone() const override {
        auto copy = new DbString();
        if (!isNull()) copy->setValue(getValue());
        return copy;
    }

    bool equals(const DbTypeBase* other) const override {
        if (auto o = dynamic_cast<const DbString*>(other)) {
            if (isNull() && o->isNull()) return true;
            if (isNull() || o->isNull()) return false;
            return getValue() == o->getValue();
        }
        return false;
    }
};

class DbFixedString : public DbString {
    size_t fixed_length;
public:
    DbFixedString(size_t length) : fixed_length(length) {}
    DbFixedString(const std::string& val, size_t length)
            : DbString(val.substr(0, length)), fixed_length(length) {}

    void print() const override {
        if (isNull()) std::cout << "DbFixedString(" << fixed_length << "): NULL" << std::endl;
        else std::cout << "DbFixedString(" << fixed_length << "): \"" << getValue() << "\"" << std::endl;
    }

    DbTypeBase* clone() const override {
        auto copy = new DbFixedString(fixed_length);
        if (!isNull()) copy->setValue(getValue());
        return copy;
    }

    bool equals(const DbTypeBase* other) const override {
        if (auto o = dynamic_cast<const DbFixedString*>(other)) {
            if (fixed_length != o->fixed_length) return false;
            return DbString::equals(o);
        }
        return false;
    }

};


class DbCompositer : public DbTypeBase {

    std::vector<DbTypeBase*> elements;

public:

    DbCompositer() = default;


    void add(DbTypeBase* element) {
        elements.push_back(element);
    }

    bool isNull() const override {
        return elements.empty();
    }

    void setNull() override {
        clear();
    }

    DbTypeBase* get(size_t i) const {
        if (i >= elements.size()) {
            throw std::out_of_range("Index " + std::to_string(i) +
                                    " is out of range [0, " +
                                    std::to_string(elements.size()) + ")");
        }
        return elements[i];
    }

    void print() const override {
        if (isNull()) {
            std::cout << "DbCompositer: NULL" << std::endl;
            return;
        }

        std::cout << "DbCompositer [" << elements.size() << " elements]:" << std::endl;
        for (const auto& elem : elements) {
            std::cout << "  ";
            elem->print();
        }
    }

    DbTypeBase* clone() const override {
        auto new_composite = new DbCompositer();
        for (const auto elem : elements) {
            new_composite->add(elem->clone());
        }
        return new_composite;
    }

    bool equals(const DbTypeBase* other) const override {
        if (auto o = dynamic_cast<const DbCompositer*>(other)) {
            if (isNull() && o->isNull()) return true;
            if (isNull() || o->isNull()) return false;
            if (elements.size() != o->elements.size()) return false;

            for (size_t i = 0; i < elements.size(); ++i) {
                if (!elements[i]->equals(o->elements[i])) {
                    return false;
                }
            }

            return true;
        }

        return false;
    }

public:

    ~DbCompositer()
    {
        clear();
    }

    DbCompositer(
            DbCompositer const &other)
    {
        copy_from(other);
    }

    DbCompositer &operator=(
            DbCompositer const &other)
    {
        if (this != &other)
        {
            clear();
            copy_from(other);
        }

        return *this;
    }

    DbCompositer(
            DbCompositer &&other) noexcept
    {
        move_from(std::move(other));
    }

    DbCompositer &operator=(
            DbCompositer &&other) noexcept
    {

        if (this != &other)
        {
            clear();
            move_from(std::move(other));
        }

        return *this;
    }

private:

    void clear()
    {
        for (auto elem : elements) {
            delete elem;
        }
        elements.clear();
    }

    void copy_from(const DbCompositer& other) {
        elements.reserve(other.elements.size());
        for (const auto elem : other.elements) {
            elements.push_back(elem->clone());
        }
    }

    void move_from(DbCompositer&& other) noexcept {
        elements = std::move(other.elements);
        other.elements.clear();
    }



};


#endif //BABRAT_DB_TYPES_H
