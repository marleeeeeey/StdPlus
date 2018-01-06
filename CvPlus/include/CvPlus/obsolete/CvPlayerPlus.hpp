#ifndef CvPlayerPlus_h__
#define CvPlayerPlus_h__

#include "CvInclude.h"
#include "CvSettingWindowPlus.hpp"
#include "CvImageViewerPlus.hpp"

namespace cvplus
{
    class Video
    {
    public:

        inline void setVideoCapture(cv::VideoCapture & videoCapture) { m_videoCapture = videoCapture; }
        inline void setPos(int pos) { m_videoCapture.set(cv::CAP_PROP_POS_FRAMES, pos); }

        inline int frameCount()  const { return (int)m_videoCapture.get(cv::CAP_PROP_FRAME_COUNT); }
        inline int frameWidth()  const { return (int)m_videoCapture.get(cv::CAP_PROP_FRAME_WIDTH); }
        inline int frameHeight() const { return (int)m_videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT); }
        inline int currentPos()  const { return (int)m_videoCapture.get(cv::CAP_PROP_POS_FRAMES); }

        inline cv::VideoCapture & videoCapture() { return m_videoCapture; }

    private:
        cv::VideoCapture m_videoCapture;
    };
}

    std::ostream & operator<<(std::ostream & os, const cvplus::Video & s)
    {
        os
            << s.frameCount() << "x["
            << s.frameWidth() << ", "
            << s.frameHeight() << "]"
            ;

        return os;
    }

namespace cvplus
{
    class Player : ObjectWithSettings
    {
    public:

        typedef void(*CbFrame)(const cv::Mat &, void *);

        Player()
        {

        }

        ~Player()
        {
            destroyWindows();
        }

        void setImages(cvplus::NamedMats & images)
        {
            m_isImageMode = true;
            m_images = images;
            initTrackbar();
        }

        void setVideoCapture(cv::VideoCapture & videoCapture)
        {
            m_isImageMode = false;
            m_video.setVideoCapture(videoCapture);
            initTrackbar();
        }

        void start(bool isShowWindow = true)
        {
            if (!m_isInit)
                init();

            while (true)
            {
                try
                {
                    if (m_isImageMode)
                        readFrameFromImages();
                    else
                        readFrameFromVideo();
                }
                catch (std::exception & e)
                {
                    AMSG(e.what());
                    continue;
                }

                settingsWinManualUpdate();

                if (!m_isPaintMode && m_cbFrame != nullptr)
                {
                    cv::Mat cbImage;

                    if (m_rect.area() != 0)
                    {
                        AVAR(m_rect);
                        cbImage = m_curFrame(m_rect);
                    }
                    else
                    {
                        cbImage = m_curFrame;
                    }

                    m_cbFrame(cbImage, m_userData);
                }

                if (isShowWindow)
                {
                    static cvplus::ImageViewer iw(videoWinName());
                    iw.setMouseCallback(Player::onMouseEvent, this);

                    if (m_rect.area() != 0)
                        cv::rectangle(m_curFrame, m_rect, cvplus::color::green, 5);

                    iw.setImage(m_curFrame);
                }

                if (cv::waitKey(1) == 27)
                    break;
            }
        }

        void setCallbackForFrame(const CbFrame & cb)
        {
            m_cbFrame = cb;
        }

        void setUserData(void * userData) { m_userData = userData; }

    private:

        void readFrameFromImages()
        {
            AEXCEPT_IF(m_images.empty());

            m_images.at(m_imageIndex).second.copyTo(m_curFrame);

            if (!m_isPaintMode)
                m_imageIndex += m_playSpeed;

            if (m_imageIndex >= m_images.size())
                m_imageIndex = 0;

            m_trackbarPos = m_imageIndex;
        }

        void readFrameFromVideo()
        {
            m_video.videoCapture() >> m_curFrame;

            int prevPos = m_video.currentPos() - 1;
            m_video.setPos(prevPos + m_playSpeed);

            if (m_isPaintMode)
            {
                cv::rectangle(m_curFrame, m_tempRect, cvplus::color::green, 5);
                m_video.setPos(prevPos);
            }

            // MARLEEEEEEY FIXME -5
            if (cvplus::imgArea(m_curFrame) == 0
                || m_video.currentPos() >= m_video.frameCount() - 5)
            {
                m_video.setPos(0);
                throw std::logic_error("Video end and reset to begin");
            }

            m_trackbarPos = m_video.currentPos();
        }

        inline std::string videoWinName() { return "Player " + stdplus::to_string(m_index); }

        void init()
        {
            initIndex();
            initWindows();

            m_isInit = true;
        }

        void initIndex()
        {
            static unsigned startIndex = 0;
            m_index = startIndex;
            startIndex++;
        }

        void initWindows()
        {
            //cv::namedWindow(videoWinName(), cv::WINDOW_FREERATIO);
        }

        void destroyWindows()
        {
            cv::destroyWindow(videoWinName());
        }

        void initTrackbar(int maxSpeed = 20)
        {
            int frameCount = 0;

            if (m_isImageMode)
            {
                AMSG("Images mode");
                AEXCEPT_IF(m_images.empty());
                frameCount = m_images.size();
            }
            else
            {
                AMSG("Video mode");
                AEXCEPT_IF(!m_video.videoCapture().isOpened());
                frameCount = m_video.frameCount();
            }

            std::vector<Setting> settings
            {
                { &m_trackbarPos, "trackbar" , 0, frameCount - 1, 0 },
                { &m_playSpeed  , "playSpeed", 0,     maxSpeed, 1 },
            };

            setSettingsWinName("Control for " + videoWinName());
            showSettingsWin(settings);
        }

        virtual void onSettingsUpdate() override
        {
            if (m_isImageMode)
                m_imageIndex = m_trackbarPos;
            else
                m_video.setPos(m_trackbarPos);
        }


        static void onMouseEvent(int event, int x, int y, int flags, void* userData)
        {
            AEXCEPT_IF(userData == nullptr);
            Player & curPlayer = *(Player *)userData;
            curPlayer.onMouseEvent(event, x, y, flags);
        }


        void onMouseEvent(int event, int x, int y, int flags)
        {
            static int prevFlags = flags;

            if (m_isPaintMode)
            {
                int maxX = m_curFrame.cols - 1;
                int maxY = m_curFrame.rows - 1;

                x = (std::min)(x, maxX);
                y = (std::min)(y, maxY);

                m_tempRect.width = x - m_tempRect.x;
                m_tempRect.height = y - m_tempRect.y;
            }

            if (flags == 1 && prevFlags == 0)
            {
                m_tempRect.x = x;
                m_tempRect.y = y;
                m_isPaintMode = true;
            }
            else
                if (flags == 0 && prevFlags == 1)
                {
                    m_isPaintMode = false;
                    onRectUpdate(m_tempRect);
                }

            prevFlags = flags;
        }

        void onRectUpdate(const cv::Rect & newRect)
        {
            if (m_rect.area() != 0)
                m_rect = cv::Rect();
            else
                m_rect = newRect;
        }


        bool      m_isInit = false;
        CbFrame   m_cbFrame = nullptr;
        int       m_trackbarPos = 0;
        int       m_playSpeed = 1;
        unsigned  m_index = 0;
        void *    m_userData = nullptr;
        cv::Mat   m_curFrame;
        Video     m_video;
        bool	  m_isImageMode = false;
        cvplus::NamedMats m_images;
        unsigned  m_imageIndex = 0;

        bool      m_isPaintMode = false;
        cv::Rect  m_rect;
        cv::Rect  m_tempRect;
    };

}


#endif // CvPlayerPlus_h__
