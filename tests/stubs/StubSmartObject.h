#ifndef BEAMMEUP_STUBSMARTOBJECT_H
#define BEAMMEUP_STUBSMARTOBJECT_H

#include <vector>

#include "source/Receiver.h"
#include "source/Signaler.h"
#include "source/Types.h"

namespace BeamMeUp {
    class Variant;

    /**
     * This class is used for testing smart object destruction. Because the
     * Transporter deals with some SmartObject pointers, this is necessary
     * both for testing Transporter functionality and SmartObject functionality.
     */
    class StubSmartObject : public Signaler, public Receiver {
    public:
        /**
         * Initializes the tester class, adding it to the objects collection
         */
        StubSmartObject(Transporter *transporter);

        /**
         * Clears the tester's object list. This should be called at the start of each
         * test case.
         */
        static void reset();

        /**
         * Receives some data from another object and stores it locally
         * @param signal The signal being tested
         * @param message The message
         */
        void processMessage(const Signal signal, const Variant &message) override;

        /**
         * Removes the tester class from the object collection
         */
        ~StubSmartObject() override;

        std::map<Signal, Variant> data;
        static std::vector<StubSmartObject *> objects;
    };
}

#endif //BEAMMEUP_STUBSMARTOBJECT_H
