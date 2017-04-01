#include "StdPlus.h"

int main()
{

    std::vector<double> genVars(50);

    std::generate(genVars.begin(), genVars.end(),
        []() 
    {
        static double prevVal = 0;
        prevVal += stdplus::getRandom<double>(-1, 1);
        return prevVal;
    });

    stdplus::appendVecToFile("test.txt", genVars, "genVars");
    
    std::vector<double> derVec = stdplus::derivative(genVars);
    stdplus::appendVecToFile("test.txt", derVec, "derVec");    

    APAUSE;
}
