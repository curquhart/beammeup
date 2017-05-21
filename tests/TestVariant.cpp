#include "tests/mocks/MockTransporter.h"
#include "tests/stubs/StubTrackedPointer.h"
#include "include/beammeup/VariantMap.h"
#include "include/beammeup/VariantVector.h"

using ::testing::_;

namespace BeamMeUp {
    class TestVariant : public ::testing::Test {
    protected:
        void SetUp() {
            StubTrackedPointer::count = 0;
        }
    };

    // Tests converting a string to a variant vector
    TEST_F(TestVariant, ConvertStringVariantToVariantVector) {
        Variant value("test");
        ASSERT_EQ(value, value.toVariantVector());
    }

    // Tests converting a string to a string vector
    TEST_F(TestVariant, ConvertStringVariantToStringVector) {
        Variant value("test");
        ASSERT_EQ(value, value.toStringVector());
    }

    // Tests converting a string vector to a variant vector
    TEST_F(TestVariant, ConvertStringVectorToVariantVector) {
        std::vector<std::string> list;
        list.push_back("test");
        Variant value(list);
        ASSERT_EQ(value, value.toVariantVector());
    }

    // Tests converting a variant vector to a string
    TEST_F(TestVariant, ConvertVariantVectorToString) {
        Variant value(VariantVector() << "test 1" << "test 2");
        ASSERT_EQ("test 1, test 2", value.toString());
    }

    // Tests converting a variant vector to a string vector
    TEST_F(TestVariant, ConvertVariantVectorToStringVector) {
        std::vector<std::string> list;
        list.push_back("test");
        Variant value(list);
        ASSERT_EQ(value, value.toStringVector());
    }

    // Tests that the Variant::operator== method works for int-int comparison
    TEST_F(TestVariant, OperatorEQIntInt) {
        Variant v1(124);
        Variant v2(124);
        ASSERT_EQ(v1, v2);
    }

    // Tests that the Variant::operator== method works for int-double comparison
    TEST_F(TestVariant, OperatorEQIntDouble) {
        Variant v1(124);
        Variant v2((double) 124.0);
        ASSERT_EQ(v1, v2);
    }

    // Tests that the Variant::operator== method works for int-float comparison
    TEST_F(TestVariant, OperatorEQIntFloat) {
        Variant v1(124);
        Variant v2((float) 124.0);
        ASSERT_EQ(v1, v2);
    }

    // Tests that the Variant::operator== method works for int-string comparison
    TEST_F(TestVariant, OperatorEQIntString) {
        Variant v1(124);
        Variant v2("124");
        ASSERT_EQ(v1, v2);
    }

    // Tests that the Variant::operator== method works for string-int comparison
    TEST_F(TestVariant, OperatorEQStringInt) {
        Variant v1("124");
        Variant v2(124);
        ASSERT_EQ(v1, v2);
    }

    // Tests that the Variant::operator== method works for float-string comparison
    TEST_F(TestVariant, OperatorEQFloatString) {
        Variant v1((float) 124.0);
        Variant v2("124.00");
        ASSERT_EQ(v1, v2);
    }

    // Tests that the Variant::operator== method works for string-float comparison
    TEST_F(TestVariant, OperatorEQStringFloat) {
        Variant v1("124.00");
        Variant v2((float) 124.0);
        ASSERT_EQ(v1, v2);
    }

    // Tests that the Variant::operator== method works for double-float comparison
    TEST_F(TestVariant, OperatorEQDoubleFloat) {
        Variant v1((double) 124.8);
        Variant v2((float) 124.8);
        ASSERT_EQ(v1, v2);
    }

    // Tests that the Variant::operator== method works for float-double comparison
    TEST_F(TestVariant, OperatorEQFloatDouble) {
        Variant v1((float) 124.8);
        Variant v2((double) 124.8);
        ASSERT_EQ(v1, v2);
    }

    // Tests that the Variant::operator== method works for float-string vector
    // comparison
    TEST_F(TestVariant, OperatorEQFloatVariantVector) {
        Variant v1((float) 124.8);
        Variant v2(VariantVector() << (float) 124.8);
        ASSERT_EQ(v1, v2);
    }

    // Tests that the Variant::operator== method works for float-string vector
    // comparison
    TEST_F(TestVariant, OperatorEQFloatVariantVectorMultiElements) {
        Variant v1((float) 124.8);
        Variant v2(VariantVector() << (float) 124.8 << "1");
        ASSERT_EQ(v1, v2);
    }

