#pragma once

#include <iostream>

#define OidIsValid(OID) OidIsValid(OID)

namespace babrat {

    typedef unsigned int Oid;

    typedef unsigned int IdxType;

    enum BabratSystemOID {
        BABRAT_DB = 5,
        BABRAT_ROLE = 6,
        BABRAT_NAMESPACE = 7,
        BOOL = 16,
        VARCHAR = 17,
        INTEGER = 23,
        UINT32 = 24,
        TEXT = 25,
        BABRAT_AUTH_DEFAULT_ID =  100,
    };

    constexpr Oid InvalidOid = 0;
    constexpr Oid FirstNormalObjectId = 5461;
    constexpr Oid MaxOid = 4294967295U;

    Oid toOid(BabratSystemOID value) {
        return static_cast<Oid>(value);
    }

    inline bool OidIsValid(Oid oid) {
        return oid != InvalidOid;
    }

    class OidGenerator {

    public:

        virtual ~OidGenerator() noexcept = default;

    public:

        virtual Oid generate() = 0;

    };


    class DefaultOidGenerator : OidGenerator {

    private:

        std::atomic<Oid> nextOid;

    public:

        explicit DefaultOidGenerator();

    public:

        Oid generate() override;

    public:

        ~DefaultOidGenerator() noexcept override = default;

    };

}
