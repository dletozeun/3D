#ifndef GLTRANSFORMER3D_H
#define GLTRANSFORMER3D_H

#include "Matrix.h"
#include "Vec.h"
#include "Quaternion.h"


namespace Oglf
{
	class GLtransformer3D
	{
		Matrix4x4 m_transformMat;
		Vec3 m_pivot; // a reference point to which the transformation matrix is applied

	public:
		GLtransformer3D()
		{
			reset();
		}

		~GLtransformer3D()
		{

		}

		/**
		* @brief loads the transformation matrix with the identity matrix to apply no transformations
		*/
		void reset()
		{
			m_transformMat.identity();
		}



		/**
		* @brief applies a rotation to the transformation matrix
		* @param ax rotation angle on X axis
		* @param ay rotation angle on Y axis
		* @param az rotation angle on Z axis
		*/
		void rotate(float ax, float ay, float az)
		{
			Matrix4x4 tmpMat;

			tmpMat.rotationX(ax);
			m_transformMat= m_transformMat * tmpMat;
			tmpMat.rotationY(ay);
			m_transformMat= m_transformMat * tmpMat;
			tmpMat.rotationZ(az);
			m_transformMat= m_transformMat * tmpMat;
		}

		/**
		* @briefs applies a rotation of a given angle around a given axis using quaternions
		* @param angle the rotation angle
		* @param x the rotation axis x coordinate
		* @param y the rotation axis y coordinate
		* @param z the rotation axis z coordinate
		*/
		void rotate(float angle, float x, float y, float z)
		{
			Quaternion rotQ;
			rotQ.createRotation(angle, x, y, z);

			Matrix4x4 rotMatrix;
			rotQ.createRotationMatrix(rotMatrix.m);

			m_transformMat= m_transformMat * rotMatrix;
		}

		void rotate(float angle,Vec3 oAxis )
		{
			rotate( angle, oAxis.v[ 0 ], oAxis.v[ 1 ], oAxis.v[ 2 ] );
		}

		/**
		* @brief applies a translation on the transformation matrix
		* @param tx translation factor on X axis
		* @param ty translation factor on Y axis
		* @param tz translation factor on Z axis
		*/
		void translate(float tx, float ty, float tz)
		{
			Matrix4x4 tmpMat;
			Vec3 tVec(tx, ty, tz);
			tmpMat.translate(tVec);

			m_transformMat= m_transformMat * tmpMat;
		}

		/**
		* @brief applies a uniform scale on the transformation matrix
		* @param s the scale factor
		*/
		void uniformScale(float s)
		{
			Matrix4x4 tmpMat;
			tmpMat.scale(s);

			m_transformMat= m_transformMat * tmpMat;
		}

		/**
		* @brief Returns a reference to the transform matrix to write to. The transform matrix is a
		* Opengl compliant matrix store in the float[16] array in column major order
		* @return a reference to the transfrom matrix
		*/
		Matrix4x4& getTransformMatrix()
		{
			return m_transformMat;
		}

		/**
		* @brief applies all transformations to the object using the opengl modelview matrix
		*/
		void transform()
		{
			glMultMatrixf(m_transformMat.m);
		}

		/**
		* @brief sets the pivot position.
		* @param x pivot position on X axis
		* @param y pivot position on Y axis
		* @param z pivot position on Z axis
		*/
		void setPivotPosition(float x, float y, float z)
		{
			m_pivot= Vec3(x, y, z);
		}

		/**
		* @brief returns the pivot position
		* @return the pivot position after the matrix transformation
		*/
		const float* getPivotPosition() const
		{
			return (m_transformMat + m_pivot).v;
		}
	};
}

#endif /*GLTRANSFORMABLEOBJECT_H*/
