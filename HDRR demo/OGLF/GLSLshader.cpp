#include <fstream>
#include <iostream>
#include "GLSLshader.h"
#include "Error.h"

using namespace std;

namespace Oglf
{
	/**
	* @brief constructor
	* @param  type the created shader type: vertex or fragment shader
	*/
	GLSLshader::GLSLshader (shaderType type )
	{
		if(type==VERTEX_SHADER)
		{
			handle=glCreateShader(GL_VERTEX_SHADER);
		}
		else
		{
			handle=glCreateShader(GL_FRAGMENT_SHADER);
		}
	}

	/**
	* @brief destructor
	*/
	GLSLshader::~GLSLshader ()
	{
		glDeleteShader(handle);
	}

	/**
	* @brief prints the shaders building informations in console
	*/
	void GLSLshader::printShaderInfoLog() const
	{
		int infologLength = 0;
		int charsWritten = 0;
		char *infoLog;

		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &infologLength);

		if (infologLength > 0)
		{
			infoLog = new char[infologLength];
			glGetShaderInfoLog(handle, infologLength, &charsWritten, infoLog);
			cout << infoLog << endl;
			delete [] infoLog;
		}
	}


	/**
	* @brief reads text from a file and returns the content in a string
	* @param fn file name
	*/
	char* textFileRead(const char *fn)
	{
		ifstream is;
		char *content = NULL;
		int count=0;

		is.open(fn);
		if (is.is_open())
		{
			// get length of file:
			is.seekg (0, ios::end);
			count = is.tellg();
			is.seekg (0, ios::beg);

			if (count > 0)
			{
				content = new char[count+1];
				is.read (content,count);
				content[count] = '\0';
			}
			else
			{
				throw Error( "GLSLshader::textFileRead error: shader source file is empty" );
			}
			is.close();
		}
		else
		{
			throw Error("GLSLshader::textFileRead error: Cannot open the file ", fn );
		}

		return content;
	}

	/**
	* @brief  set the shader code source
	* @param  path path toward the shader source code
	*/
	void GLSLshader::setSource (const char* path )
	{
		char* shaderSrc = NULL;

		try
		{
			//read source data
			shaderSrc = textFileRead(path);
		}
		catch (Error e)
		{
			e.showError();
			throw Error( "GLSLshader::setSource error : Invalid source code!" );
			return;
		}

		const char* shaderSrcc = shaderSrc;

		//set source code
		glShaderSource(handle, 1, &shaderSrcc, NULL);

		if(shaderSrc!=NULL)
		{
			delete[] shaderSrc;
		}

		cout << "Compiling shader " << path << " ... ";
		glCompileShader( handle );
		
		int iCompileStatus = GL_FALSE;
		glGetShaderiv( handle, GL_COMPILE_STATUS, &iCompileStatus ); 
		
		if( iCompileStatus == GL_FALSE )
		{
			this->printShaderInfoLog();
			throw Error( "GLSLshader::setSource error : Failed to compile the shader", path );
			return;
		}
		cout << "done!" << endl;
	}
}
