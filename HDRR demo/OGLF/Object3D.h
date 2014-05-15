#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "GLtransformer3D.h"
#include "Core.h"


namespace Oglf
{
	class Object3D
	{
		bool m_transformable; // is the object transformable? default: false
		GLtransformer3D m_transformer;

	public:
		Object3D() : m_transformable(false)
		{

		}

		virtual ~Object3D()
		{

		}

		/**
		* returns wether this 3D object is transformable or not
		*/
		bool isTransformable()
		{
			return m_transformable;
		}

		/**
		* @brief sets the object transformable or not
		* @param state true: the object is transformable, false: it is not
		*/
		void transformable(bool state)
		{
			m_transformable=state;
		}

		GLtransformer3D& getTransformer()
		{
			return m_transformer;
		}

		virtual void transform()
		{
			m_transformer.transform();
		}

		/**
		* @brief draws the 3D object
		*/
		virtual void draw()
		{

		}
	};
}

#endif /*OBJECT3D_H*/
