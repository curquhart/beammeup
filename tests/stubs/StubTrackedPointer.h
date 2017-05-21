#ifndef BEAMMEUP_STUBTRACKEDPOINTER_H
#define BEAMMEUP_STUBTRACKEDPOINTER_H

#include "include/beammeup/Receiver.h"

#include "include/beammeup/ArbitraryPointer.h"

namespace BeamMeUp {
    /**
     * This class counts tracked pointers in order to make sure they are appropriately constructed and destructed
     */
    class StubTrackedPointer : public ArbitraryPointer, public Receiver {
    public:
        static int count;

        StubTrackedPointer(Transporter *transporter);

        virtual ArbitraryPointer *clone() override;

        virtual ~StubTrackedPointer() override;

    private:
        Transporter *transporter;
    };
}

#endif //BEAMMEUP_STUBTRACKEDPOINTER_H
