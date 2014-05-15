#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>
#include <iostream>
#ifdef WIN32
#include <Windows.h>
#endif

#include <GL/GLU.h>

#include "Object3D.h"
#include "Vec.h"

namespace Oglf
{
#define RADCONV	   0.0174532925f // Pi/180.0

	enum ProjType
	{
		PERSPECTIVE,
		ORTHO
	};

	enum CameraMode
	{
		FREE_CAMERA,
		TARGET_CAMERA,
	};

	/**
	* class Camera manages the 3D camera
	*/
	class Camera : public Object3D
	{
		Vec3     m_position;
		Vec3     m_targetPos;
		Vec3     m_viewAxis;
		Vec3     m_vertAxis;
		Vec3     m_strafeAxis;
		GLdouble   m_fovy;
		GLdouble   m_aspect;
		GLdouble   m_zNear;
		GLdouble   m_zFar;
		GLuint     m_viewportWidth;
		GLuint     m_viewportHeight;
		GLuint     m_viewportX;
		GLuint     m_viewportY;
		float      m_yawSpeed;          // camera yaw speed
		float      m_pitchSpeed;        // camera pitch speed
		float      m_moveSpeed[3];      // camera movement speed on X, Y and Z axis
		ProjType   m_projectionType;
		GLfloat    m_xMin;
		GLfloat    m_yMin;
		GLfloat    m_xMax;
		GLfloat    m_yMax;
		CameraMode m_camMode;

	public:

		friend class Renderer;

		/**
		* @brief Camera constructor initialize a Camera instance
		* @param  posX the x camera position
		* @param  posY the y camera position
		* @param  posZ the z camera position
		* @param  tgX  the x target position
		* @param  tgY the y target position
		* @param  tgZ the z target position
		* @param  vptWidth the viewport width
		* @param  vptHeigh the viewport height
		*/
		Camera (CameraMode camMode, GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat tgX, GLfloat tgY, GLfloat tgZ, GLuint vptWidth, GLuint vptHeight );

		/**
		* @brief Camera constructor initialize a Camera instance
		* @param  position the camera position
		* @param  tgPos the target position
		* @param  vptWidth the viewport width
		* @param  vptHeight the viewport height
		*/
		Camera (CameraMode camMode, Vec3& position, Vec3& tgPos, GLuint vptWidth, GLuint vptHeight );

		Vec3& getPosition()
		{
			return m_position;
		}

		Vec3& getTarget()
		{
			return m_targetPos;
		}

		Vec3& getViewAxis()
		{
			return m_viewAxis;
		}

		Vec3& getStrafeAxis()
		{
			return m_strafeAxis;
		}

		Vec3& getVerticalAxis()
		{
			return m_vertAxis;
		}
		
		/**
		* @param  x x component of position vector
		* @param  y y component of position vector
		* @param  z z component of position vector
		*/
		void setPosition (GLfloat x, GLfloat y, GLfloat z )
		{
			m_position.v[0]=x;
			m_position.v[1]=y;
			m_position.v[2]=z;
		}

		/**
		* @param  p a blVect vector that contains the camera position
		*/
		void setPosition(Vec3& p)
		{
			m_position.v[0]=p[0];
			m_position.v[1]=p[1];
			m_position.v[2]=p[2];
		}

		/**
		* @param  x x component of target vector
		* @param  y y component of target vector
		* @param  z z component of target vector
		*/
		void setTarget (GLfloat x, GLfloat y, GLfloat z )
		{
			m_targetPos.v[0] = x;
			m_targetPos.v[1] = y;
			m_targetPos.v[2] = z;

			// build the new view axis
			m_viewAxis = Vec3(x - m_position.v[0], y - m_position.v[1], z - m_position.v[2]);
			m_viewAxis.normalize();

			m_strafeAxis = m_viewAxis ^ Vec3(0.0, 1.0, 0.0);
			m_strafeAxis.normalize();
			m_vertAxis = m_strafeAxis ^ m_viewAxis;

			Matrix4x4 rotMatrix;
			rotMatrix.m[0] = m_strafeAxis.v[0];
			rotMatrix.m[1] = m_vertAxis.v[0];
			rotMatrix.m[2] = -m_viewAxis.v[0];
			rotMatrix.m[3] = 0.0;

			rotMatrix.m[4] = m_strafeAxis.v[1];
			rotMatrix.m[5] = m_vertAxis.v[1];
			rotMatrix.m[6] = -m_viewAxis.v[1];
			rotMatrix.m[7] = 0.0;

			rotMatrix.m[8] = m_strafeAxis.v[2];
			rotMatrix.m[9] = m_vertAxis.v[2];
			rotMatrix.m[10] = -m_viewAxis.v[2];
			rotMatrix.m[11] = 0.0;

			rotMatrix.m[12] = 0.0;
			rotMatrix.m[13] = 0.0;
			rotMatrix.m[14] = 0.0;
			rotMatrix.m[15] = 1.0;

			this->getTransformer().getTransformMatrix() = rotMatrix;
		}

