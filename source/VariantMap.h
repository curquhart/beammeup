#ifndef BEAMMEUP_VARIANTMAP_H
#define BEAMMEUP_VARIANTMAP_H

#include <map>
#include <string>

#include "Types.h"

namespace BeamMeUp {
    class VariantMap : public std::map<std::string, Variant> {
    };
}

#endif //BEAMMEUP_VARIANTMAP_H
