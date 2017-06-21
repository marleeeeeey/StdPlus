#include "StdPlus/StdPlus.h"
#include "gtest/gtest.h"

void derivativeTest()
{
    std::vector<double> graphic(30);

    std::generate(graphic.begin(), graphic.end(),
        []()
    {
        static double prevVal = 0;
        prevVal += stdplus::getRandom<double>(-0.5, 0.5);
        return prevVal;
    });

    std::vector<double> derivate = stdplus::derivative(graphic);

    stdplus::appendVecToFile("test.txt", graphic, "graphic");
    stdplus::appendVecToFile("test.txt", derivate, "derivate");
}

void sinTest()
{
    enum { e_vectorSize = 200 };

    std::vector<double> values(e_vectorSize);
    std::iota(values.begin(), values.end(), -e_vectorSize / 2);

    stdplus::appendVecToFile("test.txt", values, "before");

    for_each(values.begin(), values.end(),
        [](double & d) { d = sin(d / 10); });


    stdplus::appendVecToFile("test.txt", values, "after");


    std::vector<double> derivate = stdplus::derivative(values);

    stdplus::appendVecToFile("test.txt", derivate, "derivate");
}

void evenlyTest()
{
	std::vector<int> ints =
	{
		10, 20, 40, 23,5,1,2,35,1,65,3,7542,7457,2
	};

	AVAR(ints);

	ints = stdplus::evenly(ints, 2);

	AVAR(ints);
}

void meanTest()
{
	std::vector<double> rg = stdplus::randomGraphic(100);
	double mean = stdplus::mean(rg);
	std::vector<double> m(rg.size(), mean);
	double meanAbs = stdplus::meanAbs(rg);
	std::vector<double> mAbs(rg.size(), meanAbs);
	stdplus::appendVecToFile("test.txt", rg, "Graphic");
	stdplus::appendVecToFile("test.txt", m, "Mean");
	stdplus::appendVecToFile("test.txt", mAbs, "MeanAbs");
}

void appendVecToFileTest()
{
    std::vector<double> ds = stdplus::randomGraphic(20, 10);
    std::sort(ds.begin(), ds.end());

    std::vector<int> isol = stdplus::isolatedIndex(ds, 0);

    stdplus::appendVecToFile("Isolated.txt", ds, "ds");
    stdplus::appendVecToFile("Isolated.txt", isol, "isol");
}

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

TEST(WindowsMBCS, GetSubFolders)
{
    std::vector<std::string> files = stdplus::getSubFiles();
    AVAR(files);

    std::vector<std::string> folders = stdplus::getSubFolders();
    AVAR(folders);    

    std::vector<std::string> subFiles = stdplus::getSubFilesIncludeSubfolders(".", "*rit*");
    AVAR(subFiles);
}

TEST(SimpleCmdParser, ManualCmd)
{
    using CString = char *;
    std::vector<CString> argv=
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

    stdplus::SimpleCmdParser cmd;
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

int main(int argc, char **argv)
{
    LOG_STD_REGISTER_STREAM(std::cout);

    ::testing::InitGoogleTest(&argc, argv);
    int googleTestResult = RUN_ALL_TESTS();
    AVAR(googleTestResult);
    APAUSE;
    return googleTestResult;
}
