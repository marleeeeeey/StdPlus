#pragma once


void appendVecToFileTest()
{
    std::vector<double> ds = stdplus::randomGraphic(20, 10);
    std::sort(ds.begin(), ds.end());

    std::vector<int> isol = stdplus::isolatedIndex(ds, 0);

    stdplus::appendVecToFile("Isolated.txt", ds, "ds");
    stdplus::appendVecToFile("Isolated.txt", isol, "isol");
}


