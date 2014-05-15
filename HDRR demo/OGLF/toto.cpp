#include "utils.h"
#include "GLtransformer3D.h"

using namespace Oglf;

void toto()
{
	float a = 1.f;
	float b = 2.f;

	float c = maxT( a, b );

	GLtransformer3D trans;

	trans.rotate( c, a, b, c );
}