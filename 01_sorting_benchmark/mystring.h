#pragma once
#include <string>

class mystring
{
private:
    std::string data;

public:
    mystring();
    mystring(const std::string& s);

    const std::string& get() const;
    int length() const;
};