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

	std::string getPlatformName() const { return m_name; }
	void setPlatformName(std::string name) { m_name = name; };

};
#endif
