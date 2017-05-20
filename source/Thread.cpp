#include "Thread.h"
#include "Transporter.h"

namespace BeamMeUp {

    Thread::Thread() {
        // Initialize thread
        thread = nullptr;

        // Mark stopping as true
        stopping = true;

        // Mark as finished. This is so that if the thread never starts, it counts
        // as done.
        finished = true;
    }

    void Thread::start() {
        if (thread != nullptr) {
            if (!finished && !stopping) {
                throw std::runtime_error("Thread is already started");
            } else {
                stop();
            }
        }

        // Mark as unfinished and not stopping.
        finished = false;
        stopping = false;

        thread = new std::thread(&Thread::_run, this);
    }

    void Thread::stop(bool block) {
        // Mark this thread as stopping
        stopping = true;

        if (block) {
            join();
            delete thread;
            thread = nullptr;
        }
    }

    void Thread::join() {
        if (thread == nullptr) {
            return;
        }

        if (thread->joinable()) {
            thread->join();
        }
    }

    bool Thread::isFinished() {
        if (thread == nullptr) {
            // If it hasn't started it technically hasn't finished but for our
            // intent, it has.
            return true;
        }

        return finished;
    }

    bool Thread::isStopping() {
        return stopping;
    }

    void Thread::_run() {
        // Run thread's event loop
        run();

        finished = true;
    }

    Thread::~Thread() {
        stop();
    }
}

