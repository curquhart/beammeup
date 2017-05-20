#include "StubTrackedPointer.h"

namespace BeamMeUp {
    StubTrackedPointer::StubTrackedPointer(Transporter *transporter) : ArbitraryPointer(), Receiver(transporter) {
        StubTrackedPointer::count++;
        this->transporter = transporter;
    }

    ArbitraryPointer* StubTrackedPointer::clone() {
        return new StubTrackedPointer(transporter);
    }

    StubTrackedPointer::~StubTrackedPointer() {
        StubTrackedPointer::count--;
    }

    int StubTrackedPointer::count = 0;
}