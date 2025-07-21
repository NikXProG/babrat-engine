#ifndef BABRAT_CONDITION_MODEL_H
#define BABRAT_CONDITION_MODEL_H
#include <variant>
#include <optional>
#include <vector>


struct ConditionModel {

    std::optional<std::string> field;  // Поле может отсутствовать для операторов AND/OR
    std::optional<std::string> operator_;  // Оператор (=, >, <, LIKE и т.д.)
    std::optional<std::string> value;  // Значение для сравнения

    std::vector<ConditionModel> conditions;

    ConditionModel(std::string field, std::string op, std::string val)
            : field(std::move(field)), operator_(std::move(op)), value(std::move(val)) {}

    ConditionModel(std::string op, std::vector<ConditionModel> conds)
            : operator_(std::move(op)), conditions(std::move(conds)) {}
};

#endif //BABRAT_CONDITION_MODEL_H
