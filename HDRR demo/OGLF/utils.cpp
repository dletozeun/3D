#include "utils.h"

namespace Oglf
{
	/**
	* @brief computes the maximum power of two value less than the argument
	* @param an integer
	* @return the the maximum power of two value less than the argument
	*/
	int powerOf2(int val)
	{
		int i=1;
		while( i < val )
			i = i << 1;
			
		return i >> 1;
	}
}


// int max(int a, int b)
// {
// 	if(a>b) {
// 		return a;
// 	}
// 	return b;
// }
// 
// float max( float a, float b)
// {
// 	if(a > b) {
// 		return a;
// 	}
// 	return b;
// }
// 
// double max(double a, double b)
// {
// 	if(a>b) {
// 		return a;
// 	}
// 	return b;
// }
