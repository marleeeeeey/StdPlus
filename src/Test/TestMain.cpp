#include "OopSocket/OopSocket.h"
#include "gtest/gtest.h"
#include "StdPlus/StdPlus.h"

#include "MathTest.hpp"
#include "CmdParserTest.hpp"
#include "EarthTest.hpp"
#include "WindowsMBCSTest.hpp"
#include "FileTest.hpp"
#include "SocketTest.hpp"


int main(int argc, char **argv)
{
    LOG_STD_REGISTER_STREAM(std::cout);

    ::testing::InitGoogleTest(&argc, argv);
    int googleTestResult = RUN_ALL_TESTS();
    AVAR(googleTestResult);
    APAUSE;
    return googleTestResult;
}
