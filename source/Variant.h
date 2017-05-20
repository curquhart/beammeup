#ifndef BEAMMEUP_VARIANT_H
#define BEAMMEUP_VARIANT_H

#include <cstddef>
#include <string>
#include <vector>

#include "Types.h"

namespace BeamMeUp {
    typedef enum {
        D_NULL = 0,
        D_STRING = 10,
        D_STRINGVECTOR = 20,
        D_VARIANTVECTOR = 30,
        D_VARIANTMAP = 40,
        D_ULONG = 50,
        D_ULONGLONG = 60,
        D_LONG = 70,
        D_LONGLONG = 80,
        D_UINT = 90,
        D_INT = 100,
        D_USHORT = 110,
        D_SHORT = 120,
        D_FLOAT = 140,
        D_DOUBLE = 150,
        D_BOOLEAN = 160,
        D_POINTER = 170
    } DataType;

    class Variant {
    public:
        /**
         * Initializes a null variant.
         */
        Variant();

        /**
         * Makes a copy of another variant.
         * @param value The value to copy
         */
        Variant(const Variant &value);

        /**
         * Initializes a variant based on an std-string
         * @param value The value to copy
         */
        Variant(const std::string &value);

        /**
         * Initializes a variant based on a string (c-style)
         * @param value The value to copy
         */
        Variant(const char *value);

        /**
         * Initializes a variant based on a pointer
         * @param value The value to use
         * @param manage True to manage the pointer memory (i.e. delete it when we replace it with something else or we destruct)
         */
        Variant(ArbitraryPointer *value, bool manage);

        /**
         * Initializes a variant based on a string vector
         * @param value The value to copy
         */
        Variant(const std::vector<std::string> &value);

        /**
         * Initializes a variant based on a variant vector
         * @param value The value to copy
         */
        Variant(const VariantVector &value);

        /**
         * Initializes a variant based on a variant map
         * @param value The value to copy
         */
        Variant(const VariantMap &value);

        /**
         * Initializes a variant based on a long
         * @param value The value to copy
         */
        Variant(const long &value);

        /**
         * Initializes a variant based on a long long
         * @param value The value to copy
         */
        Variant(const long long &value);

        /**
         * Initializes a variant based on an unsigned long
         * @param value The value to copy
         */
        Variant(const unsigned long &value);

        /**
         * Initializes a variant based on an unsigned long long
         * @param value The value to copy
         */
        Variant(const unsigned long long &value);

        /**
         * Initializes a variant based on an integer
         * @param value The value to copy
         */
        Variant(const int &value);

        /**
         * Initializes a variant based on an unsigned integer
         * @param value The value to copy
         */
        Variant(const unsigned int &value);

        /**
         * Initializes a variant based on a short
         * @param value The value to copy
         */
        Variant(const short &value);

        /**
         * Initializes a variant based on an unsigned short
         * @param value The value to copy
         */
        Variant(const unsigned short &value);

        /**
         * Initializes a variant based on a bool
         * @param value The value to copy
         */
        Variant(const bool &value);

        /**
         * Initializes a variant based on a double
         * @param value The value to copy
         */
        Variant(const double &value);

        /**
         * Initializes a variant based on a float
         * @param value The value to copy
         */
        Variant(const float &value);

        /**
         * Returns this variant as a pointer, but only if that is already what it was.
         * If not, returns nullptr.
         * @return the internal pointer
         */
        ArbitraryPointer *toPointer() const;

        /**
         * Converts this variant to a string.
         * @return The string representation of this variant
         */
        const std::string toString() const;

        /**
         * Converts this variant to a string vector. If the data happens to be a variant
         * vector, it is iterated and each element converted. Other data types will be
         * inserted into a new vector as the first element.
         * @return The string vector representation of this variant
         */
        const std::vector<std::string> toStringVector() const;

        /**
         * Converts this variant to a variant vector. If the data happens to be a string
         * vector, it is iterated and each element converted. Other data types will be
         * inserted into a new vector as the first element.
         * @return The variant vector representation of this variant
         */
        const VariantVector toVariantVector() const;

        /**
         * Converts this variant to a variant map. If the stored type is not a variant
         * map, returns an empty variant map
         * @return The variant map representation of this object
         */
        const VariantMap toVariantMap() const;

