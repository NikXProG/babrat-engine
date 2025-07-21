#ifndef BABRAT_AUTH_TABLE_H
#define BABRAT_AUTH_TABLE_H
#include <sodium.h>

#include "system_table.h"


std::string hash_password(const std::string& password) {
    if (sodium_init() < 0) throw std::runtime_error("libsodium error");


    constexpr size_t OPS_LIMIT = crypto_pwhash_argon2id_OPSLIMIT_MODERATE;
    constexpr size_t MEM_LIMIT = crypto_pwhash_argon2id_MEMLIMIT_MODERATE;

    char hash[crypto_pwhash_STRBYTES];

    if (crypto_pwhash_str(hash, password.c_str(), password.size(),
                          OPS_LIMIT, MEM_LIMIT) != 0) {
        throw std::runtime_error("Password hashing failed");
    }

    return std::string(hash);
}

bool verify_password(const std::string& hash, const std::string& password) {
    return crypto_pwhash_str_verify(hash.c_str(), password.c_str(), password.size()) == 0;
}

//
//class AuthTable : public SystemTable {
//
//
//public:
//
//    ~AuthTable() override = default;
//
//public:
//
//    explicit AuthTable(
//            const std::string& pool_name,
//            const std::string& schema_name,
//            const std::string& collection_name,
//            babrat::DatabaseStorage<Column*, std::vector<Column*>, Traits>* storage)
//            : SystemTable(pool_name,schema_name,collection_name,storage){}
//
//public:
//
//    void init_default() override {
//
//        register_column_name("auth_id");
//        register_column_name("auth_login");
//        register_column_name("auth_password");
//        register_column_name("auth_db_id");
//        register_column_name("status_connect");
//
//        register_type(
//                babrat::to_column(make_oid(BabratSystemOID::BABRAT_AUTH_DEFAULT_ID)),
//                {
//                    babrat::to_column(std::string(DB_DEFAULT_NAME)),
//                    babrat::to_column(std::string(hash_password("2334"))),
//                    babrat::to_column(make_oid(BabratSystemOID::BABRAT_DB)),
//                    babrat::to_column(true)
//                });
//
//
//    }
//
//};


#endif //BABRAT_AUTH_TABLE_H
