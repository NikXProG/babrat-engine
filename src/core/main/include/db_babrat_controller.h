#pragma once

#include "db_controller.h"

namespace babrat {

    namespace db {

        class DbController : public IDbController {

            std::string m_dbConnectName;

        public:

            explicit DbController(
                    const std::string& username,
                    const std::string& password,
                    const std::string& db_name);

            explicit DbController(
                    DbConnect dbConnect);

        public:

            const std::string& getDatabase() const override;


        };

    }

}


