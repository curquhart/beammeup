#include "source/Transporter.h"
#include "source/Signaler.h"
#include "tests/stubs/StubSmartObject.h"
#include "tests/mocks/MockTransporter.h"

using ::testing::Exactly;
using ::testing::Return;
using ::testing::_;

namespace BeamMeUp {
    class TestArbitraryPointer : public ::testing::Test {
    protected:
        void TearDown() {
            // Reset statically stored data
            StubSmartObject::reset();
        }
    };

    // Tests that the emitter sends data to the receiver
    TEST_F(TestArbitraryPointer, SingleThreadCommunication) {
        MockTransporter transporter;

    }
}
