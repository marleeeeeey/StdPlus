#pragma once

// ************************************************************************
#include <iostream>
#include <thread>
#include <iomanip>
#define ADEBUG_ON
#define ATHREAD "[" << std::setw(5) << std::this_thread::get_id() << "] "
class ObjectWatcher
{
    const char * m_name = nullptr;
public:
    ObjectWatcher(const char * name) : m_name(name)
    {
        std::cout << ATHREAD << "FUN START  " << m_name << std::endl;
    }
    ~ObjectWatcher()
    {
        std::cout << ATHREAD << "FUN FINISH " << m_name << std::endl;
    }
};
#ifdef ADEBUG_ON
#  define AFUN ObjectWatcher ___objectWatcher(__PRETTY_FUNCTION__)
#  define AVAR(var) std::cout << ATHREAD << "VAR " << #var << "=" << var << std::endl
#  define AMSG(var) std::cout << ATHREAD << "MSG " << var << std::endl
#  define ATHIS     std::cout << ATHREAD << "THIS=" << this << std::endl
#else
#  define AFUN      
#  define AVAR(var) 
#  define AMSG(var) 
#  define ATHIS
#endif
// *************************************************************************