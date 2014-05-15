#include "Core.h"
#include <map>


namespace Oglf
{
	size_t Core::m_iAllocatedByteNb = 0;
	std::map< char*, size_t > Core::m_oAllocatedSizeMap;
}
