#ifndef CIOC_HPP
#define CIOC_HPP

class cIoC
{
private:
    cIoC(const cIoC&) = delete;
    cIoC& operator=(const cIoC&) = delete;

public:
    cIoC() = default;
    ~cIoC() = default;

    template< typename T, typename... Args>
    T Resolve(const char *sz, iAdapterObj *obj, Args & ... args)
    {
    }
};

extern cIoC IoC;

#endif //#ifndef CIOC_HPP
