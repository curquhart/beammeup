#include <algorithm>

#include "StubSmartObject.h"

namespace BeamMeUp {
    StubSmartObject::StubSmartObject(Transporter *transporter) :
            Signaler(transporter), Receiver(transporter) {
        // Add to collection
        objects.push_back(this);
    }

    StubSmartObject::~StubSmartObject() {
        auto it = std::find(objects.begin(), objects.end(), this);

        if (it != objects.end()) {

            // Remove from collection
            objects.erase(it);
        }
    }

    void StubSmartObject::reset() {

        // Clear collection
        objects.clear();
    }

    void StubSmartObject::processMessage(const Signal queue, const Variant &message) {

        // Set data
        data[queue] = message;
    }

    std::vector<StubSmartObject *> StubSmartObject::objects;
}
