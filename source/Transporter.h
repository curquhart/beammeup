#ifndef BEAMMEUP_TRANSPORTER_H
#define BEAMMEUP_TRANSPORTER_H

#ifdef THREAD_SAFE
#include <shared_mutex>
#endif
#include <queue>
#include <unordered_set>

#include "Signaler.h"
#include "Types.h"
#include "Variant.h"

namespace BeamMeUp {
    class Transporter : public Signaler {
        friend class Receiver;
        friend class Signaler;

    public:
        /**
         * Initialize the transporter
         */
        Transporter();

        /**
         * Processes any queued events
         */
        void processMessages();

        /**
         * Checks if this object is registered
         * @param object The object to check
         * @return true if this object has been registered
         */
#ifdef VIRTUAL_FOR_MOCKING
        virtual
#endif
        bool isObjectRegistered(Receiver *object);

    protected:
        /**
         * Registers this object for event processing
         * @param object The object to register
         */
#ifdef VIRTUAL_FOR_MOCKING
        virtual
#endif
        void registerObject(Receiver *object);

        /**
         * Unregisters this object
         * @param object The object to register
         */
#ifdef VIRTUAL_FOR_MOCKING
        virtual
#endif
        void unregisterObject(Receiver *object);

    private:
        typedef std::unordered_set<Receiver *> ObjectSet;
        ObjectSet objects;
#ifdef THREAD_SAFE
        std::shared_timed_mutex mutex;
#endif
    };

}

#endif //BEAMMEUP_TRANSPORTER_H
