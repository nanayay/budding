#ifndef AY_PLATFORM
#define AY_PLATFORM

#include <string>

class Platform
{
private:
	std::string m_name;

public:
	explicit Platform(const std::string& name)
			: m_name(name)
	{
	}

	explicit Platform(const char* p_str)
			: m_name(p_str)
	{
	}

	virtual ~Platform() {}

	std::string GetPlatformName() const { return m_name; }

};
#endif
