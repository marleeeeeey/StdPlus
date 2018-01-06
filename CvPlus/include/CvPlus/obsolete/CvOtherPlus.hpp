#ifndef CvOtherPlus_h__
#define CvOtherPlus_h__

#include "CvInclude.h"


template<typename T>
bool operator<(const cv::Point_<T> & p1, const cv::Point_<T> & p2)
{
    if (p1.y < p2.y)
        return true;
    else if (p1.y > p2.y)
        return false;

    //y1 == y2
    if (p1.x < p2.x)
        return true;

    return false;
}


namespace cvplus
{
    inline std::string type_of(const cv::Mat & m)
    {
        int type = m.type();
        std::string strType;
        uchar depth = type & CV_MAT_DEPTH_MASK;
        uchar chans = 1 + (type >> CV_CN_SHIFT);
        switch (depth)
        {
        case CV_8U:  strType = "8U"; break;
        case CV_8S:  strType = "8S"; break;
        case CV_16U: strType = "16U"; break;
        case CV_16S: strType = "16S"; break;
        case CV_32S: strType = "32S"; break;
        case CV_32F: strType = "32F"; break;
        case CV_64F: strType = "64F"; break;
        default:     strType = "User"; break;
        }
        strType += "C";
        strType += (chans + '0');
        return strType;
    }

    inline std::vector<cv::Rect> split(const cv::Rect & rect, int cols = 1, int rows = 1)
    {
        double w = (double)rect.width / cols;
        double h = (double)rect.height / rows;
        cv::Point tl = rect.tl();
        std::vector<cv::Rect> result;

        for (int row = 0; row < rows; row++)
        {
            for (int col = 0; col < cols; col++)
            {
                cv::Point newTopLeft = tl + cv::Point(col * w, row * h);
                cv::Rect newRect(newTopLeft, cv::Size(w, h));
                result.push_back(newRect);
            }
        }

        return result;
    }

    inline std::vector<cv::Rect> splitByWidth(const cv::Rect & rect, int w)
    {
        int cols = rect.width / w;
        return cvplus::split(rect, cols, 1);
    }

    inline std::vector<cv::Rect> splitByHeight(const cv::Rect & rect, int h)
    {
        int rows = rect.height / h;
        return cvplus::split(rect, 1, rows);
    }

    inline cv::Mat getMorphElement(int radius = 5)
    {
        int dilation_type = cv::MORPH_ELLIPSE;

        cv::Mat element = cv::getStructuringElement(dilation_type,
            cv::Size(2 * radius + 1, 2 * radius + 1),
            cv::Point(radius, radius));

        return element;
    }

	inline cv::Mat vChannelFromBGR(const cv::Mat & constBgrImage, int channelNum = 2, bool isBgr = false)
    {
		AEXCEPT_IF(channelNum < 0 || channelNum > 2);

        cv::Mat tempImg;

		if (!isBgr)
			cv::cvtColor(constBgrImage, tempImg, cv::COLOR_BGR2HSV);
		else
			tempImg = constBgrImage;

        std::vector<cv::Mat> hsvPlanes;
        cv::split(tempImg, hsvPlanes);
        tempImg = hsvPlanes[channelNum];
        return tempImg;
    }

    inline cv::Mat grayChannelFromBGR(const cv::Mat & constBgrImage)
    {
        cv::Mat tempImg;
        cv::cvtColor(constBgrImage, tempImg, cv::COLOR_BGR2GRAY);
        return tempImg;
    }

    inline double contourCompact(const Contour & c)
    {
        double area = cv::contourArea(c);
        double length = cv::arcLength(c, true);

        return area / (length * length) * 4 * M_PI;
    }

    inline cv::Point contourCenter(const Contour & c)
    {
        cv::Moments mm = cv::moments(c, false);
        cv::Point mc(mm.m10 / mm.m00, mm.m01 / mm.m00);

        return mc;
    }

    inline double imgArea(const cv::Mat & constImg)
    {
        int w = constImg.cols;
        int h = constImg.rows;

        cv::Point p1(0, 0);
        cv::Point p2(w, 0);
        cv::Point p3(w, h);
        cv::Point p4(0, h);

        Contour c{ p1, p2, p3, p4 };

        return cv::contourArea(c);
    }


