#ifndef DEMO_MAIN_H
#define DEMO_MAIN_H

#include "OGLframeworkBase.h"

bool g_bRun = true;

int wWidth = 1024;                      // window width
int wHeight = 512;                      // window height

// Hud
Oglf::Hud* g_pHelpHud;
bool	   g_bDisplayHelp = true;


// Camera setup
Oglf::CameraMode g_eCameraMode = Oglf::TARGET_CAMERA;

bool	g_bRotateCamera = false;        // camera rotation is activated with a left mouse click on the window
bool	g_bTrackCamera = false;         // camera panning is activated with a middle mouse click on the window
bool	g_bZoomCamera = false;          // camera zooming is activated with a right mouse click on the window
int		g_iLastMouseX;                  // last mouse x position
int		g_iLastMouseY;                  // last mouse y position
bool	g_bMoveObjectMode = false;		// true: Mouse controls the object movement, false: mouse controls the camera movement
bool	g_bMoveObject = false;			// true when left mouse button is pressed and move object mode 

// Scene data
void loadSceneData( void* pData );

std::string g_sMeshPresetArray[] = { "bunny-conformal", "sphere", "armadillo", "cart", "ducky", "pitcher", "foliage" };
std::string g_sEnvPresetArray[] = { "kitchen", "beach", "uffizi", "building" };

struct SceneDesc
{
	std::string		sMeshName;
	std::string		sEnvName;
	int				iSceneID;
	Oglf::Scene*	pScene;
	Oglf::Mesh*		pMesh;
	Oglf::CubeMap*	pSkyBoxDif;
	Oglf::CubeMap*	pSkyBoxSpec;
	Oglf::CubeMap*	pSkyBoxEnv;
	GLFWmutex		oSceneNeedUpdateLock;
	bool			bSceneNeedUpdate;
	bool			bMeshNeedUpdate;
	Oglf::Camera*	pCamera;

	SceneDesc()
		: iSceneID( 0 )
		, pScene( NULL )
		, pMesh( NULL )
		, pSkyBoxDif( NULL )
		, pSkyBoxSpec( NULL )
		, pSkyBoxEnv( NULL )
		, oSceneNeedUpdateLock( NULL )
		, bSceneNeedUpdate( false )
		, bMeshNeedUpdate( false )
		, pCamera( NULL )
	{
	}
	~SceneDesc()
	{
		destroy();
	}

	void destroy()
	{
		if( pScene )
			delete pScene;
		if( pMesh )
			delete pMesh;
		if( pSkyBoxDif )
			delete pSkyBoxDif;
		if( pSkyBoxSpec )
			delete pSkyBoxSpec;
		if( pSkyBoxEnv )
			delete pSkyBoxEnv;
		if( pCamera )
			delete pCamera;
	}
};

SceneDesc*	g_pScene1Desc;
SceneDesc*	g_pScene2Desc;
SceneDesc*	g_pScene3Desc;
SceneDesc*	g_pCurrentSceneDesc;


Oglf::Renderer* g_pRenderer;

//GI Fx
Oglf::RenderingFX*	g_pGiFx;
int					g_iGiFxID;
Oglf::Vec3			g_oCamPos;
int					g_iCamPosFxID;
int					g_iCubeDiffSamplerID;
int					g_iCubeSpecSamplerID;

GLfloat				g_fAvgLuminance;
GLfloat				g_fCurrentLum = 100.f;
const int			g_iTMtexSize = 64;

class HDRdemoRenderingConfiguration : public Oglf::RenderingConfiguration
{
public:
	HDRdemoRenderingConfiguration(Oglf::Renderer& renderer) : RenderingConfiguration(renderer)
	{

	}

	void configureRenderer()
	{
		m_renderer->setLighting( false );
		m_renderer->setClearColor(0.2f, 0.0f, 0.2f, 0.0f);
		m_renderer->setWireframe(false);
		//m_renderer->setTextureMapping2D(false);
	}
};

#endif
