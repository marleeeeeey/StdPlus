#pragma once
#include "StdPlus/StdPlus.h"

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


