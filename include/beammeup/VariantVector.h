#ifndef BEAMMEUP_VARIANTVECTOR_H
#define BEAMMEUP_VARIANTVECTOR_H

#include <vector>

#include "Types.h"

namespace BeamMeUp {
    class VariantVector : public std::vector<Variant> {

    public:
        /**
         * Writes our data to the provided variant list.
         * @param variant A variant object
         * @return The original variant vector
         */
        VariantVector &operator<<(const Variant &value);

        /**
        * Converts this to a Variant and returns the newly created object
        * @return the return variant
        */
        const std::string toString();
    };
}

#endif //BEAMMEUP_VARIANTVECTOR_H
