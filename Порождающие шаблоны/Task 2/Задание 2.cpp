#pragma once

#include <string>
#include <vector>
#include <map>
#include <sstream>

class SqlSelectQueryBuilder {
private:
    std::vector<std::string> columns;
    std::string table_name;
    std::map<std::string, std::string> where_conditions;

public:
    SqlSelectQueryBuilder() = default;

    SqlSelectQueryBuilder& AddColumn(const std::string& column) {
        columns.push_back(column);
        return *this;
    }

    SqlSelectQueryBuilder& AddFrom(const std::string& table) {
        table_name = table;
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::string& column, const std::string& value) {
        where_conditions[column] = value;
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept {
        for (const auto& [key, value] : kv) {
            where_conditions[key] = value;
        }
        return *this;
    }

    SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& columns_list) noexcept {
        for (const auto& col : columns_list) {
            columns.push_back(col);
        }
        return *this;
    }

    std::string BuildQuery() const {
        std::ostringstream query;

        query << "SELECT ";
        if (columns.empty()) {
            query << "*";
        }
        else {
            for (size_t i = 0; i < columns.size(); ++i) {
                if (i > 0) {
                    query << ", ";
                }
                query << columns[i];
            }
        }

        query << " FROM " << table_name;

        if (!where_conditions.empty()) {
            query << " WHERE ";
            bool first = true;
            for (const auto& [column, value] : where_conditions) {
                if (!first) {
                    query << " AND ";
                }
                query << column << "=" << value;
                first = false;
            }
        }

        query << ";";
        return query.str();
    }
};