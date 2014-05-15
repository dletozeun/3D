#ifndef UTIL_H
#define UTIL_H

namespace Oglf
{
	template< typename T>
	T maxT( T a, T b )
	{
		if( a > b )
			return a;

		return b;
	}

	/**
	* @brief computes the maximum power of two value less than the argument
	* @param an integer
	* @return the the maximum power of two value less than the argument
	*/
	int powerOf2(int val);


// 	int max( int a, int b );
// 	float max( float a, float b );
// 	double max( double a, double b );
}

#endif
