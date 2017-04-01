#include "StdPlus.h"

int main()
{
    AVAR(stdplus::dateTimeStr());
    
    std::vector<double> ints(100);

    std::generate(ints.begin(), ints.end(),
        []()
    {
        return stdplus::getRandom<double>(10, 90);
    });

    stdplus::appendVecToFile("test.txt", ints, "Vars");

    APAUSE;
}
