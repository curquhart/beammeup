#ifndef BEAMMEUP_RECEIVER_H
#define BEAMMEUP_RECEIVER_H

#include <map>
#ifdef THREAD_SAFE
#include <shared_mutex>
#endif
#include <queue>
#include <string>
#include <typeinfo>

#include "Types.h"
#include "Variant.h"

namespace BeamMeUp {
    class Receiver {
        friend class Transporter;
        friend class Signaler;

    public:
        int processMessages();

    protected:
        /**
         * initialize Receiver
         * @param transporter
         */
        Receiver(Transporter *transporter = nullptr);

        /**
         * Receive some data from another object
         * @param signal The signal
         * @param message The message to receive
         */
        void receiveMessage(Signal signal, const Variant &message);

        /**
         * Process a message received from another object. By default, this does nothing.
         * @param queue
         * @param message
         */
        virtual void processMessage(const Signal signal, const Variant &message);

        /**
         * Disconnects the receiver
         */
        virtual ~Receiver();

    private:
        Transporter *transporter;
        std::queue<std::pair<Signal, Variant>> messageQueue;
#ifdef THREAD_SAFE
        std::shared_timed_mutex mutex;
#endif
    };
}

#endif //BEAMMEUP_RECEIVER_H
