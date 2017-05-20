#include <algorithm>
#include <cmath>
#include <sstream>

#include "ArbitraryPointer.h"
#include "Variant.h"
#include "VariantMap.h"
#include "VariantVector.h"

namespace BeamMeUp {
    void Variant::init(DataType type) {
        this->type = type;
        this->data = nullptr;
        switch (type) {
            case D_NULL:
            case D_POINTER:
                deleteData = false;
                break;
            case D_STRING:
            case D_STRINGVECTOR:
            case D_VARIANTVECTOR:
            case D_VARIANTMAP:
            case D_LONG:
            case D_LONGLONG:
            case D_ULONG:
            case D_ULONGLONG:
            case D_INT:
            case D_UINT:
            case D_SHORT:
            case D_USHORT:
            case D_BOOLEAN:
            case D_DOUBLE:
            case D_FLOAT:
                deleteData = true;
                break;
        }
    }

    Variant::Variant() {
        init(D_NULL);
    }

    Variant::Variant(const Variant &value) {
        init(D_NULL);
        *this = value;
    }

    Variant::Variant(const std::string &value) {
        init(D_STRING);
        data = new std::string(value);
    }

    Variant::Variant(const char *value) {
        init(D_STRING);
        data = new std::string(value);
    }

    Variant::Variant(ArbitraryPointer *value, bool manage) {
        if (value == nullptr) {
            init(D_NULL);
        } else {
            init(D_POINTER);
            data = value;
            deleteData = manage;
        }
    }

    Variant::Variant(const std::vector<std::string> &value) {
        init(D_STRINGVECTOR);
        data = new std::vector<std::string>(value);
    }

    Variant::Variant(const VariantVector &value) {
        init(D_VARIANTVECTOR);
        data = new VariantVector(value);
    }

    Variant::Variant(const VariantMap &value) {
        init(D_VARIANTMAP);
        data = new VariantMap(value);
    }

    Variant::Variant(const long &value) {
        init(D_LONG);
        data = new long(value);
    }

    Variant::Variant(const long long &value) {
        init(D_LONGLONG);
        data = new long long(value);
    }

    Variant::Variant(const unsigned long &value) {
        init(D_LONG);
        data = new unsigned long(value);
    }

    Variant::Variant(const unsigned long long &value) {
        init(D_ULONGLONG);
        data = new unsigned long long(value);
    }

    Variant::Variant(const int &value) {
        init(D_INT);
        data = new int(value);
    }

    Variant::Variant(const unsigned int &value) {
        init(D_UINT);
        data = new unsigned int(value);
    }

    Variant::Variant(const short &value) {
        init(D_SHORT);
        data = new short(value);
    }

    Variant::Variant(const unsigned short &value) {
        init(D_USHORT);
        data = new unsigned short(value);
    }

    Variant::Variant(const bool &value) {
        init(D_BOOLEAN);
        data = new bool(value);
    }

    Variant::Variant(const double &value) {
        init(D_DOUBLE);
        data = new double(value);
    }

    Variant::Variant(const float &value) {
        init(D_FLOAT);
        data = new float(value);
    }

    ArbitraryPointer *Variant::toPointer() const {
        if (type == D_POINTER) {
            return static_cast<ArbitraryPointer *>(data);
        }

        return nullptr;
    }

    const std::string Variant::toString() const {
        std::stringstream stream;

        switch (type) {
            case D_POINTER:
                return "";
            case D_STRING:
                return *static_cast<std::string *>(data);
            case D_VARIANTVECTOR: {
                VariantVector vector(*static_cast<VariantVector *>(data));
                return vector.toString();
            }
            case D_STRINGVECTOR: {
                std::vector<std::string> vector(
                        *static_cast<std::vector<std::string> *>(data));
                if (vector.size() == 0) {
                    return "";
                }
                return vector.at(0);
            }
            case D_DOUBLE:
                stream << *static_cast<double *>(data);
                return stream.str();
            case D_FLOAT:
                stream << *static_cast<float *>(data);
                return stream.str();
            case D_SHORT:
                stream << *static_cast<short *>(data);
                return stream.str();
            case D_USHORT:
                stream << *static_cast<unsigned short *>(data);
                return stream.str();
            case D_BOOLEAN:
                if (*static_cast<bool *>(data)) {
                    return "true";
                } else {
                    return "false";
                }
            case D_INT:
                stream << *static_cast<int *>(data);
                return stream.str();
            case D_UINT:
                stream << *static_cast<unsigned int *>(data);
                return stream.str();
            case D_LONG:
                stream << *static_cast<long *>(data);
                return stream.str();
            case D_ULONG:
                stream << *static_cast<unsigned long *>(data);
                return stream.str();
            case D_NULL:
            case D_VARIANTMAP:
            default:
                return "";
        }
    }

