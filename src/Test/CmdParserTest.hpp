#pragma once

#include "gtest/gtest.h"
#include "StdPlus/StdPlus.h"


TEST(CmdParser, ManualCmd)
{
    using CString = char *;
    std::vector<CString> argv =
    {
        "test=5",
        "mama=natasha",
        "papa=gena",
        "isTest=true",
        "isTest1=-1",
        "isTest2=0",
        "isTest3=false",
        "ThisIsIndexesValue",
        "years_old=27",
        "kuka",
    };

    stdplus::CmdParser cmd;
    cmd.parseData(argv.size(), argv.data());

    cmd.print(std::cout);

    ASSERT_TRUE(cmd.getValue<int>("test") == 5);
    ASSERT_TRUE(cmd.getValue<bool>("isTest") == true);
    ASSERT_TRUE(cmd.getValue<bool>("isTest1") == true);
    ASSERT_TRUE(cmd.getValue<bool>("isTest2") == false);
    ASSERT_TRUE(cmd.getValue<bool>("isTest3") == false);
    ASSERT_TRUE(cmd.getValue<std::string>("mama") == "natasha");
    ASSERT_FALSE(cmd.getValue<std::string>("papa") == "wrongName");
    ASSERT_TRUE(cmd.getValue<int>("karamba", 10) == 10);
    ASSERT_TRUE(cmd.indexedValues().at(0) == "ThisIsIndexesValue");
    ASSERT_TRUE(cmd.isExistIdx("kuka") == true);
    ASSERT_FALSE(cmd.isExistIdx("dfsdge") == true);
}


