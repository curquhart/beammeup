#ifndef BEAMMEUP_SIGNALER_H
#define BEAMMEUP_SIGNALER_H

#include <map>
#include <queue>
#include <shared_mutex>
#include <string>
#include <typeinfo>

#include "Receiver.h"
#include "Types.h"
#include "Variant.h"

namespace BeamMeUp {
    class Signaler {
        friend class Receiver;
        friend class Transporter;

    public:
        /**
         * Initializes the SmartObject object
         * @param parent The parent to set
         */
        Signaler(Transporter *transporter);

        /**
         * Connects to receiver's queue. Receiver will be sent data we emit
         * @param signal The Signal to connect
         * @param receiver The receiving object
         */
        void connect(Signal signal, Receiver *receiver);

        /**
         * Disconnects from any objects identified by signal
         * @param id The id of the queue to disconnect
         */
        void disconnect(Signal signal);

        /**
         * Disconnects from the object receiver
         * @param receiver The receiver to disconnect from
         */
        void disconnect(const Receiver *receiver);

        /**
         * Disconnects from the object receiver
         * @param Signal The id signal disconnect
         * @param receiver The receiver to disconnect from
         */
        void disconnect(Signal id, const Receiver *receiver);

        /**
         * Disconnects from all signals on all objects
         */
        void disconnectAll();

        /**
         * @return All objects connected to this signaler
         */
        std::multimap<Signal, Receiver *> getConnectedObjects();

        /**
         * Queue some data for other object(s) to pickup
         * @param signal The signal to post
         * @param data The message to send
         * @return void
         */
        void notify(Signal signal, const Variant &message);

    private:
        Transporter *transporter;
        std::multimap<Signal, Receiver *> connectedObjects;
#ifdef THREAD_SAFE
        std::shared_timed_mutex mutex;
#endif
    };
}

#endif //BEAMMEUP_SIGNALER_H