    const std::vector<std::string> Variant::toStringVector() const {
        std::vector<std::string> vector;

        switch (type) {
            case D_STRINGVECTOR:
                return *static_cast<std::vector<std::string> *>(data);
            case D_VARIANTVECTOR: {
                VariantVector variantVector = *static_cast<VariantVector *>(data);
                for (auto it = variantVector.begin(); it != variantVector.end(); ++it) {
                    vector.push_back((*it).toString());
                }
                break;
            }
            case D_POINTER:
            case D_NULL:
            case D_VARIANTMAP:
                break;
            default:
                vector.push_back(toString());
                break;
        }

        return vector;
    }

    const VariantVector Variant::toVariantVector() const {
        VariantVector vector;

        switch (type) {
            case D_VARIANTVECTOR:
                return *static_cast<VariantVector *>(data);
            case D_STRINGVECTOR: {
                std::vector<std::string> stringVector = *static_cast<std::vector<
                        std::string> *>(data);
                for (std::vector<std::string>::iterator it = stringVector.begin();
                     it != stringVector.end(); ++it) {
                    vector.push_back(Variant(*it));
                }
                break;
            }
            case D_POINTER:
            case D_NULL:
            case D_VARIANTMAP:
                break;
            default:
                vector.push_back(*this);
                break;
        }

        return vector;
    }

    const VariantMap Variant::toVariantMap() const {
        if (type == D_VARIANTMAP) {

            return *static_cast<VariantMap *>(data);
        }

        return VariantMap();
    }

    const float Variant::toFloat() const {
        return numericCast<float>();
    }

    const double Variant::toDouble() const {
        return numericCast<double>();
    }

    const long Variant::toLong() const {
        return numericCast<long>();
    }

    const long long Variant::toLongLong() const {
        return numericCast<long long>();
    }

    const unsigned long Variant::toULong() const {
        return numericCast<unsigned long>();
    }

    const unsigned long long Variant::toULongLong() const {
        return numericCast<unsigned long long>();
    }

    const int Variant::toInt() const {
        return numericCast<int>();
    }

    const unsigned int Variant::toUInt() const {
        return numericCast<unsigned int>();
    }

    const short Variant::toShort() const {
        return numericCast<short>();
    }

    const unsigned short Variant::toUShort() const {
        return numericCast<unsigned short>();
    }

    const bool Variant::toBool() const {
        if (type == D_BOOLEAN) {

            // Cast to bool
            return numericCast<bool>();
        }
        auto data = toString();

        // Convert to lowercase
        std::transform(data.begin(), data.end(), data.begin(), ::tolower);

        return data == "true" || numericCast<bool>();
    }

    const bool Variant::isNull() const {
        return type == D_NULL || data == nullptr;
    }

    void Variant::operator=(const Variant &value) {
        if (deleteData && data != value.data) {
            deinit();
        }

        init(value.type);
        switch (type) {
            case D_NULL:
                data = nullptr;
                break;
            case D_POINTER:
                if (value.data == nullptr) {
                    init(D_NULL);
                } else {
                    data = value.toPointer()->clone();
                    if (data == nullptr) {
                        init(D_NULL);
                    } else {
                        deleteData = value.deleteData;
                    }
                }
                break;
            case D_STRING:
                data = new std::string(value.toString());
                break;
            case D_STRINGVECTOR:
                data = new std::vector<std::string>(value.toStringVector());
                break;
            case D_VARIANTVECTOR:
                data = new VariantVector(value.toVariantVector());
                break;
            case D_VARIANTMAP:
                data = new VariantMap(value.toVariantMap());
                break;
            case D_LONG:
                data = new long(value.toLong());
                break;
            case D_LONGLONG:
                data = new unsigned long long(value.toLong());
                break;
            case D_ULONG:
                data = new unsigned long(value.toLong());
                break;
            case D_ULONGLONG:
                data = new unsigned long long(value.toLong());
                break;
            case D_INT:
                data = new int(value.toLong());
                break;
            case D_UINT:
                data = new unsigned int(value.toLong());
                break;
            case D_SHORT:
                data = new short(value.toLong());
                break;
            case D_USHORT:
                data = new unsigned short(value.toLong());
                break;
            case D_BOOLEAN:
                data = new bool(value.toBool());
                break;
            case D_DOUBLE:
                data = new double(value.toDouble());
                break;
            case D_FLOAT:
                data = new float(value.toFloat());
                break;
        }
    }

