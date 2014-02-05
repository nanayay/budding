#ifndef AY_APPLICATION
#define AY_APPLICATION

class Application
{
public:
    Application() {};
    virtual ~Application() {};

    virtual bool Initialize() = 0;
    virtual void Run() = 0;
};

#endif