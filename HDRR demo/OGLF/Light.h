#ifndef LIGHT_H
#define LIGHT_H

#include "Object3D.h"
#ifdef WIN32
#include <Windows.h>
#endif
#include <GL/glut.h>


namespace Oglf
{
	class Light : public Object3D
	{
		GLfloat m_kc, m_kl, m_kq; // constant, linear and quadratic attenuations

		bool m_shadowsOn; // enable shadows for this light or not

	protected:

		GLfloat m_diffuseColor[4];  // light diffuse color
		GLfloat m_specularColor[4]; // light specular color
		GLfloat m_ambientColor[4];  // light ambient color

		GLint m_glID;               // the number of the opengl light to which this light is bound
		GLint m_maxLightNumber;
		bool m_renderable;          // enable or disable the rendering of the light as a yellow sphere

	public:

		Light(bool shadowsOn=false);
		virtual ~Light() {}

		/**
		* @brief enable or disable the light rendering
		* @param renderable true: light is rendered, false: light is not rendered
		*/
		void setRenderable(bool renderable)
		{
			m_renderable= renderable;
		}

		/**
		* @brief draws the light
		*/
		virtual void draw()
		{

		}

		/**
		* @brief sets the light diffuse color
		* @param color a GLfloat array that contains rgba color
		*/
		void setDiffuseColor(GLfloat* color)
		{
			m_diffuseColor[0]=color[0];
			m_diffuseColor[1]=color[1];
			m_diffuseColor[2]=color[2];
			m_diffuseColor[3]=color[3];
		}

		/**
		* @brief sets the light diffuse color
		* @param r red component
		* @param g green component
		* @param b blue component
		*/
		void setDiffuseColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
		{
			m_diffuseColor[0]=r;
			m_diffuseColor[1]=g;
			m_diffuseColor[2]=b;
			m_diffuseColor[3]=a;
		}

		/**
		* @brief sets the light specular color
		* @param color a GLfloat array that contains rgba color
		*/
		void setSpecularColor(GLfloat* color)
		{
			m_specularColor[0]=color[0];
			m_specularColor[1]=color[1];
			m_specularColor[2]=color[2];
			m_specularColor[3]=color[3];
		}

		/**
		* @brief sets the light specular color
		* @param r red component
		* @param g green component
		* @param b blue component
		*/
		void setSpecularColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
		{
			m_specularColor[0]=r;
			m_specularColor[1]=g;
			m_specularColor[2]=b;
			m_specularColor[3]=a;
		}

		/**
		* @brief sets the light ambient Color
		* @param color a GLfloat array that contains rgba color
		*/
		void setAmbientColor(GLfloat* color)
		{
			m_ambientColor[0]=color[0];
			m_ambientColor[1]=color[1];
			m_ambientColor[2]=color[2];
			m_ambientColor[3]=color[3];
		}

		/**
		* @brief sets the light ambient Color
		* @param r red component
		* @param g green component
		* @param b blue component
		*/
		void setAmbientColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
		{
			m_ambientColor[0]=r;
			m_ambientColor[1]=g;
			m_ambientColor[2]=b;
			m_ambientColor[3]=a;
		}

		/**
		* @brief sets the light attenation factors. Attenuation is 1/(kc + d*kl + d²*kq) where d is the fragment-light distance
		* @param kc the constant attenuation factor
		* @param kl the linear attenuation factor
		* @param kq the quadratic attenuation factor
		*/
		void setAttenuation(GLfloat kc, GLfloat kl, GLfloat kq)
		{
			m_kc=kc;
			m_kl=kl;
			m_kq=kq;
		}

		/**
		* @brief returns the light diffuse color
		* @return the light diffuse color
		*/
		const GLfloat* const getDiffuseColor() const
		{
			return m_diffuseColor;
		}

		/**
		* @brief returns the light specular color
		* @return the light specular color
		*/
		const GLfloat* const getSpecularColor() const
		{
			return m_specularColor;
		}

		/**
		* @brief returns the light ambient color
		* @return the light ambient color
		*/
		const GLfloat* const getAmbientColor() const
		{
			return m_ambientColor;
		}

		/**
		* @brief enable or disable shadows for this light
		*/
		void shadows(bool state)
		{
			m_shadowsOn=state;
		}

		/**
		* @brief disable the opengl light which number is given in the parameter
		* @param the opengl light number
		*/
		static void disableGLlight(GLint id)
		{
			glDisable(GL_LIGHT0 + id);
		}

		/**
		* @brief binds this light to the openGL light which number is given in parameter
		* @param the opengl light number to bind to (-1 means to use the stored light number: glID (0 initially))
		*/
		virtual void bind(GLint id=-1) = 0;

