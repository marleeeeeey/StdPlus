#ifndef CvInclude_h__
#define CvInclude_h__

// latest version from https://github.com/marleeeeeey/StdPlus
#include "StdPlus/StdPlus.h"

#include <opencv2\opencv.hpp>

namespace cvplus
{
    typedef std::pair<std::string, cv::Mat> NamedMat;
    typedef std::vector< NamedMat > NamedMats;

    typedef std::pair<std::string, cv::VideoCapture> NamedCapture;
    typedef std::vector< NamedCapture > NamedCaptures;

    typedef cv::Vec4f Line;
    typedef std::vector<Line> Lines;

    typedef std::vector<cv::Point> Contour;
    typedef std::vector<Contour> Contours;

    typedef cv::Mat Hist;
    typedef std::vector<cv::Vec4i> Hierarchy;
}

#endif // CvInclude_h__
