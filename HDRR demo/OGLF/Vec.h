#ifndef _VEC_H_
#define _VEC_H_

#include <iostream>
#include <stdio.h>
#include <math.h>

namespace Oglf
{
	struct Vec3
	{
		float v[3]; // dx, dy, dz

		Vec3()                            { v[0] = v[1] = v[2] = 0.0; };
		Vec3( const Vec3& u)            { v[0] = u.v[0]; v[1] = u.v[1]; v[2] = u.v[2];  };
		Vec3( float x, float y, float z ) { v[0] = x; v[1] = y; v[2] = z; };
		Vec3( const Vec3 &v1, const Vec3 &v2, const float &t );
		Vec3( const Vec3 &v1, const Vec3 &v2 ); // v1 = starting point; v2 = ending point

		float length() const;
		float length2() const;
		void  normalize();

		void flipX();
		void flipY();
		void flipZ();


		Vec3& operator  = ( const Vec3 & );                          // copie
		Vec3& operator += ( const Vec3 & );                          // incrementation
		Vec3& operator -= ( const Vec3 & );                          // decrementation
		Vec3& operator *= ( const float );                             // multiplication par un scalaire
		Vec3& operator /= ( const float );                             // division par un scalaire

		friend Vec3 operator  - ( const Vec3& u);                    // -v1
		friend Vec3 operator  + ( const Vec3& a, const Vec3& b );  // v1 + v2
		friend Vec3 operator  - ( const Vec3& a, const Vec3& b );  // v1 - v2
		friend Vec3 operator  * ( const Vec3& a, const float d   );  // v1 * 3.0
		friend Vec3 operator  * ( const float d,   const Vec3& a );  // 3.0 * v1
		friend float  operator  | ( const Vec3& a, const Vec3& b );  // produit scalaire
		friend Vec3 operator  / ( const Vec3& a, const float d   );  // v1 / 3.0
		friend Vec3 operator  ^ ( const Vec3& a, const Vec3& b );  // produit vectoriel
		friend bool   operator == ( const Vec3& a, const Vec3& b );  // v1 == v2 ?
		friend bool   operator != ( const Vec3& a, const Vec3& b );  // v1 != v2 ?

		float  operator[]( int i )  const { return v[i]; }
		float& operator[]( int i )        { return v[i]; }

		//void print( char * ) const;

	};

	inline void Vec3::normalize()
	{
		float l = length();
		v[0] /= l;
		v[1] /= l;
		v[2] /= l;
	}

	inline Vec3::Vec3( const Vec3 &v1, const Vec3 &v2, const float &t )
	{
		v[0] = ( v2.v[0] - v1.v[0] ) * t + v1.v[0];
		v[1] = ( v2.v[1] - v1.v[1] ) * t + v1.v[1];
		v[2] = ( v2.v[2] - v1.v[2] ) * t + v1.v[2];
	}

	inline Vec3::Vec3( const Vec3 &v1, const Vec3 &v2 )
	{
		v[0] = ( v2.v[0] - v1.v[0] );
		v[1] = ( v2.v[1] - v1.v[1] );
		v[2] = ( v2.v[2] - v1.v[2] );
	}

	inline float Vec3::length() const
	{
		return sqrt( v[0]*v[0] + v[1]*v[1] + v[2]*v[2] );
	}

	inline float Vec3::length2() const
	{
		return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
	}

	inline Vec3& Vec3::operator = (const Vec3& u)
	{
		v[0] = u.v[0]; v[1] = u.v[1]; v[2] = u.v[2]; return *this;
	}

	inline Vec3& Vec3::operator += ( const Vec3& u )
	{
		v[0] += u.v[0]; v[1] += u.v[1]; v[2] += u.v[2];
		return *this;
	}

	inline Vec3& Vec3::operator -= ( const Vec3& u )
	{
		v[0] -= u.v[0]; v[1] -= u.v[1]; v[2] -= u.v[2];
		return *this;
	}

	inline Vec3& Vec3::operator *= ( const float d )
	{
		v[0] *= d; v[1] *= d; v[2] *= d;
		return *this;
	}

	inline Vec3& Vec3::operator /= ( const float d )
	{
		float d_inv = 1.f/d;

		v[0] *= d_inv; v[1] *= d_inv; v[2] *= d_inv;
		return *this;
	}

	inline Vec3 operator - (const Vec3& a)
	{
		return Vec3( -a.v[0],-a.v[1],-a.v[2]);
	}

	inline Vec3 operator + (const Vec3& a, const Vec3& b)
	{
		return Vec3(  a.v[0]+ b.v[0], a.v[1] + b.v[1], a.v[2] + b.v[2]);
	}

	inline Vec3 operator - (const Vec3& a, const Vec3& b)
	{
		return Vec3(  a.v[0]-b.v[0], a.v[1]-b.v[1], a.v[2]-b.v[2]);
	}

	inline Vec3 operator * (const Vec3& a, const float d)
	{
		return Vec3(  d*a.v[0], d*a.v[1], d*a.v[2]);
	}

	inline Vec3 operator * (const float d, const Vec3& a)
	{
		return a*d;
	}

	inline float operator | (const Vec3& a, const Vec3& b)
	{
		return (a.v[0]*b.v[0] + a.v[1]*b.v[1] + a.v[2]*b.v[2]);
	}

	inline Vec3 operator / (const Vec3& a, const float d)
	{
		float d_inv = 1.f/d;

		return Vec3(  a.v[0]*d_inv, a.v[1]*d_inv, a.v[2]*d_inv);
	}

	inline Vec3 operator ^ (const Vec3& a, const Vec3& b)
	{
		return Vec3(  a.v[1]*b.v[2] - a.v[2]*b.v[1],
			a.v[2]*b.v[0] - a.v[0]*b.v[2],
			a.v[0]*b.v[1] - a.v[1]*b.v[0] );
	}

	inline bool operator == (const Vec3& a, const Vec3& b)
	{
		return (a.v[0] == b.v[0]) && (a.v[1] == b.v[1]) && (a.v[2] == b.v[2]);
	}

	inline bool operator != (const Vec3& a, const Vec3& b)
	{
		return !(a == b);
	}

	inline void Vec3::flipX()
	{
		static float t;

		t    = v[2]; // t = dz;
		v[2] = v[1]; // dz = dy;
		v[1] = t;    // dy = t;
	}

	inline void Vec3::flipY()
	{
		static float t;

		t    = v[2];
		v[2] = v[0];
		v[0] = t;
	}

	inline void Vec3::flipZ()
	{
		static float t;

		t    = v[0];
		v[0] = v[1];
		v[1] = t;
	}

	inline std::ostream& operator << (std::ostream& out, Vec3& v)
	{
		return out<<"("<<v.v[0]<<", "<<v.v[1]<<", "<<v.v[2]<<")";
	}
}

#endif
