#pragma once

// ************************************************************************
#include <iostream>
class ObjectWatcher
{
    const char * m_name = nullptr;
public:
    ObjectWatcher(const char * name) : m_name(name) 
    { std::cout << "FUN START  " << m_name << std::endl; }
    ~ObjectWatcher() 
    { std::cout << "FUN FINISH " << m_name << std::endl; }
};

#define AFUN ObjectWatcher ___objectWatcher(__PRETTY_FUNCTION__)
#define AVAR(var) std::cout << "VAR " << #var << "=" << var << std::endl
#define AMSG(var) std::cout << "MSG " << var << std::endl
// *************************************************************************