#ifndef GLSLSHADER_H
#define GLSLSHADER_H

#include <GL/glew.h>

namespace Oglf
{
	enum shaderType
	{
		VERTEX_SHADER,
		FRAGMENT_SHADER
	};


	/**
	* @brief class that allow to manage easily a GLSL vertex and fragment shaders
	*/
	class GLSLshader
	{
		GLuint handle;

	public:

		/**
		* @brief constructor
		* @param  type the created shader type: vertex or fragment shader
		*/
		GLSLshader (shaderType type );

		/**
		* @brief destructor
		*/
		~GLSLshader ();

		/**
		* @brief prints the shaders building informations in console
		*/
		void printShaderInfoLog() const;

		/**
		* @brief  set the shader code source
		* @param  path path toward the shader source code
		*/
		void setSource (const char* path );

		/**
		* @return handle the shader handle
		*/
		GLuint getHandle ( ) const
		{
			return handle;
		}

	};
}

#endif // GLSLSHADER_H
