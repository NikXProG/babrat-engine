#ifndef NOT_IMPLEMENTED_H
#define NOT_IMPLEMENTED_H

#include <stdexcept>

class not_implemented final:
    public std::logic_error
{

public:

    explicit not_implemented(
        std::string const &method_name,
        std::string const &message);

};


#endif //NOT_IMPLEMENTED_H
