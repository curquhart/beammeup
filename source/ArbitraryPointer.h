#ifndef BEAMMEUP_ARBITRARYPOINTER_H
#define BEAMMEUP_ARBITRARYPOINTER_H

namespace BeamMeUp {
    /**
     * ArbitraryPointer is an interface that can be stored in Variant. It exists so that any class can extend it and
     * thus be usable with Variant. Note that the main reason for its existence (as opposed to just allowing void *
     * pointers in Variant) is so that the object's destructor is called on destroy.
     */
    class ArbitraryPointer {
    public:
        /**
         * Clone this object
         * If this object is managed by Variant then this can be shallow (or even return this if desired) but if it is
         * not managed by variant, it MUST be a deep copy or memory corruption may occur.
         * @return a clone of this object.
         */
        virtual ArbitraryPointer *clone() = 0;

        /**
         * Destroy this object
         */
        virtual ~ArbitraryPointer();
    };

}

#endif //BEAMMEUP_ARBITRARYPOINTER_H
