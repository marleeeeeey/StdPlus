#ifndef CvImageViewerPlus_h__
#define CvImageViewerPlus_h__

#include "CvInclude.h"
#include "StdPlus/OtherPlus.hpp"        //thisThreadIdString

namespace cvplus
{

    class ImageViewer
    {
    public:
        ImageViewer(std::string friendlyName, const cv::Mat & img)
        {
            init(friendlyName);
            setImage(img);
        }

        ImageViewer(std::string friendlyName = std::string())
        {
            init(friendlyName);
        }

        void setFriendlyName(std::string winName)
        {
            AEXCEPT_IF(m_isInitWindow);
            m_friendlyName = winName;
        }

        std::string name()
        {
            if (m_friendlyName.empty())
                return m_friendlyName + " ImageViewer " + stdplus::to_string(m_index);
            else
                return m_friendlyName;
        }

        void setImage(const cv::Mat & img)
        {
            if (!m_isInitWindow)
                initWindow();

            cv::imshow(name(), img);
            cv::waitKey(1);
        }

        void setMouseCallback(cv::MouseCallback cb, void * userdata)
        {
            m_cbMouse = cb;
            m_cbUserData = userdata;
        }


    private:

        void init(std::string friendlyName)
        {
            static unsigned index = 0;
            m_index = index;
            index++;

            m_friendlyName = friendlyName;
        }

        void initWindow()
        {
            cv::namedWindow(name(), cv::WINDOW_KEEPRATIO);
            cv::setMouseCallback(name(), m_cbMouse, m_cbUserData);
            m_isInitWindow = true;
        }

        void destroyWindow()
        {
            cv::destroyWindow(name());
        }


        std::string       m_friendlyName;
        unsigned          m_index = 0;
        bool              m_isInitWindow = false;
        cv::MouseCallback m_cbMouse = nullptr;
        void *            m_cbUserData = nullptr;

    };

}

#define AVIEW(img)	cvplus::ImageViewer(#img + stdplus::thisThreadIdString(), img)


#endif // CvImageViewerPlus_h__