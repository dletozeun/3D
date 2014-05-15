#include <iostream>
#include "GLSLshaderProgram.h"
#include "Error.h"

using namespace std;

namespace Oglf
{
	// default shader program handle used is 0 which means the fixed pipeline
	GLuint GLSLshaderProgram::m_inUse = 0;

	/**
	* @brief  prints the shaders building informations in console
	*/
	void GLSLshaderProgram::printProgramInfoLog() const
	{
		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;

		glGetProgramiv(m_handle, GL_INFO_LOG_LENGTH,&infologLength);

		if (infologLength > 0)
		{
			infoLog = new char[infologLength];
			glGetProgramInfoLog(m_handle, infologLength, &charsWritten, infoLog);
			cout << infoLog << endl;
			delete [] infoLog;
		}
	}

	/**
	* @brief constructor
	*/
	GLSLshaderProgram::GLSLshaderProgram ()
	{
		m_handle=glCreateProgram();
	}

	/**
	* @brief destructor
	*/
	GLSLshaderProgram::~GLSLshaderProgram ()
	{
		glDeleteProgram(m_handle);
	}
}
