#ifndef CvMediaFactoryPlus_h__
#define CvMediaFactoryPlus_h__


#include "CvInclude.h"

namespace cvplus
{

    class MediaFactory
    {
    public:
        static MediaFactory & instance();

        inline cvplus::NamedMats & images() { return m_images; }
        inline cvplus::NamedMats & sampleImages() { return m_sampleImages; }
        inline cvplus::NamedCaptures & captures() { return m_captures; }
        std::string getFileNameForSave(const std::string & postfix = "none") const;

    private:
        MediaFactory();
        MediaFactory(const MediaFactory & other) = delete;

        void readSettings();

        MediaFactory & operator=(const MediaFactory & other) = delete;

        void loadImagesTo(std::vector<std::string> fileNames, cvplus::NamedMats & matPairs);
        void loadCapturesTo(std::vector<std::string> fileNames, cvplus::NamedCaptures & captures);
        std::vector<std::string> getLinesFrom(const std::string & fileName);

        template<typename T>
        void printReport(std::string varName, std::vector< std::pair<std::string, T> > pairs)
        {
            unsigned index = 0;
            for (auto & p : pairs)
            {
                std::string name = p.first;

                std::ostringstream os;
                os
                    << varName << "["
                    << std::setw(2) << index << "] "
                    << name
                    << std::endl;

                index++;

                std::cout << os.str();
            }
        }

        typedef std::map<std::string, std::string> NamedSettings;

        NamedSettings         m_settings;
        cvplus::NamedMats     m_images;
        cvplus::NamedMats     m_sampleImages;
        cvplus::NamedCaptures m_captures;
        std::string           m_pathToSave;
    };


    inline MediaFactory & MediaFactory::instance()
    {
        static MediaFactory mf;
        return mf;
    }

    inline std::string MediaFactory::getFileNameForSave(const std::string & postfix) const
    {
        enum { minCounter = 100, maxCounter = 999 };

        static int index = minCounter;
        static std::string runString = stdplus::dateTimeStr();

        std::string folderName =
            m_pathToSave + "\\" +
            runString + "\\";

        stdplus::createFolder(folderName);

        std::string fullName = folderName +
            stdplus::timeStr() + " " + stdplus::to_string(index) + " " + postfix;

        if (++index > maxCounter)
            index = minCounter;

        return fullName;
    }

    inline std::vector<std::string> MediaFactory::getLinesFrom(const std::string & fileName)
    {
        std::ifstream ist(fileName);

        AEXCEPT_IF(!ist.good());

        std::vector<std::string> names;
        const std::string commentBegin("//");

        while (!ist.eof())
        {
            std::string line;
            getline(ist, line);

            std::size_t found = line.find(commentBegin);
            if (found != std::string::npos)
                line.erase(found, line.length() - found);

            if (line.empty())
                continue;

            while (line.back() == '\\')
                line = line.substr(0, line.size() - 1);

            names.push_back(line);
        }

        return names;
    }

    inline MediaFactory::MediaFactory()
    {
        readSettings();

        loadImagesTo(getLinesFrom(m_settings.at("imageNames")), m_images);
        loadImagesTo(getLinesFrom(m_settings.at("sampleImageNames")), m_sampleImages);
        loadCapturesTo(getLinesFrom(m_settings.at("captureNames")), m_captures);
        m_pathToSave = m_settings.at("pathToSave");

        printReport("m_anyImages", m_images);
        printReport("m_sampleImages", m_sampleImages);
        printReport("m_captures", m_captures);
        AVAR(m_pathToSave);
        ASPLIT;
    }

    inline void MediaFactory::readSettings()
    {
        std::ifstream ist("g:\\Programming\\Soft\\2017-03-02 2000 bus-info-system\\msvc\\14\\AlternativeSolution\\settings.ini");

        AEXCEPT_IF(!ist.good());

        std::string line;

        while (getline(ist, line))
        {
            if (stdplus::trim(line).empty())
                continue;

            std::vector <std::string> nameAndValue = stdplus::split(line, '=');
            AEXCEPT_IF(nameAndValue.size() != 2);

            std::string name = stdplus::trim(nameAndValue.front());
            std::string val = stdplus::trim(nameAndValue.back());

            m_settings[name] = val;
        };
    }

    inline void MediaFactory::loadImagesTo(
        std::vector<std::string> fileNames, cvplus::NamedMats & matPairs)
    {
        int counter = 0;
        for (auto & fn : fileNames)
        {
            if (!stdplus::file_exist(fn))
            {
                AMSG("Error loaded image: " + fn);
                continue;
            }

            cv::Mat img = cv::imread(fn);
            matPairs.push_back(make_pair(fn, img));
        }

        AMSG("Loaded images: "
            + stdplus::to_string(matPairs.size()) + " succeeded, "
            + stdplus::to_string(fileNames.size() - matPairs.size()) + " failed"
        );
    }

    inline void MediaFactory::loadCapturesTo(
        std::vector<std::string> fileNames, cvplus::NamedCaptures & captures)
    {
        int counter = 0;
        for (auto & fn : fileNames)
        {
            if (!stdplus::file_exist(fn))
            {
                AMSG("Error load capture: " + fn);
                continue;
            }

            cv::VideoCapture vc = cv::VideoCapture(fn);
            captures.push_back(make_pair(fn, vc));
        }

        AMSG("Loaded captures: "
            + stdplus::to_string(captures.size()) + " succeeded, "
            + stdplus::to_string(fileNames.size() - captures.size()) + " failed"
        );

        ASPLIT;
    }

}
#endif // CvMediaFactoryPlus_h__
