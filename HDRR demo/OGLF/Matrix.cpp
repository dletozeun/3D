#include "Matrix.h"
#include <cstring>


namespace Oglf
{
	Matrix4x4::Matrix4x4()
	{
		this->identity();
	}

	Matrix4x4::~Matrix4x4()
	{
	}

	Matrix4x4::Matrix4x4(const Matrix4x4& ma)
	{
		memcpy( m, ma.m, sizeof( m ) );
	}

	Matrix4x4& Matrix4x4::operator = (const Matrix4x4& ma)
	{
		memcpy( m, ma.m, sizeof(m) );
		return *this;
	}

	float& Matrix4x4::operator () ( unsigned row, unsigned col )
	{
		return m[col*4+row]; // because of gl column major matrix mode
	}

	float Matrix4x4::operator () ( unsigned row, unsigned col ) const
	{
		return m[col*4+row];
	}

	void Matrix4x4::identity()
	{
		memset( m, 0, sizeof(m) );
		Matrix4x4 &mat = *this;

		mat(0,0) = 1.0;
		mat(1,1) = 1.0;
		mat(2,2) = 1.0;
		mat(3,3) = 1.0;

	}

	void Matrix4x4::resetScaleAndRotation()
	{
		(*this)(0,0) = 1.0;
		(*this)(0,1) = 0.0;
		(*this)(0,2) = 0.0;
		(*this)(1,0) = 0.0;
		(*this)(1,1) = 1.0;
		(*this)(1,2) = 0.0;
		(*this)(2,0) = 0.0;
		(*this)(2,1) = 0.0;
		(*this)(2,2) = 1.0;
	}

	void Matrix4x4::resetTranslation()
	{
		(*this)(0,3) = 0.0;
		(*this)(1,3) = 0.0;
		(*this)(2,3) = 0.0;
		(*this)(3,3) = 1.0;
	}

	void Matrix4x4::transpose()
	{
		Matrix4x4 sav = (*this);

		for( int l = 0; l < 4; l++ )
			for( int c = 0; c < 4; c++ )
				(*this)(l,c) = sav(c,l);
	}

	Matrix4x4 Matrix4x4::operator * ( const Matrix4x4 &b ) const
	{
		Matrix4x4 m;

		for( int l = 0; l < 4; l++ )
			for( int c = 0; c < 4; c++ )
				m(l,c) = (*this)(l,0) * b(0,c) + (*this)(l,1) * b(1,c) + (*this)(l,2) * b(2,c) + (*this)(l,3) * b(3, c);

		return m;
	}

	// Gauss-Jordan elimination with partial pivoting
	void Matrix4x4::invert()
	{
		Matrix4x4 a(*this),          // a evolves from original to identity
			b;                 // b evolves from indentity to inverse(a)

		int   i, j, i1, i2;
		float temp, pivot;
		float factor=0.0;

		b.identity();           // b : identity

		for( j=0; j<4; j++ )    // Loop over cols eliminating above and below diag
		{                       // Find largest pivot in col j
			i1 = j;
			for( i=j; i<4; i++ ) // Row with possible largest pivot
				if( fabs(a(i,j)) > fabs(a(i1,j) ) ) i1 = i;
			if (i1!=j)
			{
				for (i2=0; i2<4; i2++)
				{
					temp = b(i1,i2);
					b(i1,i2) = b(j,i2);
					b(j,i2) = temp;
					if (i2>=j)
					{
						temp = a(i1,i2);
						a(i1,i2) = a(j,i2);
						a(j,i2) = temp;
					}
				}
			}
			pivot = a(j,j);
			for (i2=0; i2<4; i2++)
			{
				b(j,i2) = b(j,i2) / pivot;                   // Scale row j to have a unit diag
				if (i2>=j) a(j,i2) = a(j,i2) / pivot;
			}

			for( i=0; i<4; i++)           // Eliminate off-diag elems in col j (a) and (b)
			{
				if (i!=j) factor = a(i,j);
				for (i2=0; i2<4; i2++)
				{
					if (i!=j)
					{
						b(i,i2) = b(i,i2) - factor * b(j,i2);
						a(i,i2) = a(i,i2) - factor * a(j,i2);
					}
				}
			}                       // Inversion done
		}

		(*this)(0,0) = b(0,0) ; (*this)(0,1) = b(0,1) ; (*this)(0,2) = b(0,2) ; (*this)(0,3) = b(0,3) ;
		(*this)(1,0) = b(1,0) ; (*this)(1,1) = b(1,1) ; (*this)(1,2) = b(1,2) ; (*this)(1,3) = b(1,3) ;
		(*this)(2,0) = b(2,0) ; (*this)(2,1) = b(2,1) ; (*this)(2,2) = b(2,2) ; (*this)(2,3) = b(2,3) ;
		(*this)(3,0) = b(3,0) ; (*this)(3,1) = b(3,1) ; (*this)(3,2) = b(3,2) ; (*this)(3,3) = b(3,3) ;


	}

