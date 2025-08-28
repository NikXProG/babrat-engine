#ifndef BABRAT_ATTRIBUTE_CTX_H
#define BABRAT_ATTRIBUTE_CTX_H

#include <optional>
#include <vector>
#include <common/condition_model.h>

class AttributeContext {

public:

    std::map<std::string, std::vector<Column*>>& value_;

    std::vector<std::vector<Column*>>& vectors_;

    std::unordered_map<std::string,size_t> columns_index_;

    std::optional<ConditionModel> where_condition;


public:

    explicit AttributeContext(std::map<std::string, std::vector<Column*>>& value,
                              std::vector<std::vector<Column*>>& vectors)
            : value_(value), vectors_(vectors) {}

  /*  std::optional<std::string> order_by;
    std::optional<bool> ascending;
    std::optional<size_t> limit;
    std::optional<size_t> offset;

*/

public:

    void setResult( std::vector<std::vector<Column*>> vectors){
        vectors_ = vectors;
    }


};
#endif //BABRAT_ATTRIBUTE_CTX_H
