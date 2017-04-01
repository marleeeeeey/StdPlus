#include "StdPlus.h"

int main()
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
    APAUSE;
}
