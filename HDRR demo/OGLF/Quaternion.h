#ifndef QUATERNION_H
#define QUATERNION_H

#include "Error.h"


namespace Oglf
{
	/**
	* @class Quaternion used to handle 3D rotations using quaternions totally inspired from the Vic Hollis
	* NeHe quaternion camera tutorial.
	* http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=Quaternion_Camera_Class
	*/
	class Quaternion
	{
		float m_w;
		float m_x;
		float m_y;
		float m_z;

	public:

		Quaternion()
		{
			m_w = 0.0;
			m_x = 0.0;
			m_y = 0.0;
			m_z = 0.0;
		}

		/**
		* @brief creates a quaternion to build then a rotation matrix
		* @param angle the rotation angle in degrees
		* @param x the rotation axis x coordinate
		* @param y the rotation axis y coordinate
		* @param z the rotation axis z coordinate
		*/
		void createRotation(float angle, float x, float y, float z);

		/**
		* @brief creates a rotation matrix from the current set up quaternion
		* @param matrix a 4x4 homogeneous matrix (this matrix is supposed OpenGL compliant, i.e it is
		* represented in a 1-dimensional array which size is 16 and vectors are represented in column major order)
		*/
		void createRotationMatrix(float* matrix);

		/**
		* @brief operator * to allow quaternions multiplications
		* @param q another quaternion
		* @return the result of the quaternions multiplication
		*/
		Quaternion operator*(Quaternion q)
		{
			Quaternion r;

			r.m_w = m_w*q.m_w - m_x*q.m_x - m_y*q.m_y - m_z*q.m_z;
			r.m_x = m_w*q.m_x + m_x*q.m_w + m_y*q.m_z - m_z*q.m_y;
			r.m_y = m_w*q.m_y + m_y*q.m_w + m_z*q.m_x - m_x*q.m_z;
			r.m_z = m_w*q.m_z + m_z*q.m_w + m_x*q.m_y - m_y*q.m_x;

			return r;
		}
	};
}

#endif /*QUATERNION_H*/
