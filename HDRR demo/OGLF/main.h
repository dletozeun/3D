#ifndef MAIN_H
#define MAIN_H

#include "OGLframeworkBase.h"

bool g_bRun = true;

int wWidth = 800;                      // window width
int wHeight = 600;                     // window height
int frame = 0;                         // for fps stats
char fpsString[8];                     // string to display to frame per second number on screen

// Camera setup
Oglf::Camera* cam;
Oglf::CameraMode cameraMode = Oglf::TARGET_CAMERA;
Oglf::Vec3 cameraPos(2.0, 2.0, 2.0);
Oglf::Vec3 targetPos(0.0, 0.0, 0.0);
bool rotateCamera = false;             // camera rotation is activated with a left mouse click on the window
bool trackCamera    = false;           // camera panning is activated with a middle mouse click on the window
bool zoomCamera   = false;             // camera zooming is activated with a right mouse click on the window
int g_iLastMouseX;                                // last mouse x position
int g_iLastMouseY;                                // last mouse y position

Oglf::Renderer* r;

//simpleFX
Oglf::RenderingFX* simpleFX;
float color[4] = {1.0, 1.0, 0.0, 0.0};
int colorParamID;

class TestRenderingConfiguration : public Oglf::RenderingConfiguration
{
public:
	TestRenderingConfiguration(Oglf::Renderer& renderer) : RenderingConfiguration(renderer)
	{

	}

	void configureRenderer()
	{
		m_renderer->setLighting(true);
		m_renderer->setClearColor(0.2f, 0.0f, 0.2f, 0.0f);
		m_renderer->setWireframe(false);
		//m_renderer->setTextureMapping2D(false);
	}
};

class ObjMesh : public Oglf::Mesh
{
	void draw()
	{
		glEnable(GL_NORMALIZE);
		// draw mesh geometry
		glBegin(GL_TRIANGLES);
		for (unsigned int i = 0; i < vertIndices.size(); i++)
		{
			glTexCoord2f(uv[2 * uvIndices[i]], uv[2 * uvIndices[i] + 1]);

			// smooth shading
			glNormal3fv(vertNormals[vertIndices[i]].v);
			//flat shading
			//glNormal3fv(facesNormals[i/3].v);

			glVertex3fv(vertices[vertIndices[i]].v);
		}
		glEnd();


//		glDisable(GL_LIGHTING);
//
//		glColor3f(1.0, 1.0, 0.0);
//		glBegin(GL_LINES);
//			for (unsigned int i = 0; i < vertIndices.size(); i++)
//			{
//				glVertex3fv(vertices[vertIndices[i]].v);
//				glVertex3fv((vertices[vertIndices[i]] + facesNormals[i/3] / 20.0).v);
//				//std::cout<<"face normal: "<<facesNormals[i/3].v[0]<<" "<<facesNormals[i/3].v[1]<<" "<<facesNormals[i/3].v[2]<<std::endl;
//			}
//		glEnd();
//
//		glBegin(GL_LINES);
//			for (unsigned int i=0; i<vertices.size(); i++)
//			{
//				glColor3f(1.0, 0.0, 0.0);
//				glVertex3fv(vertices[i].v);
//				glVertex3fv((vertices[i] + vertNormals[i]/20.0).v);
//
//				glColor3f(0.0, 1.0, 0.0);
//				glVertex3fv(vertices[i].v);
//				glVertex3fv((vertices[i] + vertTangents[i]/20.0).v);
//
//				glColor3f(0.0, 0.0, 1.0);
//				glVertex3fv(vertices[i].v);
//				glVertex3fv((vertices[i] + vertBinormals[i]/20.0).v);
//			}
//		glEnd();
//
//		glEnable(GL_LIGHTING);
	}
};

#endif
