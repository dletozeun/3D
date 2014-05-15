#ifndef _MATRIX_H
#define _MATRIX_H

#include <iostream>
#include <string>

#include "Vec.h"

namespace Oglf
{
	class Matrix4x4
	{
	public:

		float m[16]; // Opengl compliant matrix that is internally stored in gl
		// column-major order. (16 consecutive floats)
		// Nevertheless, the overloded (,) operator accesses the coefficients
		// in the mathematical style row-major order.
		// (that is, take (row,column) as arguments)

		Matrix4x4();
		~Matrix4x4();
		Matrix4x4(const Matrix4x4& ma);
		Matrix4x4& operator = (const Matrix4x4& ma);
		float& operator () ( unsigned row, unsigned col );
		float  operator () ( unsigned row, unsigned col ) const;

		// transform v[x,y,z,1] and w-divide <=> 3d point transformation
		friend Vec3 operator + ( const Matrix4x4& m, const Vec3 &v );
		friend Vec3 operator + ( const Vec3 &v, const Matrix4x4& m );

		// transform v[x,y,z,0] <=> vector transformation
		friend Vec3 operator * ( const Matrix4x4& m, const Vec3 &v );
		friend Vec3 operator * ( const Vec3 &v, const Matrix4x4& m );

		Matrix4x4 operator * ( const Matrix4x4 &b ) const;

		void identity();
		void resetScaleAndRotation();
		void resetTranslation();
		void invert();
		void rotationX( float radians );
		void rotationY( float radians );
		void rotationZ( float radians );
		void scale( float s );
		void translate( Vec3 &t );
		void print( char *nom );

		void transpose();

		void display()
		{
			for(int i=0; i<4; i++)
			{
				for(int j=0; j<4; j++)
				{
					std::cout<<m[i+4*j]<<" ";
				}
				std::cout<<std::endl;
			}
		}
	};
}

#endif

