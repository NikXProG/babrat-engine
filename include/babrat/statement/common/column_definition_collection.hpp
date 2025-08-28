#pragma once

#include <vector>
#include <babrat/statement/common/column_definition.hpp>

class ColumnDefinitionCollection : SqlStatement {

    std::vector<ColumnDefinition> m_columnDefinitions;

    explicit ColumnDefinitionCollection(std::vector<ColumnDefinition> columnDefinitions)
            : m_columnDefinitions(std::move(columnDefinitions)) {
        validate();
    }

public:

    static ColumnDefinitionCollection get(std::vector<ColumnDefinition> columnDefinitions) {
        return ColumnDefinitionCollection(std::move(columnDefinitions));
    }

    template<typename... Args>
    static ColumnDefinitionCollection get(Args&&... args) {
        std::vector<ColumnDefinition> columnDefinitions;
        (columnDefinitions.emplace_back(std::forward<Args>(args)), ...);
        return ColumnDefinitionCollection(std::move(columnDefinitions));
    }

public:

    bool operator==(const ColumnDefinitionCollection& other) const {
        return m_columnDefinitions == other.m_columnDefinitions;
    }

    bool operator!=(const ColumnDefinitionCollection& other) const {
        return !(*this == other);
    }

public:

    bool empty() const {
        return m_columnDefinitions.empty();
    }


    size_t size() const {
        return m_columnDefinitions.size();
    }


public:

    auto begin() const { return m_columnDefinitions.begin(); }
    auto end() const { return m_columnDefinitions.end(); }

public:

    auto cbegin() const { return m_columnDefinitions.cbegin(); }
    auto cend() const { return m_columnDefinitions.cend(); }

public:

    const std::vector<ColumnDefinition>& getColumnDefinitions() const & {
        return m_columnDefinitions;
    }

    std::vector<ColumnDefinition> getColumnDefinitions() && {
        return std::move(m_columnDefinitions);
    }


public:

    void add(ColumnDefinition columnDefinition){
        m_columnDefinitions.push_back(std::move(columnDefinition));
        validate();
    }

public:

    std::string toString() const override {

        auto accumulator = StringAccumulator("ColumnDefinitions");

        for(const auto& it : getColumnDefinitions()){
            accumulator.accumulateValue(it.toString());
        }

        return accumulator.str();

    }

public:

    void validate() const {

        std::unordered_map<std::string, int> nameCounts;

        for (const auto& elem : m_columnDefinitions) {
            nameCounts[elem.getColumnName()]++;
        }

        std::string duplicates;
        for (const auto& [name, count] : nameCounts) {
            if (count > 1) {
                if (!duplicates.empty()) duplicates += ", ";
                duplicates += name;
            }
        }

        if (!duplicates.empty()) {
            throw std::runtime_error("Duplicate column names: " + duplicates);
        }
    }


};

