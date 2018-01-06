#ifndef CvSettingWindowPlus_h__
#define CvSettingWindowPlus_h__

#include "CvInclude.h"

namespace cvplus
{
    template<typename T>
    class Setting_
    {
    public:
        friend class SettingsWindow;

        Setting_(T * pValue, const std::string & name, T min, T max, T initValue = 0)
        {
            m_value = pValue;
            init(name, min, max, initValue);
        }

        T * pValue()
        {
            AEXCEPT_IF(m_value == nullptr);
            return m_value;
        }

        T value() const { return *m_value; }
        void setValue(T val)
        {
            if (val < m_min) val = m_min;
            if (val > m_max) val = m_max;

            *m_value = val;
        }

        void setInitValue(T val) { m_initValue = *m_value = val; }

    private:

        void init(const std::string & name, T min, T max, T initValue)
        {
            AEXCEPT_IF(initValue < min);
            AEXCEPT_IF(initValue > max);

            m_name = name;
            m_min = min;
            m_max = max;
            setInitValue(initValue);
        }


        std::string m_name;
        T      m_min;
        T      m_max;
        T      m_initValue;
        T *    m_value = nullptr;
    };

    typedef Setting_<int> Setting;

    typedef void(*CbSettingsChanged) (const Setting &, void * parent);

    class SettingsWindow
    {
    public:
        SettingsWindow(void * parent = nullptr)
        {
            static unsigned index = 0;
            m_id = index;
            index++;

            setParent(parent);

            //initWindow(); // After all add!!! not uncomment!
        }

        ~SettingsWindow()
        {
            destroyWindow();
        }

        void setParent(void * parent) { m_parent = parent; }

        void add(const Setting & setting)
        {
            AEXCEPT_IF(m_isInited);

            m_settings.push_back(setting);
        }

        void add(const std::vector<Setting> & settings)
        {
            AEXCEPT_IF(m_isInited);

            m_settings.insert(m_settings.end(), settings.begin(), settings.end());
        }

        void start()
        {
            if (!m_isInited)
                initWindow();
        }

        // used call if values changed out this class
        void manualUpdate()
        {
            for_each(m_settings.begin(), m_settings.end(),
                [&](Setting & s)
            {
                cv::setTrackbarPos(s.m_name, m_winName, *s.pValue());
            });
        }

        Setting & getSetting(const std::string & settingName)
        {
            auto it = std::find_if(m_settings.begin(), m_settings.end(),
                [&settingName](const Setting & s)
            {
                return s.m_name == settingName;
            });

            AEXCEPT_IF(it == m_settings.end());

            return *it;
        }

        void setCallback(CbSettingsChanged cb) { m_callback = cb; }

        void setWinName(std::string winName)
        {
            AEXCEPT_IF(m_isInited);
            m_winName = winName;
        }

        void setSaveSettingsToFile(bool b) { m_isSaveSettingsToFile = b; }

    private:
        void initWindow()
        {
            if (m_isSaveSettingsToFile)
                loadFromFile();

            if (m_winName == "")
                m_winName = "Settings Window " + stdplus::to_string(m_id);

            cv::namedWindow(m_winName, cv::WINDOW_KEEPRATIO);

            initAllTrackbars();

            m_isInited = true;
        }

        void destroyWindow()
        {
            m_isInited = false;

            if (m_isSaveSettingsToFile)
                saveToFile();

            cv::destroyWindow(m_winName);
        }

        typedef std::pair<SettingsWindow *, Setting &> CbData;

        void initAllTrackbars()
        {
            for_each(m_settings.begin(), m_settings.end(),
                [&](Setting & s)
            {
                CbData * cbData = new CbData(this, s);

                cv::createTrackbar(s.m_name, m_winName,
                    nullptr, s.m_max, sharedCallback, cbData);

                cv::setTrackbarMin(s.m_name, m_winName, s.m_min);

                cv::setTrackbarPos(s.m_name, m_winName, s.m_initValue);

            });
        }

        static void sharedCallback(int pos, void * pData)
        {
            CbData * pCbData = (CbData *)(pData);
            CbData & cbData = *pCbData;

            SettingsWindow * pThis = cbData.first;
            Setting & setting = cbData.second;

            AERR_IF(setting.pValue() == nullptr);
            if (setting.pValue() == nullptr) return;

            *setting.pValue() = pos;

            if (pThis->m_callback != nullptr)
                pThis->m_callback(setting, pThis->m_parent);
        }


        std::string iniFileName() { return "Options " + m_winName + ".ini"; }

        void saveToFile()
        {
            std::ofstream ost(iniFileName(), std::ios_base::trunc);

            for (auto & s : m_settings)
            {
                ost << s.m_name << m_splitter << s.value() << std::endl;
            }

        }

        void loadFromFile()
        {
            std::ifstream ist(iniFileName());

            std::string line;
            while (getline(ist, line))
            {
                std::vector <std::string> nameAndValue = stdplus::split(line, m_splitter);
                std::string name = nameAndValue.front();
                int val = stdplus::to_int(nameAndValue.back());

                try
                {
                    getSetting(name).setInitValue(val);
                }
                catch (std::exception &)
                {
                    AMSG("Not used param " + name);
                }
            };
        }

        std::vector<Setting>   m_settings;
        unsigned          m_id = 0;
        bool              m_isInited = false;
        CbSettingsChanged m_callback = nullptr;
        void *            m_parent = nullptr;
        std::string            m_winName;
        bool              m_isSaveSettingsToFile = false;
        const char        m_splitter = '=';
    };


    class ObjectWithSettings
    {
    public:

        virtual void onSettingsUpdate() = 0;

        void settingsWinManualUpdate()
        {
            m_settingsWindow.manualUpdate();
        }

        void showSettingsWin(const std::vector<Setting> & settings, std::string settigWinName = "")
        {
            AEXCEPT_IF(m_isSettingsInit);

            m_settingsWindow.setParent(this);
            m_settingsWindow.setCallback(privateUpdate);
            m_settingsWindow.add(settings);
            m_isSettingsInit = true;

            m_settingsWindow.start();
        }

        void setSettingsWinName(std::string winName) { m_settingsWindow.setWinName(winName); }

        void setSaveSettingsToFile(bool b)
        {
            m_settingsWindow.setSaveSettingsToFile(b);
        }

    private:

        static void privateUpdate(const Setting & setting, void * parent)
        {
            AEXCEPT_IF(parent == nullptr);
            ObjectWithSettings * pThis = (ObjectWithSettings *)parent;
            AEXCEPT_IF(!pThis->m_isSettingsInit);
            pThis->onSettingsUpdate();
        }

        bool           m_isSettingsInit = false;
        SettingsWindow m_settingsWindow;
    };

}


#endif // CvSettingWindowPlus_h__
