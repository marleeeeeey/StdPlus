#pragma once

#include <string>
#include <opencv2\opencv.hpp>

namespace cvplus
{
    inline void showMixImage(const cv::Mat & first, const cv::Mat & second, const std::string & name)
    {
        cv::Mat temp;
        temp.copySize(second);
        cv::addWeighted(first, 0.5, second, 0.5, 0, temp);
        cv::imshow(name, temp);
        cv::waitKey(1);
    }

    inline void showImage(const cv::Mat & img, const std::string & name)
    {
//         std::string fileName = stdplus::dateTimeStr();
//         static int counter = 100;
//         counter++;
//         fileName += " " + std::to_string(counter) + " " + name;
//         fileName.replace()
//         cv::imwrite(fileName, img);
//         return;

        cv::imshow(name, img);
        cv::waitKey(1);
    }

    template <typename TPoint>
    inline void drawPoints(const cv::Mat & img, 
        std::vector<TPoint> & points, const std::string & imgName)
    {
        cv::Mat tempImg;
        img.copyTo(tempImg);

        const int RADIUS = 2;
        const cv::Scalar COLOR = cv::Scalar(0, 0, 255, 0);
        const cv::Scalar COLOR_2 = cv::Scalar(0, 255, 255, 0);
        const int THICKNESS = 2;
        const int FONT_FACE = cv::FONT_HERSHEY_SIMPLEX;
        const float FONT_SCALE = 1;


        int index = 0;
        for (auto & p : points)
        {
            cv::putText(tempImg, std::to_string(index), p, 
                FONT_FACE, FONT_SCALE, COLOR, THICKNESS);
            cv::circle(tempImg, p, RADIUS, COLOR, THICKNESS);
            index++;
        }

        cv::Point bl(0, tempImg.rows);

        cv::putText(tempImg, std::to_string(points.size()),
            bl, FONT_FACE, FONT_SCALE, COLOR_2, THICKNESS);

        showImage(tempImg, imgName);
    }

}

#define LOG_CV_SHOW_MIX(first, second)  cvplus::showMixImage(first, second, std::string("MIX: ") + #first + ", " + #second)
#define LOG_CV_SHOW_IMG(var)            cvplus::showImage(var, #var)
#define LOG_CV_DRAW_POINTS(img, points) cvplus::drawPoints(img, points, std::string("DRAW_POINTS: ") + #img + ", " + #points)