		/**
		* @param  tg a blVect vector that contains the camera target position
		*/
		void setTarget(Vec3& tg)
		{
			setTarget(tg.v[0], tg.v[1], tg.v[2]);
		}

		/**
		* @brief sets perspective projection
		* @param fov field of view (in degrees)
		* @param near the near clipping plane distance from the eye
		* @param far the far clipping plane distance from the eye
		*/
		void setPerspectiveProj (GLdouble fovy, GLdouble Znear, GLdouble Zfar )
		{
			this->m_fovy=fovy;
			this->m_zNear=Znear;
			this->m_zFar=Zfar;

			m_projectionType=PERSPECTIVE;
		}

		/**
		* @brief sets orthographic projection
		* @param xmin the minimum x value
		* @param xmax the maximum x value
		* @param ymin the minimum y value
		* @param ymax the maximum y value
		*/
		void setOrthoProj(GLfloat xmin, GLfloat xmax, GLfloat ymin, GLfloat ymax, GLfloat Znear, GLfloat Zfar)
		{
			this->m_xMin=xmin;
			this->m_yMin=ymin;
			this->m_xMax=xmax;
			this->m_yMax=ymax;
			this->m_zNear=Znear;
			this->m_zFar=Zfar;

			m_projectionType=ORTHO;
		}

		/**
		* @brief sets the camera active in the OpenGL scene
		*/
		void setActive()
		{
			glViewport(m_viewportX, m_viewportY, m_viewportWidth, m_viewportHeight);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();

			if(m_projectionType==PERSPECTIVE)	gluPerspective(m_fovy, m_aspect, m_zNear, m_zFar);
			else
			{
				// sets a 2D orthographic projection
				glOrtho(m_xMin, m_xMax, m_yMin, m_yMax, m_zNear, m_zFar);
			}
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();

			this->getTransformer().transform();
			glTranslatef(-m_position.v[0], -m_position.v[1], -m_position.v[2]);

			//const float* tPos = this->getTransformer().getPivotPosition();
			//gluLookAt(tPos[0], tPos[1], tPos[2], tg->x, tg->y, tg->z, vAxis.x, vAxis.y, vAxis.z);
		}

		/**
		* @brief sets the camera vertical axis
		* @param  x x component of vertical axis vector
		* @param  y y component of vertical axis vector
		* @param  z z component of vertical axis vector
		*/
		void setVertAxis (GLfloat x, GLfloat y, GLfloat z )
		{
			m_vertAxis.v[0] = x;
			m_vertAxis.v[1] = y;
			m_vertAxis.v[2] = z;
		}

		/**
		* @brief sets camera pitch speed
		* @param  s the camera pitch speed
		*/
		void setPitchSpeed(float s)
		{
			m_pitchSpeed = s;
		}

		/**
		* @brief sets camera yaw speed
		* @param  s the camera yaw speed
		*/
		void setYawSpeed(float s)
		{
			m_yawSpeed = s;
		}

		/**
		* @brief sets camera X movement speed
		* @param s the camera X movement speed
		*/
		void setMoveXspeed(float s)
		{
			m_moveSpeed[0]=s;
		}

		/**
		* @brief sets camera Y movement speed
		* @param s the camera Y movement speed
		*/
		void setMoveYspeed(float s)
		{
			m_moveSpeed[1]=s;
		}

		/**
		* @brief sets camera Z movement speed
		* @param s the camera Z movement speed
		*/
		void setMoveZspeed(float s)
		{
			m_moveSpeed[2]=s;
		}

		/**
		* @brief sets the camera movement mode
		* @param mode the camera movement mode
		*/
		void setCameraMode(CameraMode mode)
		{
			m_camMode = mode;
			if(mode == TARGET_CAMERA) {
				setTarget(m_targetPos.v[0], m_targetPos.v[1], m_targetPos.v[2]);
			}
		}

		/**
		* @brief sets the viewport size
		* @param x the viewport x position
		* @param y the viewport y position
		* @param  w the viewport width
		* @param  h the viewport height
		*/
		void initViewport(GLuint x, GLuint y, GLuint w, GLuint h)
		{
			m_viewportX=x;
			m_viewportY=y;
			m_viewportWidth=w;
			m_viewportHeight=h;
			m_aspect=(float)w/((float)h+1.0);
		}

