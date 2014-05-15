#ifndef NAMABLE_H
#define NAMABLE_H

#include <string>

class Namable
{
protected:
	std::string m_sName;

public:

	Namable( const std::string& sName )
		: m_sName( sName )
	{

	}

	std::string& getName()
	{
		return m_sName;
	}
};

#endif