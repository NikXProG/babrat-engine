#include "db_babrat_controller.h"
#include "string_empty_error.h"

namespace babrat{

    namespace db {

        DbController::DbController(
                const std::string& username,
                const std::string& password,
                const std::string& db_name) :
                m_dbConnectName(db_name){

            if (db_name.empty()){
                throw StringEmptyError("db_name");
            }

        }

        DbController::DbController(
                babrat::db::DbConnect dbConnect) {}

        const std::string &DbController::getDatabase() const {
            return m_dbConnectName;
        }


    }
}