    namespace color
	{
		cv::RNG rng(12345);

		cv::Scalar random() 
		{
			return cv::Scalar(
				rng.uniform(0, 255),
				rng.uniform(0, 255),
				rng.uniform(0, 255));
		}

        const cv::Scalar white  = cv::Scalar(255, 255, 255, 255);
        const cv::Scalar black  = cv::Scalar(  0,   0,   0, 255);
        const cv::Scalar red    = cv::Scalar(  0,   0, 255, 255);
        const cv::Scalar green  = cv::Scalar(  0, 255,   0, 255);
        const cv::Scalar blue   = cv::Scalar(255,   0,   0, 255);
        const cv::Scalar yellow = cv::Scalar(  0, 255, 255, 255);
    }

	inline void drawGrid(cv::Mat & bgrImage, int cols, int rows)
	{
		const int lineWidht    = 2;
		const int minX_px      = 0;
		const int maxX_px      = bgrImage.cols;
		const int minY_px      = 0;
		const int maxY_px      = bgrImage.rows;
		const cv::Scalar color = cvplus::color::black;

		float stepYLine_px = (float)(maxY_px - minY_px) / cols;
		for (float yFixLine_px = minY_px;
			yFixLine_px <= maxY_px; yFixLine_px += stepYLine_px)
		{
			cv::line(bgrImage,
				cv::Point(minX_px, yFixLine_px),
				cv::Point(maxX_px, yFixLine_px),
				color, lineWidht);
		}

		float stepXLine_px = (float)(maxX_px - minX_px) / rows;
		for (float xFixLine_px = minX_px;
			xFixLine_px <= maxX_px; xFixLine_px += stepXLine_px)
		{
			cv::line(bgrImage,
				cv::Point(xFixLine_px, minY_px),
				cv::Point(xFixLine_px, maxY_px),
				color, lineWidht);
		}

	}

    inline void drawGraphics(cv::Mat & bgrImage,
        const std::vector<double> & vars, int minY_unit, int maxY_unit,
		const cv::Scalar & color = cvplus::color::green)
    {
		const int	lineWidht		 = 4;
        const int   imgWidht_px      = bgrImage.cols;
        const int   imgHeight_px     = bgrImage.rows;
        const int   minX_unit        = 0;
        const int   maxX_unit        = vars.size() - 1;
        const float stepX_px_in_unit = (float)imgWidht_px  / (maxX_unit - minX_unit);
        const float stepY_px_in_unit = (float)imgHeight_px / (maxY_unit - minY_unit);
        const int   minX_px          = minX_unit * stepX_px_in_unit;
        const int   maxX_px          = imgWidht_px;
        const int   minY_px          = minY_unit * stepY_px_in_unit;
        const int   maxY_px          = imgHeight_px;
				
        cv::line(bgrImage, 
            cv::Point(minX_px, maxY_px + minY_unit * stepY_px_in_unit),
            cv::Point(maxX_px, maxY_px + minY_unit * stepY_px_in_unit),
			color, lineWidht);

        for (int xIndex = 1; xIndex <= maxX_unit; xIndex++)
        {
            int curX_px  = (xIndex - 1) * stepX_px_in_unit;
            int nextX_px = (xIndex - 0) * stepX_px_in_unit;
            int curY_px  = (vars[xIndex - 1] - minY_unit) * stepY_px_in_unit;
            int nextY_px = (vars[xIndex - 0] - minY_unit) * stepY_px_in_unit;

            cv::Point p1(minX_px + curX_px,  maxY_px - curY_px);
            cv::Point p2(minX_px + nextX_px, maxY_px - nextY_px);

            cv::line(bgrImage, p1, p2, color, lineWidht);
        }
    }

    inline Contours contoursConvex(Contours cs)
    {
        Contours res(cs.size());
        for (int i = 0; i < cs.size(); ++i)
            cv::convexHull(cs[i], res[i], false);

        return res;
    }


	void contoursFromRoiToFull(Contours & cs, const cv::Rect & roiRect)
	{
		int diffX = roiRect.x;
		int diffY = roiRect.y;

		for (auto & c : cs)
		{
			for (auto & p : c)
			{
				p.x += diffX;
				p.y += diffY;
			}
		}
	}