        /**
         * Converts this variant to a float. In the event if a vector, the first
         * element will be returned.
         * @return The float representation of this object
         */
        const float toFloat() const;

        /**
         * Converts this variant to a double. In the event if a vector, the first
         * element will be returned.
         * @return The double representation of this object
         */
        const double toDouble() const;

        /**
         * Converts this variant to a long. In the event if a vector, the first
         * element will be returned.
         * @return The long representation of this object
         */
        const long toLong() const;

        /**
         * Converts this variant to a long long. In the event if a vector, the first
         * element will be returned.
         * @return The long representation of this object
         */
        const long long toLongLong() const;

        /**
         * Converts this variant to an unsigned long. In the event if a vector, the
         * first element will be returned.
         * @return The unsigned long representation of this object
         */
        const unsigned long toULong() const;

        /**
         * Converts this variant to an unsigned long long. In the event if a vector, the
         * first element will be returned.
         * @return The unsigned long representation of this object
         */
        const unsigned long long toULongLong() const;

        /**
         * Converts this variant to an integer. In the event if a vector, the first
         * element will be returned.
         * @return The int representation of this object
         */
        const int toInt() const;

        /**
         * Converts this variant to an unsigned integer. In the event if a vector, the
         * first element will be returned.
         * @return The unsigned int representation of this object
         */
        const unsigned int toUInt() const;

        /**
         * Converts this variant to a short. In the event if a vector, the first element
         * will be returned.
         * @return The short representation of this object
         */
        const short toShort() const;

        /**
         * Converts this variant to an unsigned short. In the event if a vector, the
         * first element will be returned.
         * @return The unsigned short representation of this object
         */
        const unsigned short toUShort() const;

        /**
         * Converts this variant to a bool. In the event if a vector, the first element
         * will be returned.
         * @return The unsigned short representation of this object
         */
        const bool toBool() const;

        /**
         * Checks if this variant represents nullptr. Returns true if so else false.
         * @return Is this variant referencing null?
         */
        const bool isNull() const;

        /**
         * Assigns the contents of the argument passed (value) to this object. This
         * makes a deep copy.
         * @param value The value to copy
         */
        void operator=(const Variant &value);

        /**
         * Compares this variant with the one identified by value. Returns true if they
         * are the same (comparison based on the type of this object). Else false.
         * @param value The value to compare
         */
        bool operator==(const Variant &value) const;

        /**
         * Compares this variant with the one identified by value. Returns true if this
         * is greater than or equal to value (based on their data as converted to the
         * type of this object). Else false.
         * @param value The value to compare
         */
        bool operator!=(const Variant &value) const;

        /**
         * Compares this variant with the one identified by value. Returns true if this
         * is greater than value (comparison based on the type of this object). Else
         * false.
         * @param value The value to compare
         */
        bool operator>(const Variant &value) const;

        /**
         * Compares this variant with the one identified by value. Returns true if this
         * is less than value (comparison based on the type of this object). Else false.
         * @param value The value to compare
         */
        bool operator<(const Variant &value) const;

        /**
         * Compares this variant with the one identified by value. Returns true if this
         * is less than or equal to value (based on their data as converted to the type
         * of this object). Else false.
         * @param value The value to compare
         */
        bool operator>=(const Variant &value) const;

        /**
         * Compares this variant with the one identified by value. Returns true if this
         * is greater than or equal to value (their data comparisons return true).
         * Else false.
         * @param value The value to compare
         */
        bool operator<=(const Variant &value) const;

        /**
         * Returns the actual type of this variant
         * @return The type
         */
        const DataType getType() const;

        /**
         * This object is being destroyed. Lets free up the memory used by our internal
         * data
         * @return void
         */
        ~Variant();

    private:
        /**
         * Converts our data to the type identified by T
         * @return a numeric-type cast of data
         */
        template<typename T>
        T numericCast() const;

        /**
         * Basic initialization (sets defaults)
         * @param type The type of Variant we're initializing
         */
        void init(DataType type);

        /**
         * De-initializes the data. This frees any allocated memory.
         */
        void deinit();

        void *data;
        bool deleteData;
        DataType type;
    };
}

#endif //BEAMMEUP_VARIANT_H
