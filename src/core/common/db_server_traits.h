#pragma once

#ifndef BABRAT_DB_SERVER_TRAITS_H
#define BABRAT_DB_SERVER_TRAITS_H
#include <iostream>
#include "column.h"
#include "column_type.h"
#include "db_enums.h"

#include <boost/flyweight.hpp>

#define EnableWALWrite true

namespace babrat {

    namespace storage {

   /*     template<Mode Mode,
                SearchTreeVariant TreeVariant,
                bool EnableWAL = false>
        struct db_server_traits {

            static const enum Mode mode = Mode;

            static const SearchTreeVariant db_tree_variant = TreeVariant;

        };*/

        class ColumnBaseComparator {

        public:

            bool operator()(
                    Column* lhs,
                    Column* rhs) const {

                if (!lhs && !rhs) return false;
                if (!lhs) return true;
                if (!rhs) return false;

                if (lhs->getType() != rhs->getType()) {
                    return false;
                }

                return !lhs->equals(*rhs);

            }

        };


    /*    using Traits = babrat::storage::db_server_traits<
                babrat::storage::Mode::file_system,
                babrat::storage::SearchTreeVariant::b_plus,
                EnableWALWrite>;
*/
    }



}

#endif //BABRAT_DB_SERVER_TRAITS_H

