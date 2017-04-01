#include "StdPlus.h"

struct Point
{
    double x;
    double y;
};

Point operator-(const Point & p)
{
    Point ret;
    ret.x = -p.x;
    ret.y = -p.y;
    return ret;
}

Point operator+(const Point & p1, const Point & p2)
{
    Point ret;
    ret.x = p1.x + p2.x;
    ret.y = p1.y + p2.y;
    return ret;
}

Point operator-(const Point & p1, const Point & p2)
{
    Point ret = p1 + (-p2);
    return ret;
}

Point operator/(const Point & p1, const double & div)
{
    Point ret;
    ret.x = p1.x / div;
    ret.y = p1.y / div;
    return ret;
}

double derivative(Point p1, Point p2)
{
    Point delta = p2 - p1;
    if (delta.x == 0)
    {
        delta.x = 0.0000001;
        AMSG("WARNING in " + AFUNSIG);
    }

    return delta.y / delta.x;
}

double derivative(Point p1, Point p2, Point p3)
{
    Point p23 = (p2 + p3) / 2;
    return derivative(p1, p23);
}

std::vector<double> derivative(const std::vector<double> & inVec)
{
    std::vector<double> outVec;

    if (inVec.empty()) return outVec;

    std::vector<double> extVec;
    extVec.push_back(inVec.front());    // + 1
    extVec.insert(extVec.end(), inVec.begin(), inVec.end());
    extVec.push_back(inVec.back());     // + 1 --> 1+1 = 2

    int index = 0;
    for (auto it = extVec.begin(); it < extVec.end() - 2; it++)
    {
        Point p1{ index + 0, *(it + 0) };
        Point p2{ index + 1, *(it + 1) };
        Point p3{ index + 2, *(it + 2) };
        outVec.push_back(derivative(p1, p2, p3));
    }

    return outVec;
}

std::ostream & operator<<(std::ostream & os, const Point & p)
{
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}

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

//     Point p1{ 0, 0 };
//     Point p2{ 3, 4 };
//     Point p3{ 9, 0 };
//     AVAR(p1);
//     AVAR(p2);
//     AVAR(p3);
// 
//     double der2 = derivative(p1, p2);
//     AVAR(der2);
// 
//     double der3 = derivative(p1, p2, p3);
//     AVAR(der3);

    std::vector<double> derVec = derivative(genVars);
    stdplus::appendVecToFile("test.txt", derVec, "derVec");    

    APAUSE;
}
