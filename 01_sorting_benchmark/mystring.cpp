#include "mystring.h"

mystring::mystring() {}

mystring::mystring(const std::string& s) : data(s) {}

const std::string& mystring::get() const
{
    return data;
}

int mystring::length() const
{
   return static_cast<int>(data.size()); //cause data.size returns type 'std::size_t', not int
    //return data.size();
}