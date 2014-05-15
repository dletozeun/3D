#ifndef CORE_H
#define CORE_H

#include <map>
#include <iostream>

#define LOG_MEMORY_ALLOCATIONS
#define TRACE_MEMORY_ALLOCATIONS


namespace Oglf
{
	class Core
	{
		static size_t m_iAllocatedByteNb;
		static std::map<char*, size_t> m_oAllocatedSizeMap;

	public:

#ifdef TRACE_MEMORY_ALLOCATIONS

		void* operator new(size_t size)
		{
			m_iAllocatedByteNb += size;
			char* data = ::new char[size];
			m_oAllocatedSizeMap.insert( std::pair< char*, size_t >( data, size ) );

#ifdef LOG_MEMORY_ALLOCATIONS

			std::cout << "allocated memory: " << m_iAllocatedByteNb << " byte(s)" << std::endl;

#endif

			return data;
		}

		void operator delete(void* data)
		{
			m_iAllocatedByteNb -= m_oAllocatedSizeMap[ ( char* )data ];
			m_oAllocatedSizeMap.erase( ( char* )data );

#ifdef LOG_MEMORY_ALLOCATIONS

			std::cout << "allocated memory: " << m_iAllocatedByteNb << " byte(s)" << std::endl;

#endif

			::operator delete( data );
		}

		void* operator new[](size_t size)
		{
			m_iAllocatedByteNb += size;
			char* data = ::new char[size];
			m_oAllocatedSizeMap.insert( std::pair<char*, size_t>( data, size ) );

#ifdef LOG_MEMORY_ALLOCATIONS

			std::cout << "allocated memory: " << m_iAllocatedByteNb << " byte(s)" << std::endl;

#endif

			return data;
		}

		void operator delete[](void* data)
		{
			m_iAllocatedByteNb -= m_oAllocatedSizeMap[ ( char* )data];
			m_oAllocatedSizeMap.erase( ( char* )data );

#ifdef LOG_MEMORY_ALLOCATIONS

			std::cout << "allocated memory: " << m_iAllocatedByteNb << " byte(s)" << std::endl;

#endif

			::operator delete[]( data );
		}

#endif

	};
}

#endif /* CORE_H */