		/**
		* @brief apply model transformations to the light before drawing
		*/
		virtual void transfom()
		{
			Object3D::transform();
		}
	};

	class SpotLight : public Light
	{
		GLfloat m_pos[4]; // light position (the 4th element is used by opengl to determine if this light is omnidirectional or directional
		GLfloat m_cutoff; // half angle between light direction and the border of the spot light cone
		GLfloat m_direction[3]; // light direction
		GLuint m_expnt; // Spot cone attenuation exponent

	public:
		SpotLight(GLfloat* pos, GLfloat* dir,
			GLfloat cutoff=45.0f, GLuint spotExpnt=20, bool shadows=false);

		SpotLight(GLfloat posX, GLfloat posY, GLfloat posZ,
			GLfloat dirX, GLfloat dirY, GLfloat dirZ,
			GLfloat cutoff=45.0f, GLuint spotExpnt=20, bool shadows=false);
		virtual ~SpotLight() {};

		/**
		* @brief draws the light
		*/
		void draw()
		{
			GLboolean lighting = glIsEnabled(GL_LIGHTING);
			if(m_renderable)
			{
				if(lighting) {
					glDisable(GL_LIGHTING);
				}

				glColor3f(1.0, 1.0, 0.0);
				//glutSolidSphere(0.1, 4, 2);

				if(lighting) {
					glEnable(GL_LIGHTING);
				}
			}
		}

		/**
		* @brief sets the light position
		* @param x light position on x axis
		* @param y light position on y axis
		* @param z light position on z axis
		*/
		void setPosition(GLfloat x, GLfloat y, GLfloat z)
		{
			m_pos[0]=x;
			m_pos[1]=y;
			m_pos[2]=z;
			this->getTransformer().setPivotPosition(x, y, z);
		}

		/**
		* @brief sets the light position
		* @param pos a GLfloat array that contains the light position
		*/
		void setPosition(GLfloat* pos)
		{
			this->m_pos[0]=pos[0];
			this->m_pos[1]=pos[1];
			this->m_pos[2]=pos[2];
			this->getTransformer().setPivotPosition(pos[0], pos[1], pos[2]);
		}

		/**
		* @brief returns the light position
		* @return the light position
		*/
		const GLfloat* getPosition()
		{
			return this->getTransformer().getPivotPosition();
		}

		/**
		* @brief binds this light to the openGL light which number is given in parameter
		* @param the opengl light number to bind to (-1 means to use the stored light number: glID (0 initially))
		*/
		void bind(GLint id=-1)
		{
			if(id>m_maxLightNumber-1 || id<-1) {
				throw Error("Light::binTo error: cant bind a light to a light ID negative or greater than GL_MAX_LIGHTS");
			}
			else
			{
				if(id!=-1) {
					m_glID=id;
				}
				GLenum light=GL_LIGHT0 + m_glID; // the light number : GL_LIGHT[0-GL_MAX_LIGHTS]

				glLightfv(light, GL_AMBIENT, m_ambientColor);
				glLightfv(light, GL_DIFFUSE, m_diffuseColor);
				glLightfv(light, GL_SPECULAR, m_specularColor);
				glLightfv(light, GL_POSITION, m_pos);
				glLightf(light, GL_SPOT_CUTOFF, m_cutoff);
				glLightfv(light, GL_SPOT_DIRECTION, m_direction);
				glLighti(light, GL_SPOT_EXPONENT, m_expnt);
				glEnable(GL_LIGHTING);
				glEnable(light);
			}
		}

		/**
		* @brief apply model transformations to the light before drawing
		*/
		void transform()
		{
			Object3D::transform();
			glLightfv(GL_LIGHT0 + m_glID, GL_POSITION, m_pos);
		}
	};

	class OmniLight : public Light
	{
		GLfloat m_pos[4]; // light position (the 4th element is used by opengl to determine if this light is omnidirectional or directional

	public:

		/**
		* @brief constructor
		* @param posX light position vector projected on X axis
		* @param posY light position vector projected on Y axis
		* @param posZ light position vector projected on Z axis
		* @param shadows is this light casts shadows
		*/
		OmniLight(GLfloat posX, GLfloat posY, GLfloat posZ, bool shadows=false);

		/**
		* @brief constructor
		* @param pos light position vector
		* @param shadows is this light casts shadows
		*/
		OmniLight(GLfloat* pos, bool shadows=false);

		virtual ~OmniLight() {}

		/**
		* @brief draws the light
		*/
		void draw()
		{
			if(m_renderable)
			{
				glTranslatef(m_pos[0], m_pos[1], m_pos[2]);
				glDisable(GL_LIGHTING);
				glColor3f(1.0, 1.0, 0.0);
				//glutSolidSphere(0.1, 4, 2);
				glEnable(GL_LIGHTING);
			}
		}

