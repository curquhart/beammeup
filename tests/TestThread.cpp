#include <algorithm>

#include "source/Thread.h"
#include "tests/mocks/MockTransporter.h"

using ::testing::_;
using ::testing::Exactly;
using ::testing::Return;

namespace BeamMeUp {
    class TestThreader : public Thread {

    public:
        static const int DELAY_MULTIPLIER;

        TestThreader();

        TestThreader(unsigned int delayInMicroseconds);

        void run() override;

        ~TestThreader() override;

        unsigned int delay;
        static std::mutex mutex;
        static std::vector<TestThreader *> deletedThreads;
    };
    const int TestThreader::DELAY_MULTIPLIER = 200;

    std::mutex TestThreader::mutex;
    std::vector<TestThreader *> TestThreader::deletedThreads;

    TestThreader::TestThreader() : Thread() {
        this->delay = 0;
    }

    TestThreader::TestThreader(unsigned int delayInMicroseconds) : Thread() {
        // Multiply delay by DELAY_MULTIPLIER (the number is somewhat arbitrary really) to reduce edge cases caused by slow VMs
        this->delay = delayInMicroseconds * DELAY_MULTIPLIER;
    }

    void TestThreader::run() {
        // Wait until finished is true
        while (!isStopping()) {

            // Sleep for 5 μs
            std::this_thread::sleep_for(std::chrono::microseconds(5 * DELAY_MULTIPLIER));
        }

        if (delay > 0) {

            // Wait for delay ms before actually shutting down
            std::this_thread::sleep_for(std::chrono::microseconds(delay));
        }
    }

    TestThreader::~TestThreader() {

        // Stop it!
        stop();

        // Join is also called in the parent destructor, but we want to ensure shut
        // down is actually complete before we add to our collection of shut down
        // threads
        join();

        // Lock mutex
        mutex.lock();

        // Add to collection
        // This data is used in TEST(Thread, ShutdownCompletedFromDestructor)
        deletedThreads.push_back(this);

        // Unlock mutex
        mutex.unlock();
    }

    // Tests that the thread has shut down successfully
    TEST(TestThread, ShutdownCompleted) {
        // Initialize a thread
        TestThreader thread;

        // Start the thread
        thread.start();

        // Ensure thread is not finished
        ASSERT_FALSE(thread.isFinished());

        // Tell it to stop and block until it does so
        thread.stop(true);

        // Check if the thread has actually stopped
        ASSERT_TRUE(thread.isFinished());
    }

    // Tests that the thread has shut down successfully when shut down actually
    // takes time
    TEST(TestThread, ShutdownCompletedAfterTime) {
        // Initialize a thread with a 2 μs shutdown time
        TestThreader thread(2);

        // Start the thread
        thread.start();

        // Sleep for 1 μs to ensure that the thread has actually started running
        std::this_thread::sleep_for(std::chrono::microseconds(TestThreader::DELAY_MULTIPLIER));

        // Tell the thread to stop
        thread.stop(false);

        // The thread takes at least 2 μs to shut down, so it should still be
        // running
        ASSERT_FALSE(thread.isFinished());

        // Sleep for enough time for the thread to shut down. 50 μs?
        std::this_thread::sleep_for(std::chrono::microseconds(50 * TestThreader::DELAY_MULTIPLIER));

        // The thread should definitely be finished now
        ASSERT_TRUE(thread.isFinished());
    }

    // Tests that the start method throws an exception if the thread is already running
    TEST(TestThread, DoubleStartThrowsException) {
        // Initialize a thread with a 2 μs shutdown time
        TestThreader thread(2);

        // Start the thread
        thread.start();
        ASSERT_THROW(thread.start(), std::runtime_error);

        // Sleep for 1 μs to ensure that the thread has actually started running
        std::this_thread::sleep_for(std::chrono::microseconds(1 * TestThreader::DELAY_MULTIPLIER));

        // Tell the thread to stop
        thread.stop(false);

        // The thread takes at least 2 μs to shut down, so it should still be
        // running
        ASSERT_FALSE(thread.isFinished());

        // Sleep for enough time for the thread to shut down. 50 μs?
        std::this_thread::sleep_for(std::chrono::microseconds(50 * TestThreader::DELAY_MULTIPLIER));

        // The thread should definitely be finished now
        ASSERT_TRUE(thread.isFinished());
    }

    // Tests that the start method waits for the thread to complete and then restarts it if it is shutting down already
    TEST(TestThread, DoubleStartWhileStoppingWaitsAndThenRestarts) {
        // Initialize a thread with a 2 μs shutdown time
        TestThreader thread(2);

        // Start the thread
        thread.start();
        thread.stop(false);
        thread.start();

        // Sleep for 1 μs to ensure that the thread has actually started running
        std::this_thread::sleep_for(std::chrono::microseconds(1 * TestThreader::DELAY_MULTIPLIER));

        // Tell the thread to stop
        thread.stop(false);

        // The thread takes at least 2 μs to shut down, so it should still be
        // running
        ASSERT_FALSE(thread.isFinished());

        // Sleep for enough time for the thread to shut down. 50 μs?
        std::this_thread::sleep_for(std::chrono::microseconds(50 * TestThreader::DELAY_MULTIPLIER));

        // The thread should definitely be finished now
        ASSERT_TRUE(thread.isFinished());
    }

    // Tests that the thread has shut down successfully when shut down by the
    // application. This is to ensure that an infinite loop isn't created on a child
    // thread.
    TEST(TestThread, ShutdownCompletedFromDestructor) {
        // Initialize a thread
        TestThreader *thread = new TestThreader();

        // Start it up
        thread->start();

        // Shut down thread
        delete thread;

        // Lock mutex
        TestThreader::mutex.lock();

        // Make a copy of the deleted threads vector
        auto vector = TestThreader::deletedThreads;

        // Unlock mutex
        TestThreader::mutex.unlock();

        // Ensure that the thread in question was in fact shut down
        ASSERT_NE(std::find(vector.begin(), vector.end(), thread), vector.end());
    }
}

