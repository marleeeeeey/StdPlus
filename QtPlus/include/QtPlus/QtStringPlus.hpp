/*!
 * \author Tyulenev Sergey
 * Contact: marleeeeeey@gmail.com
 * last version https://github.com/marleeeeeey/QtPlus
*/

#pragma once

#include <QDebug>
//#include <consoleapi.h>	// AllocConsole();

namespace qtplus
{    
    inline void qtConsoleOn(bool isInputToo = false)
    {
        AllocConsole();
    
        freopen("CONOUT$", "wt", stdout);
        freopen("CONOUT$", "wt", stderr);

        if (isInputToo)
        {
            freopen("CONIN$", "r", stdin);
        }
    }    

    template<class T>
    QString to_qstring(const T & p)
    {
        QString strBuf;
        QDebug dout(&strBuf);
        dout << p;
        return strBuf;
    }


    template<class T>
    std::string to_string(const T & p)
    {
        return to_qstring(p).toStdString();
    }

    inline unsigned int logCnt()
    {
#ifdef MacrosPlus_h__
        return stdplus::logCnt();
#else
        return 100;
#endif // MacrosPlus_h__
    }
}

#define QVAR(var)  qDebug() << qtplus::logCnt() << "VAR" << #var << "=" << (var) 