    // Tests that the Variant::operator== method works for variant vector-variant
    // vector comparison
    TEST_F(TestVariant, OperatorEQVariantVectorVariantVector) {
        Variant v1(VariantVector() << (float) 124.8 << "1" << (double) 7);
        Variant v2(VariantVector() << (float) 124.8 << "1" << (double) 7);
        ASSERT_EQ(v1, v2);
    }

    // Tests that the Variant::operator== method works for variant vector-variant
    // vector comparison w/ typecasting
    TEST_F(TestVariant, OperatorEQVariantVectorVariantVectorTypeCasting) {
        Variant v1(VariantVector() << (float) 124.8 << "1.0" << (double) 7);
        Variant v2(VariantVector() << (float) 124.8 << 1.0 << (double) 7);
        ASSERT_EQ(v1, v2);

        Variant v3(VariantVector() << (float) 124.8 << "1" << (double) 7);
        Variant v4(VariantVector() << (float) 124.8 << (float) 1.0 << (double) 7);
        ASSERT_EQ(v3, v4);
    }

    // Tests that the Variant::operator== method works for variant vector-string
    // vector comparison
    TEST_F(TestVariant, OperatorEQVariantVectorStringVector) {
        Variant v1(VariantVector() << (float) 124.8 << "1" << (double) 7);
        std::vector<std::string> list;
        list.push_back("124.8");
        list.push_back("1");
        list.push_back("7");
        Variant v2(list);
        ASSERT_EQ(v1, v2);
    }

    // Tests that the Variant::operator== method returns true for variant vector-
    // variant map comparison (no data)
    TEST_F(TestVariant, OperatorEQVariantVectorVariantMap) {
        Variant v1 = VariantVector();
        Variant v2 = VariantMap();
        ASSERT_EQ(v1, v2);
    }

    // Tests that the Variant::operator== method returns true for variant map-
    // variant map comparison (shuffled data)
    TEST_F(TestVariant, OperatorEQShuffledVariantMap) {
        VariantMap m1;
        m1["abc"] = "123";
        m1["def"] = "456";
        m1["ghi"] = "789";
        m1["jkl"] = "012";

        VariantMap m2;
        m2["def"] = "456";
        m2["jkl"] = "012";
        m2["abc"] = "123";
        m2["ghi"] = "789";

        ASSERT_EQ(m1, m2);
    }

    // Tests that the Variant::operator== method works for float-double comparison
    TEST_F(TestVariant, OperatorNEFloatDouble) {
        Variant v1((float) 124.8);
        Variant v2((double) 124.7);
        EXPECT_NE(v1, v2);
    }

    // Tests that the Variant::operator!= method works for variant vector-variant
    // vector comparison w/ typecasting
    TEST_F(TestVariant, OperatorNEVariantVectorVariantVectorTypeCasting) {
        Variant v1(VariantVector() << (float) 7);
        Variant v2(VariantVector() << (double) 7.5);
        EXPECT_NE(v1, v2);
    }

    // Tests that the Variant::operator== method works for string vector-variant
    // vector comparison
    TEST_F(TestVariant, OperatorEQStringVectorVariantVector) {
        std::vector<std::string> list;
        list.push_back("124.8");
        list.push_back("1");
        list.push_back("7");
        Variant v1(list);
        Variant v2(VariantVector() << (float) 124.8 << "1" << (double) 7);
        ASSERT_EQ(v1, v2);
    }

    // Tests that the Variant::operator!= method works for blank string("")-nullptr
    TEST_F(TestVariant, OperatorNEBlankStringnullptr) {
        Variant v1("");
        Variant v2;
        EXPECT_NE(v1, v2);
    }

    // Tests that the Variant::operator!= method works for nullptr-blank int(0)
    TEST_F(TestVariant, OperatorNEnullptrBlankInt) {
        Variant v1;
        Variant v2(0);
        EXPECT_NE(v1, v2);
    }

    // Tests that the Variant::operator!= method returns true for variant vector-
    // variant map comparison (with data)
    TEST_F(TestVariant, OperatorNEVariantVectorVariantMap) {
        {
            Variant v1 = VariantVector() << "123";
            VariantMap map;
            map["123"] = "456";
            Variant v2(map);
            EXPECT_NE(v1, v2);
        }
        {
            Variant v1 = VariantVector() << "123";
            VariantMap map;
            map["456"] = "123";
            Variant v2(map);
            EXPECT_NE(v1, v2);
        }
    }