    bool Variant::operator==(const Variant &value) const {
        DataType type;

        // By default use our type
        type = this->type;

        // Because of precision differences, we need to treat double-float
        // comparisons specially. Realistically, this comparison shouldn't ever
        // happen >.<
        if (type == D_FLOAT && value.type == D_DOUBLE) {

            return fabs(toFloat() - value.toDouble()) < 0.00001;
        } else if (type == D_DOUBLE && value.type == D_FLOAT) {

            return fabs(toDouble() - value.toFloat()) < 0.00001;
        }

        // We need to make sure that if a equals b, that b also equals a. To do this
        // it is necessary to always use the greatest (or least) type. However, it
        // is impractical to prefer string over double or float as that would make
        // 1.0 not equal "1", so decimal types are always greater than non-decimal
        // types. NONE is a special case and is not simply put at the end of the
        // DataType list because if types get added in the future and numeric
        // representations are changed in a data store, that would change
        // the id of it and break all the things.
        if (type != D_NULL && (value.type == D_NULL || value.type > type)) {

            // Set type to the right side's type (greater than left or NONE)
            type = value.type;
        }

        switch (type) {
            case D_POINTER:
                return toPointer() == value.toPointer();
            case D_STRING:
                return toString() == value.toString();
            case D_STRINGVECTOR:
                return toStringVector() == value.toStringVector();
            case D_VARIANTVECTOR:
                return toVariantVector() == value.toVariantVector();
            case D_VARIANTMAP:
                return toVariantMap() == value.toVariantMap();
            case D_DOUBLE:
                return toDouble() == value.toDouble();
            case D_FLOAT:
                return toFloat() == value.toFloat();
            case D_SHORT:
                return toShort() == value.toShort();
            case D_USHORT:
                return toUShort() == value.toUShort();
            case D_BOOLEAN:
                return toBool() == value.toBool();
            case D_INT:
                return toInt() == value.toInt();
            case D_UINT:
                return toUInt() == value.toUInt();
            case D_LONG:
                return toLong() == value.toLong();
            case D_LONGLONG:
                return toLongLong() == value.toLongLong();
            case D_ULONG:
                return toULong() == value.toULong();
            case D_ULONGLONG:
                return toULongLong() == value.toULongLong();
            case D_NULL:
                return isNull() == value.isNull();
            default:
                return false;
        }
    }

    bool Variant::operator!=(const Variant &value) const {
        return !(*this == value);
    }

    bool Variant::operator>(const Variant &value) const {
        DataType type;

        // By default use our type
        type = this->type;

        // We need to make sure that if a > b, that b is < a. To do this
        // it is necessary to always use the greatest (or least) type -- it need
        // only be consistent
        if (value.type > this->type) {
            type = value.type;
        }

        switch (type) {
            case D_POINTER:
                return toPointer() > value.toPointer();
            case D_STRING:
                return toString() > value.toString();
            case D_STRINGVECTOR:
                return toStringVector() > value.toStringVector();
            case D_VARIANTVECTOR:
                return toVariantVector() > value.toVariantVector();
            case D_VARIANTMAP:
                return toVariantMap() > value.toVariantMap();
            case D_DOUBLE:
                return toDouble() > value.toDouble();
            case D_FLOAT:
                return toFloat() > value.toFloat();
            case D_SHORT:
                return toShort() > value.toShort();
            case D_USHORT:
                return toUShort() > value.toUShort();
            case D_BOOLEAN:
                return toBool() > value.toBool();
            case D_INT:
                return toInt() > value.toInt();
            case D_UINT:
                return toUInt() > value.toUInt();
            case D_LONG:
                return toLong() > value.toLong();
            case D_LONGLONG:
                return toLongLong() > value.toLongLong();
            case D_ULONG:
                return toULong() > value.toULong();
            case D_ULONGLONG:
                return toULongLong() > value.toULongLong();
            case D_NULL:
                return value.isNull();
            default:
                return false;
        }
    }