		/**
		* @brief recompute the camera target position when we look around in the scene with the mouse
		* @param dx mouse position horizontal variation
		* @param dy mouse position vertical variation
		*/
		void reComputeTargetPos (int dx, int dy )
		{
			float dPitch = (float)dy * m_pitchSpeed * (180.f / (float)M_PI);
			float dYaw = (float)dx * m_yawSpeed * (180.f / (float)M_PI);

			Quaternion pitchQ, yawQ;
			Matrix4x4 pitchMat, yawMat;

			yawQ.createRotation(dYaw, 0.f, 1.f, 0.f);
			yawQ.createRotationMatrix(yawMat.m);
			this->getTransformer().getTransformMatrix() = this->getTransformer().getTransformMatrix() * yawMat;

			yawMat.transpose();
			m_strafeAxis = yawMat * m_strafeAxis;
			m_viewAxis   = yawMat * m_viewAxis;
			m_vertAxis   = yawMat * m_vertAxis;
			if(m_camMode == TARGET_CAMERA) {
				m_position = yawMat * (m_position - m_targetPos) + m_targetPos;
			}

			pitchQ.createRotation(dPitch, m_strafeAxis.v[0], m_strafeAxis.v[1], m_strafeAxis.v[2]);
			pitchQ.createRotationMatrix(pitchMat.m);
			this->getTransformer().getTransformMatrix() = this->getTransformer().getTransformMatrix() * pitchMat;

			pitchMat.transpose();
			m_strafeAxis = pitchMat * m_strafeAxis;
			m_viewAxis   = pitchMat * m_viewAxis;
			m_vertAxis   = pitchMat * m_vertAxis;
			if(m_camMode == TARGET_CAMERA) {
				m_position = pitchMat * (m_position - m_targetPos) + m_targetPos;
			}

			// compensate the camera vertical axis inversion
			if(m_vertAxis.v[1] < 0.f) {
				m_yawSpeed = -fabs(m_yawSpeed);
			}
			else {
				m_yawSpeed = fabs(m_yawSpeed);
			}

			/*std::cout<<"position: "<<pos<<std::endl;
			std::cout<<"viewAxis: "<<viewAxis<<std::endl;
			std::cout<<"vertAxis: "<<vertAxis<<std::endl;
			std::cout<<"strafeAxis: "<<strafeAxis<<std::endl<<std::endl;*/
		}

		void moveX(int dx)
		{
			m_position= m_position + m_strafeAxis * (float)dx * m_moveSpeed[0];
			if(m_camMode == TARGET_CAMERA)
			{
				m_targetPos= m_targetPos + m_strafeAxis * (float)dx * m_moveSpeed[0];
				//setTarget(m_targetPos.v[0], m_targetPos.v[1], m_targetPos.v[2]);
			}
		}

		void moveY(int dy)
		{
			m_position= m_position + m_vertAxis * (float)dy * m_moveSpeed[1];
			if(m_camMode == TARGET_CAMERA)
			{
				m_targetPos= m_targetPos + m_vertAxis * (float)dy * m_moveSpeed[1];
				//setTarget(m_targetPos.v[0], m_targetPos.v[1], m_targetPos.v[2]);
			}
		}

		void moveZ(int dz)
		{
			m_position= m_position + m_viewAxis * (float)dz * m_moveSpeed[2];
		}

		/**
		* @brief returns the camera position
		* @return m_position the camera position
		*/
		const Vec3& getPosition ( ) const
		{
			return m_position;
		}

		/**
		* @brief returns the camera target
		* @return m_targetPos the camera target
		*/
		const Vec3& getTarget ( ) const
		{
			return m_targetPos;
		}

		/**
		* @brief returns the viewport width
		* @return the viewport width
		*/
		int getViewportWidth()
		{
			return m_viewportWidth;
		}

		/**
		* @brief returns the viewport height
		* @return the viewport height
		*/
		int getViewportHeight()
		{
			return m_viewportHeight;
		}

		/**
		* @brief returns the viewport x offset from the lower left corner
		* @return the viewport x offset
		*/
		int getViewportX()
		{
			return m_viewportX;
		}

		/**
		* @brief returns the viewport y offset from the lower left corner
		* @return the viewport y offset
		*/
		int getViewportY()
		{
			return m_viewportY;
		}
	};
}

#endif // CAMERA_H
