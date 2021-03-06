#include "Cross.h"
#include "../Common/stdstringext.hpp"
int _wtoi(const wchar_t *wstr)
{
    std::string temp;
    temp = STDSTRINGEXT::WToA(wstr);
    return atoi(temp.c_str());
}
__int64 _wtoi64(const wchar_t *wstr)
{
   std::string temp;
    temp = STDSTRINGEXT::WToA(wstr);
    return strtoll(temp.c_str(),NULL,10);
}
float _wtof(const wchar_t* wstr)
{
    std::string temp;
    temp = STDSTRINGEXT::WToA(wstr);
    return atof(temp.c_str());
}