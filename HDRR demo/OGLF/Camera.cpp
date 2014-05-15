#include "Camera.h"

using namespace std;

namespace Oglf
{
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
	Camera::Camera (CameraMode camMode, GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat tgX, GLfloat tgY, GLfloat tgZ, GLuint vptWidth, GLuint vptHeight ) :

m_fovy(45.f),
m_zNear(1.f),
m_zFar(100.f),
m_viewportWidth(vptWidth),
m_viewportHeight(vptHeight),
m_yawSpeed(0.005f),
m_pitchSpeed(0.005f),
m_projectionType(PERSPECTIVE),
m_xMin(-1.f),
m_yMin(-1.f),
m_xMax(1.f),
m_yMax(1.f),
m_camMode(camMode)
{
	m_position.v[0] = posX;
	m_position.v[1] = posY;
	m_position.v[2] = posZ;

	m_targetPos.v[0] = tgX;
	m_targetPos.v[1] = tgY;
	m_targetPos.v[2] = tgZ;

	m_viewAxis = Vec3(0.f, 0.f, -1.f);
	m_vertAxis = Vec3(0.f, 1.f, 0.f);
	m_strafeAxis = Vec3(1.f, 0.f, 0.f);

	setTarget(tgX, tgY, tgZ);

	m_aspect=(GLdouble)m_viewportWidth/(GLdouble)m_viewportHeight;

	m_moveSpeed[0] = 0.01f;
	m_moveSpeed[1] = 0.01f;
	m_moveSpeed[2] = 0.1f;

	/*cout<<"position: "<<pos<<endl;
	cout<<"viewAxis: "<<viewAxis<<endl;
	cout<<"vertAxis: "<<vertAxis<<endl;
	cout<<"strafeAxis: "<<strafeAxis<<endl<<endl;*/
}


/**
* @brief Camera constructor initialize a Camera instance
* @param  position the camera position
* @param  tgPos the target position
* @param  vptWidth the viewport width
* @param  vptHeight the viewport height
*/
Camera::Camera (CameraMode camMode, Vec3& position, Vec3& tgPos, GLuint vptWidth, GLuint vptHeight ) :

m_fovy(45.f),
m_zNear(1.f),
m_zFar(100.f),
m_viewportWidth(vptWidth),
m_viewportHeight(vptHeight),
m_yawSpeed(0.005f),
m_pitchSpeed(0.005f),
m_projectionType(PERSPECTIVE),
m_xMin(-1.f),
m_yMin(-1.f),
m_xMax(1.f),
m_yMax(1.f),
m_camMode(camMode)
{
	m_position = position;
	m_targetPos = tgPos;
	//this->getTransformer().setPivotPosition(pos->x, pos->y, pos->z);

	m_viewAxis = Vec3(0.f, 0.f, -1.f);
	m_vertAxis = Vec3(0.f, 1.f, 0.f);
	m_strafeAxis = Vec3(1.0, 0.f, 0.f);

	setTarget(tgPos.v[0], tgPos.v[1], tgPos.v[2]);

	m_aspect=(GLdouble)m_viewportWidth/(GLdouble)m_viewportHeight;

	m_moveSpeed[0] = 0.01f;
	m_moveSpeed[1] = 0.01f;
	m_moveSpeed[2] = 0.1f;

	//	cout<<"position: "<<m_position<<endl;
	//	cout<<"viewAxis: "<<m_viewAxis<<endl;
	//	cout<<"vertAxis: "<<m_vertAxis<<endl;
	//	cout<<"strafeAxis: "<<m_strafeAxis<<endl<<endl;
}

}