		/**
		* @brief sets the light position
		* @param x light position on x axis
		* @param y light position on y axis
		* @param z light position on z axis
		*/
		void setPosition(GLfloat x, GLfloat y, GLfloat z)
		{
			m_pos[0]=x;
			m_pos[1]=y;
			m_pos[2]=z;
			this->getTransformer().setPivotPosition(x, y, z);
		}

		/**
		* @brief sets the light position
		* @param pos a GLfloat array that contains the light position
		*/
		void setPosition(GLfloat* pos)
		{
			this->m_pos[0]=pos[0];
			this->m_pos[1]=pos[1];
			this->m_pos[2]=pos[2];
			this->getTransformer().setPivotPosition(pos[0], pos[1], pos[2]);
		}

		/**
		* @brief returns the light position
		* @return the light position
		*/
		const GLfloat* getPosition()
		{
			return this->getTransformer().getPivotPosition();
		}

		/**
		* @brief binds this light to the openGL light which number is given in parameter
		* @param the opengl light number to bind to (-1 means to use the stored light number: glID (0 initially))
		*/
		void bind(GLint id=-1)
		{
			if (id>m_maxLightNumber-1 || id<-1)
			{
				throw Error("Light::binTo error: cant bind a light to a light ID negative or greater than GL_MAX_LIGHTS");
			}
			else
			{
				if (id!=-1)
				{
					m_glID=id;
				}
				GLenum light=GL_LIGHT0 + m_glID; // the light number : GL_LIGHT[0-GL_MAX_LIGHTS]

				glLightfv(light, GL_AMBIENT, m_ambientColor);
				glLightfv(light, GL_DIFFUSE, m_diffuseColor);
				glLightfv(light, GL_SPECULAR, m_specularColor);
				glLightfv(light, GL_POSITION, m_pos);
				glEnable(GL_LIGHTING);
				glEnable(light);
			}
		}

		/**
		* @brief apply model transformations to the light before drawing
		*/
		void transform()
		{
			Object3D::transform();
			glLightfv(GL_LIGHT0 + m_glID, GL_POSITION, m_pos);
		}
	};

	class DirectionalLight : public Light
	{
		GLfloat m_direction[4]; // light direction

	public:

		/**
		* @brief constructor
		* @param posX light position vector projected on X axis
		* @param posY light position vector projected on Y axis
		* @param posZ light position vector projected on Z axis
		* @param shadows is this light casts shadows
		*/
		DirectionalLight(GLfloat* dir, bool shadows=false);

		/**
		* @brief constructor
		* @param pos light position vector
		* @param shadows is this light casts shadows
		*/
		DirectionalLight(GLfloat dirX, GLfloat dirY, GLfloat dirZ, bool shadows=false);

		/**
		* @brief sets the light direction
		* @param x light direction on x axis
		* @param y light direction on y axis
		* @param z light direction on z axis
		*/

		virtual ~DirectionalLight() {}

		void setDirection(GLfloat x, GLfloat y, GLfloat z)
		{
			m_direction[0]=x;
			m_direction[1]=y;
			m_direction[2]=z;
		}

		/**
		* @brief sets the light direction
		* @param pos a GLfloat array that contains the light direction
		*/
		void setDirection(GLfloat* dir)
		{
			this->m_direction[0]=dir[0];
			this->m_direction[1]=dir[1];
			this->m_direction[2]=dir[2];
		}

		/**
		* @brief returns the light position
		* @return the light position
		*/
		const GLfloat* const getDirection() const
		{
			return m_direction;
		}

		/**
		* @brief binds this light to the openGL light which number is given in parameter
		* @param the opengl light number to bind to (-1 means to use the stored light number: glID (0 initially))
		*/
		void bind(GLint id=-1)
		{
			if (id>m_maxLightNumber-1 || id<0)
			{
				throw Error("Light::binTo error: cant bind a light to a light ID negative or greater than GL_MAX_LIGHTS");
			}
			else
			{
				if (id!=-1)
				{
					m_glID=id;
				}
				GLenum light=GL_LIGHT0 + m_glID; // the light number : GL_LIGHT[0-GL_MAX_LIGHTS]

				glLightfv(light, GL_AMBIENT, m_ambientColor);
				glLightfv(light, GL_DIFFUSE, m_diffuseColor);
				glLightfv(light, GL_SPECULAR, m_specularColor);
				glLightfv(light, GL_POSITION, m_direction);
				glEnable(GL_LIGHTING);
				glEnable(light);
			}
		}

		/**
		* @brief apply model transformations to the light before drawing
		*/
		void transform()
		{
			Object3D::transform();
			glLightfv(GL_LIGHT0 + m_glID, GL_POSITION, m_direction);
		}
	};
}

#endif
