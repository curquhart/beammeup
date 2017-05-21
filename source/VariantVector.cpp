#include "include/beammeup/Variant.h"
#include "include/beammeup/VariantVector.h"

namespace BeamMeUp {
    VariantVector &VariantVector::operator<<(const Variant &variant) {
        push_back(variant);
        return *this;
    }

    const std::string VariantVector::toString() {
        std::string valueList;

        for (auto value : *this) {
            if (valueList.size() != 0) {
                valueList += ", ";
            }

            valueList += value.toString();
        }

        return valueList;
    }
}