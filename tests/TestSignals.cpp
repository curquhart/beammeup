#include "source/Transporter.h"
#include "source/Signaler.h"
#include "source/VariantVector.h"

#include "tests/stubs/StubSmartObject.h"
#include "tests/mocks/MockTransporter.h"

using ::testing::Exactly;
using ::testing::Return;
using ::testing::_;

namespace BeamMeUp {
    class TestSignals : public ::testing::Test {
    protected:
        void TearDown() {
            // Reset statically stored data
            StubSmartObject::reset();
        }
    };

    // Tests that the emitter sends data to the receiver
    TEST_F(TestSignals, NullReceiver) {
        MockTransporter transporter;

        EXPECT_CALL(transporter, registerObject(_)).Times(Exactly(1));
        StubSmartObject emitter(&transporter);
        EXPECT_CALL(transporter, unregisterObject(&emitter)).Times(Exactly(1));

        emitter.connect(1, nullptr);
    }

    // Tests that disconnect by signal works
    TEST_F(TestSignals, DisconnectBySignal) {
        MockTransporter transporter;

        EXPECT_CALL(transporter, registerObject(_)).Times(Exactly(4));
        StubSmartObject emitter(&transporter);
        StubSmartObject receiver1(&transporter);
        StubSmartObject receiver2(&transporter);
        StubSmartObject receiver3(&transporter);
        EXPECT_CALL(transporter, unregisterObject(&emitter)).Times(Exactly(1));
        EXPECT_CALL(transporter, unregisterObject(&receiver1)).Times(Exactly(1));
        EXPECT_CALL(transporter, unregisterObject(&receiver2)).Times(Exactly(1));
        EXPECT_CALL(transporter, unregisterObject(&receiver3)).Times(Exactly(1));

        emitter.connect(1, &receiver1);
        emitter.connect(1, &receiver2);
        emitter.connect(2, &receiver2);
        emitter.connect(2, &receiver3);
        emitter.disconnect(1);
        auto connectedObjects = emitter.getConnectedObjects();
        ASSERT_EQ(0, connectedObjects.count(1));
        ASSERT_EQ(2, connectedObjects.count(2));
        ASSERT_EQ(2, connectedObjects.size());
        bool found2 = false;
        bool found3 = false;
        for (auto element : connectedObjects) {
            if (element.second == &receiver2) {
                found2 = true;
            }
            if (element.second == &receiver3) {
                found3 = true;
            }
        }
        ASSERT_TRUE(found2);
        ASSERT_TRUE(found3);
    }

    // Tests that disconnect by receiver works
    TEST_F(TestSignals, DisconnectByReceiver) {
        MockTransporter transporter;

        EXPECT_CALL(transporter, registerObject(_)).Times(Exactly(3));
        StubSmartObject emitter(&transporter);
        StubSmartObject receiver1(&transporter);
        StubSmartObject receiver2(&transporter);
        EXPECT_CALL(transporter, unregisterObject(&emitter)).Times(Exactly(1));
        EXPECT_CALL(transporter, unregisterObject(&receiver1)).Times(Exactly(1));
        EXPECT_CALL(transporter, unregisterObject(&receiver2)).Times(Exactly(1));

        emitter.connect(1, &receiver1);
        emitter.connect(1, &receiver2);
        emitter.connect(2, &receiver1);
        emitter.connect(2, &receiver2);
        emitter.disconnect(&receiver1);
        auto connectedObjects = emitter.getConnectedObjects();
        ASSERT_EQ(1, connectedObjects.count(1));
        ASSERT_EQ(1, connectedObjects.count(2));
        ASSERT_EQ(2, connectedObjects.size());
        for (auto element : connectedObjects) {
            ASSERT_TRUE(element.second == &receiver2);
        }
    }

