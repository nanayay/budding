#ifndef AY_PATTERN
#define AY_PATTERN

#include <cassert>

class noncopyable
{
public:
    noncopyable() = default;
    virtual ~noncopyable() = default;
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
};

/*
This Singleton pattern is special, to ignore the issue on multi-thread environment, you need to find a good place to constructor and destructor the Singleton class
*/
template <typename T>
class Singleton
    : public noncopyable
{
private:
    static T* ms_pSingleton;

public:
    Singleton()
    {
        assert(!ms_pSingleton);
        ms_pSingleton = static_cast<T*>(this);
    };
    virtual ~Singleton()
    {
        assert(ms_pSingleton);
        ms_pSingleton = nullptr;
    };

    static T* GetSingletonPtr()
    {
        assert(ms_pSingleton);
        return ms_pSingleton;
    };

    static T& GetSingleton()
    {
        assert(ms_pSingleton);
        return *ms_pSingleton;
    };
};

template<typename T>
T* Singleton<T>::ms_pSingleton = nullptr;

#include "pattern.inl"

#endif