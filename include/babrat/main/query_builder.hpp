#pragma once

#include <babrat/common/types/string.hpp>
#include <babrat/common/types/shared_ptr.hpp>
#include <babrat/common/types/vector.hpp>
#include <babrat/common/babrat_api.hpp>


namespace babrat {

    class QueryBuilder : public enable_shared_from_this<QueryBuilder> {

    public:

        //===--------------------------------------------------------------------===//
        // SELECT operations
        //===--------------------------------------------------------------------===//

        BABRAT_API shared_ptr<QueryBuilder> select(const string &selectList);
        BABRAT_API shared_ptr<QueryBuilder> select(const string &expression, const string &alias);
        BABRAT_API shared_ptr<QueryBuilder> select(const string &selectList, const vector<string> &aliases);
        BABRAT_API shared_ptr<QueryBuilder> select(const vector<string> &expressions);
        BABRAT_API shared_ptr<QueryBuilder> Select(const vector<string> &expressions, const vector<string> &aliases);

    public:

        //===--------------------------------------------------------------------===//
        // WHERE operations
        //===--------------------------------------------------------------------===//

        BABRAT_API shared_ptr<QueryBuilder> where(const string &expression);
        BABRAT_API shared_ptr<QueryBuilder> where(const vector<string> &expressions);

    public:

        //===--------------------------------------------------------------------===//
        // LIMIT operations
        //===--------------------------------------------------------------------===//

        BABRAT_API shared_ptr<QueryBuilder> limit(int64_t n, int64_t offset = 0);

    public:

        //===--------------------------------------------------------------------===//
        // ORDER BY operations
        //===--------------------------------------------------------------------===//

        BABRAT_API shared_ptr<QueryBuilder> orderBy(const string &expression);
        BABRAT_API shared_ptr<QueryBuilder> orderBy(const vector<string> &expressions);

    public:


    };


} // namespace babrat