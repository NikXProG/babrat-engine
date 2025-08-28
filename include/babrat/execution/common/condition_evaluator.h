
#ifndef BABRAT_CONDITION_EVALUATOR_H
#define BABRAT_CONDITION_EVALUATOR_H
#include <iostream>
#include <unordered_map>
#include <string>


class ConditionEvaluator {

    using RowData = std::vector<Column*>;

    const std::unordered_map<std::string, size_t>& column_positions_;

public:

    explicit  ConditionEvaluator(const std::unordered_map<std::string, size_t>& column_positions)
            : column_positions_(column_positions) {}

    bool evaluate(const RowData& row,
                  const ConditionModel& condition) const {

        if (condition.operator_ == "AND" || condition.operator_ == "OR") {
            return evaluateLogical(row, condition);
        }

        return evaluateComparison(row, condition);
    }

private:

    bool evaluateLogical(const RowData& row,
                         const ConditionModel& condition) const {

        if (condition.conditions.empty()) {
            throw std::runtime_error("Logical operator must have conditions");
        }

        bool result = (condition.operator_ == "AND");

        for (const auto& cond : condition.conditions) {
            bool cond_result = evaluate(row, cond);

            if (condition.operator_ == "AND") {
                result = result && cond_result;
                if (!result) break;
            } else {
                result = result || cond_result;
                if (result) break;
            }
        }

        return result;
    }

    bool evaluateComparison(const RowData& row,
                            const ConditionModel& condition) const {

        if (!condition.field || !condition.operator_ || !condition.value) {
            throw std::runtime_error("Invalid comparison condition structure");
        }

        auto col_it = column_positions_.find(*condition.field);

        if (col_it == column_positions_.end()) {
            throw std::runtime_error("Column '" + *condition.field + "' not found");
        }

        const auto& column = row[col_it->second];
        std::cout << ENUM_TO_STRING(column->getType()) << std::endl;


        auto value = ColumnFactory::create(column->getType());

        std::cout << "Comparing column '" << *condition.field
                  << "' of type " << ENUM_TO_STRING(column->getType())
                  << " with value '" << condition.value.value() << "'" << std::endl;
        value->insert(condition.value.value());

        PrintVisitor visitor(std::cout);


        for(auto it : row){

            if (it->isNull()){
                std::cout << "<NULL>" << " ";
                continue;
            }
            it->accept(visitor);


            std::cout << std::endl;
        }

        std::cout << std::endl;


        return !column->operation_by(condition.operator_.value(), value);

    }


};

#endif //BABRAT_CONDITION_EVALUATOR_H