    // Tests that the Variant::operator> method works for float-double comparison
    TEST_F(TestVariant, OperatorGTFloatFloat) {
        Variant v1((float) 124.09);
        Variant v2((float) 124.08);
        EXPECT_GT(v1, v2);
    }

    // Tests that the Variant::operator> method works for float-double comparison
    TEST_F(TestVariant, OperatorGTFloatDouble) {
        Variant v1((float) 124.09);
        Variant v2((double) 124.08);
        EXPECT_GT(v1, v2);
    }

    // Tests that the Variant::operator> method works for double-float comparison
    TEST_F(TestVariant, OperatorGTDoubleFloat) {
        Variant v1((double) 124.09);
        Variant v2((float) 124.08);
        EXPECT_GT(v1, v2);
    }

    // Tests that the Variant::operator> method works for string-float comparison
    TEST_F(TestVariant, OperatorGTStringFloat) {
        Variant v1("124.09");
        Variant v2((float) 124.08);
        EXPECT_GT(v1, v2);
    }

    // Tests that the Variant::operator> method works for float-string comparison
    TEST_F(TestVariant, OperatorGTFloatString) {
        Variant v1((float) 124.09);
        Variant v2("124.08");
        EXPECT_GT(v1, v2);
    }

    // Tests that the Variant::operator< method works for float-float comparison
    TEST_F(TestVariant, OperatorLTFloatFloatP1) {
        Variant v1((float) 124.8);
        Variant v2((float) 124.9);
        EXPECT_LT(v1, v2);
    }

    // Tests that the Variant::operator< method works for float-double comparison
    TEST_F(TestVariant, OperatorLTFloatFloat) {
        Variant v1((float) 124.08);
        Variant v2((double) 124.09);
        EXPECT_LT(v1, v2);
    }

    // Tests that the Variant::operator< method works for float-double comparison
    TEST_F(TestVariant, OperatorLTFloatDouble) {
        Variant v1((float) 124.08);
        Variant v2((double) 124.09);
        EXPECT_LT(v1, v2);
    }

    // Tests that the Variant::operator< method works for double-float comparison
    TEST_F(TestVariant, OperatorLTDoubleFloat) {
        Variant v1((double) 124.08);
        Variant v2((float) 124.09);
        EXPECT_LT(v1, v2);
    }

    // Tests that the Variant::operator< method works for string-float comparison
    TEST_F(TestVariant, OperatorLTStringFloat) {
        Variant v1("124.08");
        Variant v2((float) 124.09);
        EXPECT_LT(v1, v2);
    }

    // Tests that the Variant::operator< method works for float-string comparison
    TEST_F(TestVariant, OperatorLTFloatString) {
        Variant v1((float) 124.08);
        Variant v2("124.09");
        EXPECT_LT(v1, v2);
    }

    // Tests pointer memory management
    TEST_F(TestVariant, PointerCleanup) {
        MockTransporter mockTransporter;

        EXPECT_CALL(mockTransporter, registerObject(_))
                .Times(5);
        EXPECT_CALL(mockTransporter, unregisterObject(_))
                .Times(5);

        StubTrackedPointer *p1 = new StubTrackedPointer(&mockTransporter);
        StubTrackedPointer *p2 = new StubTrackedPointer(&mockTransporter);
        ArbitraryPointer *p3;

        ASSERT_EQ(2, StubTrackedPointer::count);
        {
            Variant v1(p1, false);
            Variant v2(p1, false);
        }
        ASSERT_EQ(2, StubTrackedPointer::count);
        {
            Variant v1(p1, false);
            Variant v2(v1);
            p3 = v2.toPointer();
        }
        ASSERT_EQ(3, StubTrackedPointer::count);
        delete p3;
        ASSERT_EQ(2, StubTrackedPointer::count);
        {
            Variant v1(p1, true);
            Variant v2(v1);
        }
        ASSERT_EQ(1, StubTrackedPointer::count);
        p1 = new StubTrackedPointer(&mockTransporter);
        {
            Variant v1(p1, true);
            Variant v2(p2, true);
        }
        ASSERT_EQ(0, StubTrackedPointer::count);
    }
}