#ifndef GLSLSHADERPROGRAM_H
#define GLSLSHADERPROGRAM_H

#include <GL/glew.h>
#include "GLSLshader.h"
#include "Error.h"

namespace Oglf
{
	void printProgramInfoLog(GLuint obj);

	/**
	* @brief class that allow to manage easily a GLSL program
	*/
	class GLSLshaderProgram
	{
		GLuint m_handle;       // shader program handle
		static GLuint m_inUse; // current shader program in use handle (0 is the OpenGL fixed pipeline)

	public:

		/**
		* @brief constructor
		*/
		GLSLshaderProgram();

		/**
		* @brief destructor
		*/
		~GLSLshaderProgram();

		/**
		* @brief  prints the shaders building informations in console
		*/
		void printProgramInfoLog() const;

		/**
		* @brief attach a shader to the program
		* @param  shader a shader to attach
		*/
		void attachShader (GLSLshader& shader ) const
		{
			glAttachShader(m_handle,shader.getHandle());
		}

		/**
		* @brief detach a shader from the program
		* @param  shader a shader to detach
		*/
		void detachShader (GLSLshader& shader ) const
		{
			glDetachShader(m_handle,shader.getHandle());
		}

		/**
		* @brief link the program
		*/
		void link ( ) const
		{
			glLinkProgram( m_handle );
			
			int iLinkStatus = GL_FALSE;
			glGetProgramiv( m_handle, GL_LINK_STATUS, &iLinkStatus );
				
			if( iLinkStatus == GL_FALSE )
			{
				printProgramInfoLog();
				throw Error( "GLSLshader::setSource error : Failed to link the shader program" );
			}
		}

		/**
		* @param  put the program in use or not
		* @param  state true: program is in use, false: fixed pipeline
		*/
		void use( bool state=true ) const
		{
			if(state)
			{
				glUseProgram(m_handle);
				m_inUse = m_handle;
			}
			else
			{
				glUseProgram(0);
				m_inUse = 0;
			}
		}

		/**
		* @return handle the shader program handle
		*/
		GLuint getHandle( ) const
		{
			return m_handle;
		}

		/**
		* @brief Returns the handle of the current used shader program. 0 is the fixed pipeline.
		* @return the current used shader program handle
		*/
		static GLuint currentShaderProgramInUse()
		{
			return m_inUse;
		}

		/**
		* @brief Enables the fixed pipeline.
		*/
		static void useFixedPipeline()
		{
			glUseProgram(0);
			m_inUse = 0;
		}
	};
}

#endif // GLSLSHADERPROGRAM_Hinline