	/*void blMatrix::print( char *nom )
	{
	blprintf("\n(II) Matrice %s\n", nom );

	for( int l=0; l < 4; l++ )
	blprintf("(II) | %3.3f | %3.3f | %3.3f | %3.3f |\n",
	(*this)(l,0), (*this)(l,1), (*this)(l,2), (*this)(l, 3) );
	}*/


	Vec3 operator * ( const Vec3 &v, const Matrix4x4& m )
	{
		return m*v;
	}

	Vec3 operator * ( const Matrix4x4& m, const Vec3 &v )
	{
		return Vec3(
			( m(0,0) * v[0] + m(0,1) * v[1] + m(0,2) * v[2] + m(0,3) ) ,
			( m(1,0) * v[0] + m(1,1) * v[1] + m(1,2) * v[2] + m(1,3) ) ,
			( m(2,0) * v[0] + m(2,1) * v[1] + m(2,2) * v[2] + m(2,3) )
			);

	}

	Vec3 operator + ( const Vec3 &v, const Matrix4x4& m )
	{
		return m+v;
	}

	Vec3 operator + ( const Matrix4x4& m, const Vec3 &v )
	{
		float w = m(3,0) * v[0] + m(3,1) * v[1] + m(3,2) * v[2] + m(3,3);
		return Vec3(
			( m(0,0) * v[0] + m(0,1) * v[1] + m(0,2) * v[2] + m(0,3) ) / w,
			( m(1,0) * v[0] + m(1,1) * v[1] + m(1,2) * v[2] + m(1,3) ) / w,
			( m(2,0) * v[0] + m(2,1) * v[1] + m(2,2) * v[2] + m(2,3) ) / w
			);

	}


	void Matrix4x4::rotationX( float radians )
	{
		float _cos = cos( radians ),
			_sin = sin( radians );

		identity();

		(*this)(1,1) = _cos;
		(*this)(1,2) = -_sin;
		(*this)(2,1) = _sin;
		(*this)(2,2) = _cos;
	}

	void Matrix4x4::rotationY( float radians )
	{
		float _cos = cos( radians ),
			_sin = sin( radians );

		identity();
		(*this)(0,0) = _cos;
		(*this)(2,0) = -_sin;
		(*this)(0,2) = _sin;
		(*this)(2,2) = _cos;
	}

	void Matrix4x4::rotationZ( float radians )
	{
		float _cos = cos( radians ),
			_sin = sin( radians );

		identity();
		(*this)(0,0) = _cos;
		(*this)(0,1) = -_sin;
		(*this)(1,0) = _sin;
		(*this)(1,1) = _cos;
	}

	void Matrix4x4::scale( float s )
	{
		identity();
		(*this)(0,0) = s;
		(*this)(1,1) = s;
		(*this)(2,2) = s;
	}

	void Matrix4x4::translate( Vec3 &t )
	{
		identity();
		(*this)(0,3) = t[0];
		(*this)(1,3) = t[1];
		(*this)(2,3) = t[2];
	}
}
