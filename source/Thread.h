#if !defined(BEAMMEUP_THREAD_H) && defined(THREAD_SAFE)
#define BEAMMEUP_THREAD_H

#include <thread>

namespace BeamMeUp {
    class Thread {
    public:
        /**
         * Initializes a new Thread object
         */
        Thread();

        /**
         * Starts thread execution
         */
        void start();

        /**
         * Sets this thread as "stopping". It is up to the subclass to actually handle
         * shutdown
         */
        void stop(bool block = true);

        /**
         * Joins on this thread if it is running. If not, this does nothing
         */
        void join();

        /**
         * Checks if this thread has finished running. The returns true if the thread
         * is not finished or has not started. Otherwise returns false
         * @return The state of this thread
         */
        bool isFinished();

        /**
         * Returns true if the thread is in the process of stopping, else false
         * @return bool
         */
        bool isStopping();

        /**
         * Waits for the thread to shut down and then does any cleanup tasks
         */
        virtual ~Thread();

    protected:
        /**
         * Runs the thread
         */
        virtual void run() = 0;

    private:
        /**
         * Runs the thread and cleanup tasks
         */
        void _run();

        std::thread *thread;
        std::atomic_bool finished;
        std::atomic_bool stopping;
    };
}

#endif //BEAMMEUP_THREAD_H