    bool Variant::operator<(const Variant &value) const {
        DataType type;

        // By default use our type
        type = this->type;

        // We need to make sure that if a > b, that b is < a. To do this
        // it is necessary to always use the greatest (or least) type -- it need
        // only be consistent
        if (value.type > this->type) {
            type = value.type;
        }

        switch (type) {
            case D_POINTER:
                return toPointer() < value.toPointer();
            case D_STRING:
                return toString() < value.toString();
            case D_STRINGVECTOR:
                return toStringVector() < value.toStringVector();
            case D_VARIANTVECTOR:
                return toVariantVector() < value.toVariantVector();
            case D_VARIANTMAP:
                return toVariantMap() < value.toVariantMap();
            case D_DOUBLE:
                return toDouble() < value.toDouble();
            case D_FLOAT:
                return toFloat() < value.toFloat();
            case D_SHORT:
                return toShort() < value.toShort();
            case D_USHORT:
                return toUShort() < value.toUShort();
            case D_BOOLEAN:
                return toBool() < value.toBool();
            case D_INT:
                return toInt() < value.toInt();
            case D_UINT:
                return toUInt() < value.toUInt();
            case D_LONG:
                return toLong() < value.toLong();
            case D_LONGLONG:
                return toLongLong() < value.toLongLong();
            case D_ULONG:
                return toULong() < value.toULong();
            case D_ULONGLONG:
                return toULongLong() < value.toULongLong();
            case D_NULL:
                return value.isNull();
            default:
                return false;
        }
    }

    bool Variant::operator>=(const Variant &value) const {
        return (*this == value || (*this) > value);
    }

    bool Variant::operator<=(const Variant &value) const {
        return (*this == value || (*this) < value);
    }

    const DataType Variant::getType() const {
        return type;
    }

    Variant::~Variant() {
        if (deleteData) {
            deinit();
        }
    }

    template<typename T>
    T Variant::numericCast() const {
        switch (type) {
            case D_STRING:
            case D_STRINGVECTOR:
            case D_VARIANTVECTOR: {
                T number;
                std::istringstream stream(toString());
                stream >> number;
                return number;
            }
            case D_DOUBLE:
                return *static_cast<double *>(data);
            case D_FLOAT:
                return *static_cast<float *>(data);
            case D_SHORT:
                return *static_cast<short *>(data);
            case D_USHORT:
                return *static_cast<unsigned short *>(data);
            case D_INT:
                return *static_cast<int *>(data);
            case D_UINT:
                return *static_cast<unsigned int *>(data);
            case D_LONG:
                return *static_cast<long *>(data);
            case D_ULONG:
                return *static_cast<unsigned long *>(data);
            case D_NULL:
            case D_POINTER:
            default:
                return 0;
        }
    }

    void Variant::deinit() {
        switch (type) {
            case D_NULL:
                break;
            case D_POINTER:
                delete reinterpret_cast<ArbitraryPointer *>(data);
                break;
            case D_STRING:
                delete reinterpret_cast<std::string *>(data);
                break;
            case D_STRINGVECTOR:
                delete reinterpret_cast<std::vector<std::string> *>(data);
                break;
            case D_VARIANTVECTOR:
                delete reinterpret_cast<VariantVector *>(data);
                break;
            case D_VARIANTMAP:
                delete reinterpret_cast<VariantMap *>(data);
                break;
            case D_LONG:
                delete reinterpret_cast<long *>(data);
                break;
            case D_LONGLONG:
                delete reinterpret_cast<long long *>(data);
                break;
            case D_ULONG:
                delete reinterpret_cast<unsigned long *>(data);
                break;
            case D_ULONGLONG:
                delete reinterpret_cast<unsigned long long *>(data);
                break;
            case D_INT:
                delete reinterpret_cast<int *>(data);
                break;
            case D_UINT:
                delete reinterpret_cast<unsigned int *>(data);
                break;
            case D_SHORT:
                delete reinterpret_cast<short *>(data);
                break;
            case D_USHORT:
                delete reinterpret_cast<unsigned short *>(data);
                break;
            case D_BOOLEAN:
                delete reinterpret_cast<bool *>(data);
                break;
            case D_DOUBLE:
                delete reinterpret_cast<double *>(data);
                break;
            case D_FLOAT:
                delete reinterpret_cast<float *>(data);
                break;
        }
    }
}

