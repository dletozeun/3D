#ifndef ERROR_H
#define ERROR_H

#include <iostream>

#ifdef WIN32
	#include <Windows.h>
#endif

#include <GL/gl.h>
#include <string>

namespace Oglf
{

	/**
	* class Error used for managing exceptions.
	*/
	class Error
	{
		std::string	m_sMsg;
		std::string	m_sCode;
		bool	m_bFatal;

	public:

		/**
		* @brief constructor
		* @param sMsg the error message
		* @param sCode the error code
		* @param bFatal true: fatal error, the program must quit, false: not fatal
		*/
		Error( const std::string& sMsg, const std::string& sCode, bool bFatal = false )
			: m_sMsg( sMsg )
			, m_sCode( sCode )
			, m_bFatal( bFatal )
		{
		}

		Error( const std::string& sMsg )
			: m_sMsg( sMsg )
		{
		}

		/**
		* @brief return the error message
		* @return the error message
		*/
		const std::string& getMessage() const
		{
			return m_sMsg;
		}

		/**
		* @brief return the error code
		* @return the error code
		*/
		const std::string& getCode() const
		{
			return m_sCode;
		}

		/**
		* @brief show in the console the error message and the code (if it exists)
		*/
		void showError() const
		{
			if( m_sCode.empty() )
			{
				std::cout << m_sMsg << std::endl;
			}
			else
			{
				std::cout << m_sMsg << ": " << m_sCode << std::endl;
			}
		}

		/**
		* @brief return the seriousness of the error
		* @return true: fatal error, false: not fatal
		*/
		bool isFatal() const
		{
			return m_bFatal;
		}

		/**
		* @brief check whether any OpenGL error is thrown and display it in the console
		*/
		static void checkGLerror( const std::string& ID )
		{
			GLenum oErr = GL_NO_ERROR;
			oErr = glGetError();

			if ( oErr != GL_NO_ERROR )
			{
				if( !ID.empty() )
				{
					std::cout << ID << " : ";
				}
				std::cout << " GL_ERROR : " << oErr << std::endl;
			}
		}
	};
};

#endif
