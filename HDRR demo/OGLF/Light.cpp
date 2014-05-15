#ifdef WIN32
#include <Windows.h>
#endif
#include <GL/gl.h>
#include "Light.h"

using namespace std;

namespace Oglf
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Light >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

	/**
	* @brief constructor
	* @param W W=1 omnidirectional light, W=0 directional light
	* @param castShadows is this light casts shadows
	*/
	Light::Light(bool shadowsOn) :
m_kc(1.0), m_kl(0.0), m_kq(0.0), m_shadowsOn(shadowsOn), m_glID(0), m_renderable(false)
{
	glGetIntegerv(GL_MAX_LIGHTS, &m_maxLightNumber);

	// white default diffuse color
	m_diffuseColor[0]= 1.0;
	m_diffuseColor[1]= 1.0;
	m_diffuseColor[2]= 1.0;
	m_diffuseColor[3]= 0.0;

	// white default specular color
	m_specularColor[0]= 1.0;
	m_specularColor[1]= 1.0;
	m_specularColor[2]= 1.0;
	m_specularColor[3]= 0.0;

	// black default ambient color
	m_ambientColor[0]= 0.0;
	m_ambientColor[1]= 0.0;
	m_ambientColor[2]= 0.0;
	m_ambientColor[3]= 0.0;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< SpotLight >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/**
* @brief constructor
* @param posX light position vector projected on X axis
* @param posY light position vector projected on Y axis
* @param posZ light position vector projected on Z axis
* @param tgPosX target position vector projected on X axis
* @param tgPosY target position vector projected on Y axis
* @param tgPosZ target position vector projected on Z axis
* @param castShadows is this light casts shadows
* @param cutoff spot light cone demi angle value
* @param spotExpnt spot light exponent value
*/
SpotLight::SpotLight(GLfloat posX, GLfloat posY, GLfloat posZ,
					 GLfloat dirX, GLfloat dirY, GLfloat dirZ,
					 GLfloat cutoff, GLuint spotExpnt, bool shadows) :
Light(shadows)
{
	this->setPosition(posX, posY, posZ);
	this->getTransformer().setPivotPosition(posX, posY, posZ);
	m_pos[3]=1.0; // point light (OpenGL uses the 4th position component to determines if this light is directional or omnidirectional

	this->m_cutoff=cutoff;
	this->m_expnt=spotExpnt;
}

/**
* @brief constructor
* @param pos light position vector
* @param tgPos target position vector
* @param castShadows is this light casts shadows
* @param cutoff spot light cone demi angle value
* @param spotExpnt spot light exponent value
*/
SpotLight::SpotLight(GLfloat* pos, GLfloat* dir,
					 GLfloat cutoff, GLuint spotExpnt, bool shadows)
{
	this->setPosition(pos);
	this->getTransformer().setPivotPosition(pos[0], pos[1], pos[2]);
	pos[3]=1.0; // point light (OpenGL uses the 4th position component to determines if this light is directional or omnidirectional

	this->m_cutoff=cutoff;
	this->m_expnt=spotExpnt;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< OmniLight >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/**
* @brief constructor
* @param posX light position vector projected on X axis
* @param posY light position vector projected on Y axis
* @param posZ light position vector projected on Z axis
* @param shadows is this light casts shadows
*/
OmniLight::OmniLight(GLfloat posX, GLfloat posY, GLfloat posZ, bool shadows) : Light(shadows)
{
	this->setPosition(posX, posY, posZ);
	this->getTransformer().setPivotPosition(posX, posY, posZ);
	m_pos[3]=1.0; // point light (OpenGL uses the 4th position component to determines if this light is directional or omnidirectional
}

/**
* @brief constructor
* @param pos light position vector
* @param shadows is this light casts shadows
*/
OmniLight::OmniLight(GLfloat* pos, bool shadows) : Light(shadows)
{
	this->setPosition(pos);
	this->getTransformer().setPivotPosition(pos[0], pos[1], pos[2]);
	pos[3]=1.0; // point light (OpenGL uses the 4th position component to determines if this light is directional or omnidirectional
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<< DirectionalLight >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/**
* @brief constructor
* @param dir a GLfloat array that contains the light direction
* @param shadows enable or disable shadows for this light
*/
DirectionalLight::DirectionalLight(GLfloat* dir, bool shadows) : Light(shadows)
{
	this->setDirection(dir);
	m_direction[3]=0.0; // directional light (OpenGL uses the 4th position component to determines if this light is directional or omnidirectional
}

/**
* @brief constructor
* @param pos light position vector
* @param tgPos target position vector
* @param shadows enable or disable shadows for this light
*/
DirectionalLight::DirectionalLight(GLfloat dirX, GLfloat dirY, GLfloat dirZ, bool shadows) : Light(shadows)
{
	this->setDirection(dirX, dirY, dirZ);
	m_direction[3]=0.0; // directional light (OpenGL uses the 4th position component to determines if this light is directional or omnidirectional)
}


}
