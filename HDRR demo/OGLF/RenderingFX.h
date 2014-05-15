#ifndef RENDERINGFX_H

#define RENDERINGFX_H

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "GLSLshader.h"
#include "GLSLshaderProgram.h"
#include "Error.h"
#include "Texture.h"


namespace Oglf
{
	/**
	* @brief list of possible parameter types passed as uniforms in shader program
	*/
	enum RFXparamType
	{
		FR_INT = 0,
		FR_INT_VEC2,
		FR_INT_VEC3,
		FR_INT_VEC4,
		FR_FLOAT,
		FR_FLOAT_VEC2,
		FR_FLOAT_VEC3,
		FR_FLOAT_VEC4,
		FR_FLOAT_MAT2,
		FR_FLOAT_MAT3,
		FR_FLOAT_MAT4,
		FR_SAMPLER1D,
		FR_SAMPLER2D,
		FR_SAMPLER3D
	};

	/**
	* @brief stores a parameter and manages assignment to a uniform variable in a shader program
	*/
	struct RFXparameter
	{
		const void* m_data;   // parameter data
		short m_size;   // parameter element number if this one an array
		GLint location; // associated uniform location in the shader program object


		RFXparameter(void* data, short size, GLint location) : m_data(data), m_size(size), location(location)
		{
			refresh();
		}

		virtual void refresh()
		{

		}
	};

	class RFXfloat : public RFXparameter
	{
	public:

		RFXfloat(float* data, short size, GLint location) : RFXparameter((void*)data, size, location)
		{
		}

		void refresh()
		{
			glUniform1fv(location, m_size, (GLfloat*)m_data);
		}
	};

	class RFXfloatVec2 : public RFXparameter
	{
	public:

		RFXfloatVec2(float* data, short size, GLint location) : RFXparameter((void*)data, size, location)
		{
		}

		void refresh()
		{
			glUniform2fv(location, m_size, (GLfloat*)m_data);
		}
	};

	class RFXfloatVec3 : public RFXparameter
	{
	public:

		RFXfloatVec3(float* data, short size, GLint location) : RFXparameter((void*)data, size, location)
		{
		}

		void refresh()
		{
			glUniform3fv(location, m_size, (GLfloat*)m_data);
		}
	};

	class RFXfloatVec4 : public RFXparameter
	{
	public:

		RFXfloatVec4(float* data, short size, GLint location) : RFXparameter((void*)data, size, location)
		{
		}

		void refresh()
		{
			glUniform4fv(location, m_size, (GLfloat*)m_data);
		}
	};

	class RFXfloatMat2 : public RFXparameter
	{
	public:

		RFXfloatMat2(float* data, short size, GLint location) : RFXparameter((void*)data, size, location)
		{
		}

		void refresh()
		{
			glUniformMatrix2fv(location, m_size, 0, (GLfloat*)m_data);
		}
	};

	class RFXfloatMat3 : public RFXparameter
	{
	public:

		RFXfloatMat3(float* data, short size, GLint location) : RFXparameter((void*)data, size, location)
		{
		}

		void refresh()
		{
			glUniformMatrix3fv(location, m_size, 0, (GLfloat*)m_data);
		}
	};

	class RFXfloatMat4 : public RFXparameter
	{
	public:

		RFXfloatMat4(float* data, short size, GLint location) : RFXparameter((void*)data, size, location)
		{
		}

		void refresh()
		{
			glUniformMatrix4fv(location, m_size, 0, (GLfloat*)m_data);
		}
	};

	class RFXint : public RFXparameter
	{
	public:

		RFXint(int* data, short size, GLint location) : RFXparameter((void*)data, size, location)
		{
		}

		void refresh()
		{
			glUniform1iv(location, m_size, (GLint*)m_data);
		}
	};

	class RFXintVec2 : public RFXparameter
	{
	public:

		RFXintVec2(int* data, short size, GLint location) : RFXparameter((void*)data, size, location)
		{
		}

		void refresh()
		{
			glUniform2iv(location, m_size, (GLint*)m_data);
		}
	};

	class RFXintVec3 : public RFXparameter
	{
	public:

		RFXintVec3(int* data, short size, GLint location) : RFXparameter((void*)data, size, location)
		{
		}

		void refresh()
		{
			glUniform3iv(location, m_size, (GLint*)m_data);
		}
	};

	class RFXintVec4 : public RFXparameter
	{
	public:

		RFXintVec4(int* data, short size, GLint location) : RFXparameter((void*)data, size, location)
		{
		}

		void refresh()
		{
			glUniform4iv(location, m_size, (GLint*)m_data);
		}
	};

	class RFXtexture
	{
		static unsigned short texturesUnitMax;

	public:
		Texture* tex;
		unsigned short texUnit;