    class PredFilterContourByArea
    {
    public:
        PredFilterContourByArea(double minArea = -1, double maxArea = -1)
        {
            m_minArea = minArea;
            m_maxArea = maxArea;
        }

        bool operator()(const Contour & c)
        {
            double area = cv::contourArea(c);

            if (m_minArea >= 0 && area < m_minArea)
                return true;

            if (m_maxArea >= 0 && area > m_maxArea)
                return true;

            return false;
        }

    private:
        double m_minArea = -1;
        double m_maxArea = -1;
    };

    class PredFilterContourByCompact
    {
    public:
        PredFilterContourByCompact(double minCompact = -1, double maxCompact = -1)
        {
            m_minCompact = minCompact;
            m_maxCompact = maxCompact;
        }

        bool operator()(const Contour & c)
        {
            double contourCompact = cvplus::contourCompact(c);

            if (m_minCompact >= 0 && contourCompact > m_minCompact)
                return true;

            if (m_maxCompact >= 0 && contourCompact < m_maxCompact)
                return true;

            return false;
        }

    private:
        double m_minCompact = -1;
        double m_maxCompact = -1;
    };

    class PredFilterContourByAreaRatio
    {
    public:
        PredFilterContourByAreaRatio(const cv::Mat & constImg, double minArea = -1, double maxArea = -1)
        {
            m_minAreaRatio = minArea;
            m_maxAreaRatio = maxArea;
            m_imgArea = cvplus::imgArea(constImg);
            AEXCEPT_IF(m_imgArea <= 0);
        }

        bool operator()(const Contour & c)
        {
            if (m_imgArea <= 0)
                return false;

            double contourArea = cv::contourArea(c);
            double areaRatio = contourArea / m_imgArea;

            if (m_minAreaRatio >= 0 && areaRatio < m_minAreaRatio)
                return true;

            if (m_maxAreaRatio >= 0 && areaRatio > m_maxAreaRatio)
                return true;

            return false;
        }

    private:
        double m_minAreaRatio = -1;
        double m_maxAreaRatio = -1;
        double m_imgArea = -1;
    };

    inline bool predSortContourByCenter(const Contour & left, const Contour & right)
    {
        auto centerLeft = contourCenter(left);
        auto centerRight = contourCenter(right);
        return centerLeft < centerRight;
    }



    cv::Rect boundingRect(const cv::Mat & constImg)
    {
        cv::Size s = constImg.size();
        return cv::Rect(0, 0, s.width, s.height);
    }

    std::vector<cv::Point2f> easyGoodPoints(const cv::Mat & oneChannel, int maxCorners = 1)
    {
        double qualityLevel       = 0.01;
        double minDistance        = 10;
        int    blockSize          = 3;
        bool   useHarrisDetector  = false;
        double k                  = 0.04;


        std::vector<cv::Point2f> corners;

        goodFeaturesToTrack(
            oneChannel,
            corners,
            maxCorners,
            qualityLevel,
            minDistance,
            cv::Mat(),
            blockSize,
            useHarrisDetector,
            k);

        return corners;
    }

    void drawPoints(const cv::Mat & img, std::vector<cv::Point2f> ps)
    {
        int drawPointsRadius = 4;
        ABIND_INT(drawPointsRadius, 4);
        for (int i = 0; i < ps.size(); i++)
        {
            circle(img, ps[i], drawPointsRadius,
                cvplus::color::yellow, CV_FILLED, 8, 0);
        }
    }


    cv::Point center(cv::Rect rect)
    {
        int x = rect.x;
        int y = rect.y;

        x += rect.width / 2;
        y += rect.height / 2;

        return cv::Point(x, y);
    }

    void easyPutText(cv::Mat & img, const std::string & text, cv::Point org,
        cv::Scalar color = cvplus::color::black, double fontScale = 2, int thickness = 3)
    {
        int fontFace = cv::FONT_HERSHEY_SIMPLEX;
        cv::putText(img, text, org, cv::FONT_HERSHEY_SIMPLEX, fontScale, color, thickness);
    }


}

#endif // CvOtherPlus_h__
