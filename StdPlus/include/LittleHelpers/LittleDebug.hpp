
// ************************************************************************
#include <iostream>
#include <thread>
#include <iomanip>
#include <string>
#define ADEBUG_ON
#define ATHREAD "[" << std::setw(5) << std::this_thread::get_id() << "] "
class LiveWatcher
{
    const std::string m_name = nullptr;
public:
    LiveWatcher(const std::string & name) : m_name(name)
    {
        std::cout << ATHREAD << " START  " << m_name << std::endl;
    }
    ~LiveWatcher()
    {
        std::cout << ATHREAD << " FINISH " << m_name << std::endl;
    }
};
#ifdef ADEBUG_ON
#  define AFUN LiveWatcher ___liveWatcher(std::string("FUN ") + std::string(__PRETTY_FUNCTION__))
#  define AOBJ \
    std::ostringstream ___oss; \
    ___oss << "OBJ (this=" << this << ") " << __PRETTY_FUNCTION__; \
    LiveWatcher ___liveWatcher(___oss.str()) 
#  define AVAR(var) std::cout << ATHREAD << "VAR " << #var << "=" << var << std::endl
#  define AMSG(var) std::cout << ATHREAD << "MSG " << var << std::endl
#  define ATHIS     std::cout << ATHREAD << "THIS=" << this << std::endl
#else
#  define AFUN  
#  define AOBJ      
#  define AVAR(var) 
#  define AMSG(var) 
#  define ATHIS
#endif
// *************************************************************************