#include "Quaternion.h"
#include <math.h>
#include "Vec.h"


namespace Oglf
{
	/**
	* @brief creates a quaternion to build then a rotation matrix
	* @param angle the rotation angle in degrees
	* @param x the rotation axis x coordinate
	* @param y the rotation axis y coordinate
	* @param z the rotation axis z coordinate
	*/
	void Quaternion::createRotation(float angle, float x, float y, float z)
	{
		// First we want to convert the degrees to radians
		// since the angle is assumed to be in radians
		angle = (angle / 180.f) * (float)M_PI;

		// Here we calculate the sin( theta / 2) once for optimization
		float result = sin( angle / 2.0f );

		// Calcualte the w value by cos( theta / 2 )
		m_w = cos( angle / 2.0f );

		// Calculate the x, y and z of the quaternion
		Vec3 rotAxis(x, y, z);
		rotAxis.normalize();

		m_x = rotAxis.v[0] * result;
		m_y = rotAxis.v[1] * result;
		m_z = rotAxis.v[2] * result;
	}

	/**
	* @brief creates a rotation matrix from the current set up quaternion
	* @param matrix a 4x4 homogeneous matrix (this matrix is supposed OpenGL compliant, i.e it is
	* represented in a 1-dimensional array which size is 16 and vectors are represented in column major order)
	*/
	void Quaternion::createRotationMatrix(float* matrix)
	{
		// Make sure the matrix has allocated memory to store the rotation data
		if(!matrix) {
			throw Error("Quaternion::createRotationMatrix error: supplied matrix array is not allocated");
		}

		// First row
		matrix[0]  = 1.0f - 2.0f * ( m_y * m_y + m_z * m_z );
		matrix[1]  = 2.0f * (m_x * m_y - m_z * m_w);
		matrix[2]  = 2.0f * (m_x * m_z + m_y * m_w);
		matrix[3]  = 0.0f;

		// Second row
		matrix[4]  = 2.0f * ( m_x * m_y + m_z * m_w );
		matrix[5]  = 1.0f - 2.0f * ( m_x * m_x + m_z * m_z );
		matrix[6]  = 2.0f * (m_z * m_y - m_x * m_w );
		matrix[7]  = 0.0f;

		// Third row
		matrix[8]  = 2.0f * ( m_x * m_z - m_y * m_w );
		matrix[9]  = 2.0f * ( m_y * m_z + m_x * m_w );
		matrix[10] = 1.0f - 2.0f * ( m_x * m_x + m_y * m_y );
		matrix[11] = 0.0f;

		// Fourth row
		matrix[12] = 0;
		matrix[13] = 0;
		matrix[14] = 0;
		matrix[15] = 1.0f;

		// Now matrix[] is a 4x4 homogeneous matrix that can be applied to an OpenGL Matrix
	}
}

