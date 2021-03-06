#ifndef __GEOMTK_StampString_test__
#define __GEOMTK_StampString_test__

#include "StampString.h"

using namespace geomtk;

class StampStringTest : public ::testing::Test {
protected:
    TimeManager timeManager;
    StampString ss;
    string res;

    void SetUp() {
        timeManager.init("1979-01-01 00000", "2008-12-31 86400", "1 hour");
    }
};

TEST_F(StampStringTest, run) {
    ss.init("test.%Y-%N-%D.nc");
    res = ss.run(timeManager);
    ASSERT_EQ("test.1979-01-01.nc", res);

    ss.init("test.%y-%m-%d.nc");
    res = ss.run(timeManager);
    ASSERT_EQ("test.79-1-1.nc", res);

    ss.init("test.%T.nc");
    res = ss.run(timeManager);
    ASSERT_EQ("test.00000.nc", res);

    ss.init("test.%t.nc");
    res = ss.run(timeManager);
    ASSERT_EQ("test.0.nc", res);

    timeManager.advance(true);
    ss.init("test.%Y-%N-%D_%H:%M:%S.nc");
    res = ss.run(timeManager);
    ASSERT_EQ("test.1979-01-01_01:00:00.nc", res);
}

#endif // __GEOMTK_StampString_test__