		RFXtexture(Texture* tex, unsigned short texUnit) : tex(tex)
		{
			if(texUnit < texturesUnitMax)
			{
				this->texUnit = GL_TEXTURE0 + texUnit;
			}
			else
			{
				throw Error("RFXtexture::RFXtexture error: specified texture unit is higher than the maximum supported texture unit number");
			}
		}
	};


	/**
	* @brief Interface to a rendering FX
	*/
	class RenderingFX
	{
		GLSLshader* m_vertShader;
		GLSLshader* m_fragShader;

		std::vector<RFXparameter*> m_parametersList;
		std::vector<RFXtexture*> m_texturesList;

	protected:

		GLSLshaderProgram m_program; // shader program that computes a rendering effect

	public:

		/**
		* @brief constructor, rendering FX initialization is done here
		*/
		RenderingFX() : m_vertShader(NULL), m_fragShader(NULL)
		{

		}

		/**
		* @brief destructor, all clean up is done here
		*/
		virtual ~RenderingFX();

		/**
		* @brief sets the shaders path that will be used by this effect
		* @param vsName Vertex Shader name
		* @param fsName Fragment Shader name
		*/
		void setShaders(const char* vsName, const char* fsName);

		/**
		* @brief overloadable function that actives the rendering FX for the current rendering
		*/
		inline virtual void enable() const;

		/**
		* @brief overloadable function that deactivates the rendering FX for the current rendering
		*/
		inline virtual void disable() const;

		/**
		* @brief adds a parameter to the rendering FX that will be used as a uniform in associated program shader
		* @param type the parameter type
		* @param size if the parameter is an array, say its size
		* @param name the name of the uniform in the associated shader program
		* @return the id of the added parameter in the parameter list (used to refresh its value next)
		*/
		unsigned int addParameter(const void* param, RFXparamType type, const short size, const char* name);

		/**
		* @brief adds a parameter to the rendering FX that will be used as a uniform in associated program shader
		* @param tex a texture object
		* @param textureUnit the texture unit to bind the texture to
		* @param name the name of the uniform in the associated shader program
		* @return the id of the added texture in the texture list
		*/
		unsigned int addTexture(Texture* tex, const unsigned short textureUnit, const char* name);

		/**
		* @brief removes a texture from the rendering FX
		* @param id the texture id in the texture list returned when adding a texture
		*/
		void removeTexture(unsigned int id);

		/**
		* @brief Removes a parameter from the rendering FX.
		* @param id the parameter id in the parameter list returned when adding the parameter
		*/
		void removeParameter(const unsigned int id);

		/**
		* @brief Refreshes a parameter given its id in the parameter list (see addParameter).
		* Note that there is no error handling for performance reasons, take care of the id given in parameter
		* @param id the parameter id in the parameter list returned when adding the parameter
		*/
		inline void refreshParameter(const unsigned int id) const;

		/**
		* @brief Update a parameter data location
		* @param id the parameter id in the parameter list returned when adding the parameter
		* @param data the parameter data location
		*/
		inline void updateParameterLocation( const int id, const void* data );

		/**
		* @brief Update a texture data location
		* @param id the texture id in the texture list returned when adding the texture
		* @param data the texture data location
		*/
		inline void RenderingFX::updateTextureLocation( const int id, Texture& oTex );
	};


	/**
	* @brief overloadable function that actives the rendering FX for the current rendering
	*/
	inline void RenderingFX::enable() const
	{
		if(GLSLshaderProgram::currentShaderProgramInUse() != m_program.getHandle())
		{
			m_program.use();
		}

		for(unsigned int i=0; i<m_texturesList.size(); i++)
		{
			glActiveTexture(m_texturesList[i]->texUnit);
			m_texturesList[i]->tex->bind();
		}
	}

	/**
	* @brief overloadable function that deactivates the rendering FX for the current rendering
	*/
	inline void RenderingFX::disable() const
	{
		m_program.use(false);
	}

	/**
	* @brief Refreshes a parameter given its id in the parameter list (see addParameter).
	* Note that there is no error handling for performance reasons, take care of the id given in parameter
	* @param id the parameter id in the parameter list returned when adding the parameter
	*/
	inline void RenderingFX::refreshParameter(const unsigned int id) const
	{
		if(GLSLshaderProgram::currentShaderProgramInUse() != m_program.getHandle())
		{
			m_program.use();
		}
		m_parametersList[id]->refresh();

	}

	/**
	* @brief Update a parameter data location
	* @param id the parameter id in the parameter list returned when adding the parameter
	* @param data the parameter data location
	*/
	inline void RenderingFX::updateParameterLocation( const int id, const void* data )
	{
		m_parametersList[ id ]->m_data  = data;
	}


	/**
	* @brief Update a texture data location
	* @param id the texture id in the texture list returned when adding the texture
	* @param data the texture data location
	*/
	inline void RenderingFX::updateTextureLocation( const int id, Texture& oTex )
	{
		m_texturesList[ id ]->tex = &oTex;
	}
}

#endif
