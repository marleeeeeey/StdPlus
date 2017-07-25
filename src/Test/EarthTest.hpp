#pragma once

#include "gtest/gtest.h"
#include "StdPlus/StdPlus.h"

TEST(EarthPoint, MunisAndPlusEquale)
{
    using namespace stdplus;

    EarthPoint p1{ deg2rad(55.696727), deg2rad(37.896046) };
    EarthPoint p2{ deg2rad(55.726996), deg2rad(37.918706) };

    // MARLEEEEEEY COMPILE ERROR 
    // AVAR(p1);

    EarthDirection ed = getEarthDirection(p1, p2);
    EarthPoint p2_new = p1 + ed;

    ASSERT_TRUE(p2 == p2_new);
    ASSERT_FALSE(p2 == p1);
}


