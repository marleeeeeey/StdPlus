#include "StdPlus.h"

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

int main()
{
    enum { e_vectorSize = 50 };

    std::vector<double> values(e_vectorSize);
    std::iota(values.begin(), values.end(), -e_vectorSize / 2);

    stdplus::appendVecToFile("test.txt", values, "before");

    for_each(values.begin(), values.end(), 
        [](double & d) { d = pow(d, 2); });


    stdplus::appendVecToFile("test.txt", values, "after");


    std::vector<double> derivate = stdplus::derivative(values);

    stdplus::appendVecToFile("test.txt", derivate, "derivate");

    APAUSE;
}
