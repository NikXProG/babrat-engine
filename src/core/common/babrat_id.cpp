#include "babrat_id.h"

namespace babrat {

    DefaultOidGenerator::DefaultOidGenerator() :
            nextOid(FirstNormalObjectId)  {}

    Oid DefaultOidGenerator::generate() {

        Oid newOid = nextOid.fetch_add(1, std::memory_order_relaxed);

        if (newOid >= FirstNormalObjectId + (MaxOid - FirstNormalObjectId) / 2) {

            newOid = nextOid.load(std::memory_order_relaxed);

            if (newOid >= FirstNormalObjectId + (MaxOid - FirstNormalObjectId) / 2) {
                nextOid.store(FirstNormalObjectId, std::memory_order_relaxed);
                newOid = FirstNormalObjectId;
            }

        }

        return newOid;
    }

}
