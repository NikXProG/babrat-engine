
#ifndef BABRAT_CONVERTABLE_H
#define BABRAT_CONVERTABLE_H
#include <iostream>

class Encodable {

public:

    virtual std::vector<uint8_t> encode() const = 0;
    virtual void decode(const std::vector<uint8_t>& encoded) = 0;

public:

    virtual ~Encodable() noexcept = default;

};

#endif //BABRAT_CONVERTABLE_H
