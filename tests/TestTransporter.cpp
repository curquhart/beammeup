#include "include/beammeup/Transporter.h"
#include "tests/mocks/MockMutex.h"
#include "tests/stubs/StubSmartObject.h"

using ::testing::_;
using ::testing::Exactly;
using ::testing::Return;

namespace BeamMeUp {
    TEST(TestTransporter, ShutdownAfterChildren) {
        // Test that transporter dying after children doesn't crash
        auto transporter = new Transporter();
        auto object1 = new StubSmartObject(transporter);
        auto object2 = new StubSmartObject(transporter);

        delete object1;
        delete object2;
        delete transporter;
    }

    TEST(TestTransporter, ProcessMessages) {
        {
            // Unchecked
            Transporter transporter;
            StubSmartObject emitter(&transporter);
            StubSmartObject receiver(&transporter);
            emitter.connect(1, &receiver);
            emitter.notify(1, "test");

            ASSERT_EQ(0, receiver.data.size());
            transporter.processMessages();
            ASSERT_EQ(1, receiver.data.size());

            ASSERT_NE(receiver.data.end(), receiver.data.find(1));
            ASSERT_EQ("test", receiver.data[1].toString());
            ASSERT_EQ(emitter.data.end(), emitter.data.find(1));
        }
    }
}
