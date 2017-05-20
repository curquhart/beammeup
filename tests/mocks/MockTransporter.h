#ifndef BEAMMEUP_MOCKTRANSPORTER_H
#define BEAMMEUP_MOCKTRANSPORTER_H

#include "source/Receiver.h"
#include "source/Signaler.h"
#include "source/Transporter.h"
#include "source/Variant.h"

#include "gmock/gmock.h"

namespace BeamMeUp {
    class MockTransporter : public Transporter {
    public:
        MOCK_METHOD2(postMessage, void(int, Variant));

        MOCK_METHOD0(processMessages, void());

        MOCK_METHOD1(registerObject, void(Receiver *));

        MOCK_METHOD1(unregisterObject, void(Receiver *));

        MOCK_METHOD1(isObjectRegistered, bool(Receiver *));
    };
}

#endif //BEAMMEUP_MOCKTRANSPORTER_H
