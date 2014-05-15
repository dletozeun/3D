#include "RenderingFX.h"


using namespace Oglf;


#define MAX(a,b) a>b?a:b

unsigned short RFXtexture::texturesUnitMax = MAX(GL_MAX_TEXTURE_COORDS, GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);


/**
 * @brief destructor, all clean up is done here
 */
RenderingFX::~RenderingFX()
{
	m_program.detachShader(*m_vertShader);
	m_program.detachShader(*m_fragShader);

	delete m_vertShader;
	delete m_fragShader;

	for(unsigned int i=0; i<m_parametersList.size(); i++)
	{
		delete m_parametersList[i];
	}
	m_parametersList.clear();
}

/**
 * @brief sets the shaders path that will be used by this effect
 * @param vsName Vertex Shader name
 * @param fsName Fragment Shader name
 */
void RenderingFX::setShaders(const char* vsName, const char* fsName)
{
	m_vertShader= new GLSLshader(VERTEX_SHADER);
	m_vertShader->setSource(vsName);
	m_fragShader= new GLSLshader(FRAGMENT_SHADER);
	m_fragShader->setSource(fsName);

	m_program.attachShader(*m_vertShader);
	m_program.attachShader(*m_fragShader);
	m_program.link();
}

/**
 * @brief adds a parameter to the rendering FX that will be used as a uniform in associated program shader
 * @param type the parameter type
 * @param size if the parameter is an array, say its size
 * @param name the name of the uniform in the associated shader program
 * @return the id of the added parameter in the parameter list (used to refresh its value next)
 */
unsigned int RenderingFX::addParameter(const void* param, RFXparamType type, const short size, const char* name)
{
	GLint location = glGetUniformLocation(m_program.getHandle(), name);

	if(type == FR_FLOAT)
	{
		RFXfloat* p = new RFXfloat((float*)param, size, location);
		m_parametersList.push_back(p);
		return m_parametersList.size()-1;
	}
	else if(type == FR_FLOAT_VEC2)
	{
		RFXfloatVec2* p = new RFXfloatVec2((float*)param, size, location);
		m_parametersList.push_back(p);
		return m_parametersList.size()-1;
	}
	else if(type == FR_FLOAT_VEC3)
	{
		RFXfloatVec3* p = new RFXfloatVec3((float*)param, size, location);
		m_parametersList.push_back(p);
		return m_parametersList.size()-1;
	}
	else if(type == FR_FLOAT_VEC4)
	{
		RFXfloatVec4* p = new RFXfloatVec4((float*)param, size, location);
		m_parametersList.push_back(p);
		return m_parametersList.size()-1;
	}
	else if(type == FR_FLOAT_MAT2)
	{
		RFXfloatMat2* p = new RFXfloatMat2((float*)param, size, location);
		m_parametersList.push_back(p);
		return m_parametersList.size()-1;
	}
	else if(type == FR_FLOAT_MAT3)
	{
		RFXfloatMat3* p = new RFXfloatMat3((float*)param, size, location);
		m_parametersList.push_back(p);
		return m_parametersList.size()-1;
	}
	else if(type == FR_FLOAT_MAT4)
	{
		RFXfloatMat4* p = new RFXfloatMat4((float*)param, size, location);
		m_parametersList.push_back(p);
		return m_parametersList.size()-1;
	}
	else if(type == FR_INT)
	{
		RFXint* p = new RFXint((int*)param, size, location);
		m_parametersList.push_back(p);
		return m_parametersList.size()-1;
	}
	else if(type == FR_INT_VEC2)
	{
		RFXintVec2* p = new RFXintVec2((int*)param, size, location);
		m_parametersList.push_back(p);
		return m_parametersList.size()-1;
	}
	else if(type == FR_INT_VEC3)
	{
		RFXintVec3* p = new RFXintVec3((int*)param, size, location);
		m_parametersList.push_back(p);
		return m_parametersList.size()-1;
	}
	else if(type == FR_INT_VEC4)
	{
		RFXintVec4* p = new RFXintVec4((int*)param, size, location);
		m_parametersList.push_back(p);
		return m_parametersList.size()-1;
	}

	return -1;
}

/**
 * @brief adds a parameter to the rendering FX that will be used as a uniform in associated program shader
 * @param tex a texture object
 * @param textureUnit the texture unit to bind the texture to
 * @param name the name of the uniform in the associated shader program
 * @return the id of the added texture in the texture list
 */
unsigned int RenderingFX::addTexture(Texture* tex, const unsigned short textureUnit, const char* name)
{
	GLint location = glGetUniformLocation(m_program.getHandle(), name);

	RFXtexture* t = new RFXtexture(tex, textureUnit);
	m_texturesList.push_back(t);

	if(GLSLshaderProgram::currentShaderProgramInUse() != m_program.getHandle())
	{
		m_program.use();
	}
	glUniform1i(location, textureUnit);

	return m_texturesList.size()-1;
}

/**
 * @brief removes a texture from the rendering FX
 * @param id the texture id in the texture list returned when adding a texture
 */
void RenderingFX::removeTexture(unsigned int id)
{
	if(id < m_texturesList.size())
	{
		m_texturesList.erase(m_texturesList.begin() + id);
	}
	else
	{
		throw Error("RenderingFX::removeTexture error: bad texture id");
	}

}

/**
 * @brief Removes a parameter from the rendering FX.
 * @param id the parameter id in the parameter list returned when adding the parameter
 */
void RenderingFX::removeParameter(const unsigned int id)
{
	if(id < m_parametersList.size())
	{
		m_parametersList.erase(m_parametersList.begin() + id);
	}
	else
	{
		throw Error("RenderingFX::removeParameter error: bad parameter id");
	}
}