    // Tests that disconnect by receiver works
    TEST_F(TestSignals, DisconnectBySignalAndReceiver) {
        MockTransporter transporter;

        EXPECT_CALL(transporter, registerObject(_)).Times(Exactly(3));
        StubSmartObject emitter(&transporter);
        StubSmartObject receiver1(&transporter);
        StubSmartObject receiver2(&transporter);
        EXPECT_CALL(transporter, unregisterObject(&emitter)).Times(Exactly(1));
        EXPECT_CALL(transporter, unregisterObject(&receiver1)).Times(Exactly(1));
        EXPECT_CALL(transporter, unregisterObject(&receiver2)).Times(Exactly(1));

        emitter.connect(1, &receiver1);
        emitter.connect(2, &receiver1);
        emitter.connect(3, &receiver1);
        emitter.connect(1, &receiver2);
        emitter.connect(2, &receiver2);
        emitter.connect(3, &receiver2);
        emitter.disconnect(1, &receiver1);
        emitter.disconnect(2, &receiver1);

        auto connectedObjects = emitter.getConnectedObjects();
        ASSERT_EQ(1, connectedObjects.count(1));
        ASSERT_EQ(1, connectedObjects.count(2));
        ASSERT_EQ(2, connectedObjects.count(3));
        ASSERT_EQ(4, connectedObjects.size());
        for (auto element : connectedObjects) {
            ASSERT_TRUE(element.first == 3 || element.second == &receiver2);
        }
    }

    // Tests that disconnect all works
    TEST_F(TestSignals, DisconnectAll) {
        MockTransporter transporter;

        EXPECT_CALL(transporter, registerObject(_)).Times(Exactly(3));
        StubSmartObject emitter(&transporter);
        StubSmartObject receiver1(&transporter);
        StubSmartObject receiver2(&transporter);
        EXPECT_CALL(transporter, unregisterObject(&emitter)).Times(Exactly(1));
        EXPECT_CALL(transporter, unregisterObject(&receiver1)).Times(Exactly(1));
        EXPECT_CALL(transporter, unregisterObject(&receiver2)).Times(Exactly(1));

        emitter.connect(1, &receiver1);
        emitter.connect(2, &receiver1);
        emitter.connect(3, &receiver1);
        emitter.connect(1, &receiver2);
        emitter.connect(2, &receiver2);
        emitter.connect(3, &receiver2);
        emitter.disconnectAll();

        ASSERT_EQ(0, emitter.getConnectedObjects().size());
    }

    // Tests that the emitter sends data to the receiver
    TEST_F(TestSignals, SingleThreadCommunication) {
        MockTransporter transporter;

        EXPECT_CALL(transporter, registerObject(_)).Times(Exactly(2));
        StubSmartObject emitter(&transporter);
        StubSmartObject receiver(&transporter);
        EXPECT_CALL(transporter, unregisterObject(&emitter)).Times(Exactly(1));
        EXPECT_CALL(transporter, unregisterObject(&receiver)).Times(Exactly(1));
        EXPECT_CALL(transporter, isObjectRegistered(&receiver)).Times(Exactly(2)).WillRepeatedly(Return(true));

        Signal queueNumber = 1;

        // Connect the emitter and receiver
        emitter.connect(queueNumber, &receiver);

        ASSERT_EQ(0, receiver.data.size());

        // Send data
        emitter.notify(1, Variant(VariantVector() << "test" << "test 2"));
        emitter.notify(1, Variant(VariantVector() << "test 3" << "test 4"));

        // Process queue
        ASSERT_EQ(2, receiver.processMessages());

        // We're only storing the most recent (per queue) message. It's expected that our queue has only one element.
        ASSERT_EQ(1, receiver.data.size());
        ASSERT_EQ(2, receiver.data[queueNumber].toVariantVector().size());
        ASSERT_EQ("test 3", receiver.data[queueNumber].toVariantVector()[0].toString());
        ASSERT_EQ("test 4", receiver.data[queueNumber].toVariantVector()[1].toString());
    }
}
