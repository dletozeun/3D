#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <IL/il.h>

#ifdef WIN32
#include <Windows.h>
#include <time.h>
#include <exception>
#endif

#include <GL/glfw.h>
#include "demoMain.h"

using namespace std;
using namespace Oglf;


/**
* @brief Callback on window resizing
* @param width the window width
* @param height the window height
*/
void GLFWCALL windowResize(int width, int height)
{
	wWidth = width;
	wHeight = height;

	try {
		g_pRenderer->initViewport(0, 0, width, height);
	}
	catch(Error e) { e.showError(); }
}

/**
* @brief Set the orthographic projection to draw 2D at pixel precision
*/
void setOrthographicProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, wWidth, wHeight, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
}

/**
* @brief Reset the perspective projection after drawing texts on the screen in orthographic projection
*/
void resetPerspectiveProjection() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

/**
* @brief Drawing callback
*/
void GLFWCALL draw()
{
	try
	{
		g_pRenderer->render();
		if( g_bDisplayHelp )
			g_pHelpHud->draw();
	}
	catch(Error e) { e.showError(); }
}

/**
* @brief Create an Opengl window and initialize the application program
*/
bool createApplication()
{
	srand(time(NULL));

	glfwInit();
	// Open window
	int iErr = glfwOpenWindow(	wWidth, wHeight,   // Width and height of window
								8, 8, 8,           // Number of red, green, and blue bits for color buffer
								8,                 // Number of bits for alpha buffer
								24,                // Number of bits for depth buffer (Z-buffer)
								0,                 // Number of bits for stencil buffer
								GLFW_WINDOW        // We want a desktop window (could be GLFW_FULLSCREEN)
							 );
	if( !iErr )
	{
		glfwTerminate();
		return false;
	}

	glfwSetWindowTitle( "HDRR demo" );

	glfwEnable( GLFW_STICKY_KEYS );
	glfwEnable( GLFW_KEY_REPEAT );

	g_pScene1Desc = new SceneDesc;
	g_pScene2Desc = new SceneDesc;
	g_pScene3Desc = new SceneDesc;

	g_pScene1Desc->oSceneNeedUpdateLock = glfwCreateMutex();
	g_pScene2Desc->oSceneNeedUpdateLock = glfwCreateMutex();
	g_pScene3Desc->oSceneNeedUpdateLock = glfwCreateMutex();

	if( g_pScene1Desc->oSceneNeedUpdateLock == NULL || 
		g_pScene2Desc->oSceneNeedUpdateLock == NULL || 
		g_pScene3Desc->oSceneNeedUpdateLock == NULL )
		return false;

	g_pCurrentSceneDesc = g_pScene1Desc;

	// Glew init for the use of OpenGL extensions
	if( glewInit() != GLEW_OK )
		return false;

	// Check if hardware is able to run the demo
	//
	const GLubyte* pExtentions = glGetString( GL_EXTENSIONS );
	string sExtentions( ( const char* )pExtentions  );
	//cout << sExtentions << endl;

	if( sExtentions.find( "GL_ARB_fragment_program" ) == string::npos || 
		sExtentions.find( "GL_ARB_vertex_program" ) == string::npos || 
		sExtentions.find( "GL_ARB_vertex_shader" ) == string::npos || 
		sExtentions.find( "GL_ARB_fragment_shader" ) == string::npos )
	{
		cout << "Error! GLSL shaders not supported on current hardware" << endl;
		return false;
	}
	if( sExtentions.find( "GL_EXT_framebuffer_object" ) == string::npos )
	{
		cout << "Error! framebuffer objects not supported on current hardware" << endl;
		return false;
	}
	if( sExtentions.find( "GL_ARB_texture_non_power_of_two" ) == string::npos &&
		sExtentions.find( "GL_EXT_texture_rectangle" ) == string::npos )
	{
		cout << "Error! NPOT textures not supported on current hardware" << endl;
		return false;
	}
	if( sExtentions.find( "GL_ARB_texture_float" ) == string::npos &&
		sExtentions.find( "GL_ATI_texture_float" ) == string::npos )
	{
		cout << "Error! Float textures not supported on current hardware" << endl;
		return false;
	}

	// DevIL init
	ilInit();
	ilEnable( IL_ORIGIN_SET );
	ilSetInteger( IL_ORIGIN_MODE, IL_ORIGIN_UPPER_LEFT );

	return true;
}

/**
* @brief Mouse buttons callback
* @param iButton the button id
* @param iState button state
*/
void GLFWCALL mouseClick(int iButton, int iState )
{
	if( g_bDisplayHelp )
		return;

	// controls the camera rotation with the left click
	if( iButton == GLFW_MOUSE_BUTTON_LEFT && iState == GLFW_PRESS )
	{
		if( g_bMoveObjectMode )
			g_bMoveObject = true;
		else
			g_bRotateCamera=true;
		
		glfwGetMousePos( &g_iLastMouseX, &g_iLastMouseY );
	}
	if( iButton == GLFW_MOUSE_BUTTON_LEFT && iState == GLFW_RELEASE )
	{
		g_bRotateCamera = false;
		g_bMoveObject = false;
	}

	// controls the camera tracking with the middle click
	if( iButton == GLFW_MOUSE_BUTTON_MIDDLE && iState == GLFW_PRESS )
	{
		g_bTrackCamera=true;
		glfwGetMousePos( &g_iLastMouseX, &g_iLastMouseY );
	}
	if( iButton == GLFW_MOUSE_BUTTON_MIDDLE && iState == GLFW_RELEASE )
	{
		g_bTrackCamera=false;
	}

	// controls the camera forward or backward movement with the right click
	if( iButton == GLFW_MOUSE_BUTTON_RIGHT && iState == GLFW_PRESS )
	{
		g_bZoomCamera=true;
		glfwGetMousePos( &g_iLastMouseX, &g_iLastMouseY );
	}
	if( iButton == GLFW_MOUSE_BUTTON_RIGHT && iState == GLFW_RELEASE )
	{
		g_bZoomCamera=false;
	}

	g_oCamPos = g_pCurrentSceneDesc->pCamera->getPosition();
	g_pGiFx->refreshParameter( g_iCamPosFxID );
}

/**
* @brief Mouse motion callback
* @param iMouseX the mouse cursor horizontal position
* @param iMouseY the mouse cursor vertical position
*/
void GLFWCALL mouseMove( int iMouseX, int iMouseY )
{
	if(g_bRotateCamera) // if camera rotation allowed, rotate camera
	{
		g_pCurrentSceneDesc->pCamera->reComputeTargetPos( g_iLastMouseX - iMouseX, g_iLastMouseY - iMouseY );

// 		cout << "pos : " << g_pCurrentSceneDesc->pCamera->getPosition() << endl;
// 		cout << "target : " << g_pCurrentSceneDesc->pCamera->getTarget() << endl;
	}

	if(g_bTrackCamera) // if camera tracking allowed, move camera
	{
		g_pCurrentSceneDesc->pCamera->moveX( g_iLastMouseX - iMouseX );
		g_pCurrentSceneDesc->pCamera->moveY( iMouseY - g_iLastMouseY );
	}

	if(g_bZoomCamera) // if camera zooming allowed, move camera
	{
		Vec3 oDistVec = g_pCurrentSceneDesc->pCamera->getTarget() - g_pCurrentSceneDesc->pCamera->getPosition();
		float fDist = oDistVec.length();
		Vec3 oViewAxis = g_pCurrentSceneDesc->pCamera->getViewAxis();
		float fDot = oDistVec | oViewAxis;
		int iDeltaPos = g_iLastMouseY-iMouseY;

		if( ( iDeltaPos > 0 && ( ( fDot < 0 && fDist < 10.f ) || fDot > 0 ) ) ||
			( iDeltaPos < 0 && ( ( fDot > 0 && fDist < 10.f ) || fDot < 0 ) ) )
		{
				g_pCurrentSceneDesc->pCamera->moveZ( iDeltaPos );
		}
	}

	if( g_bMoveObject )
	{
		Mesh * pMesh = g_pCurrentSceneDesc->pMesh;
		Matrix4x4 oMat = pMesh->getTransformer().getTransformMatrix();
		oMat.invert();
		Vec3 oVertAxis = g_pCurrentSceneDesc->pCamera->getVerticalAxis();
		oVertAxis = oMat * oVertAxis;
		pMesh->getTransformer().rotate( g_iLastMouseX - iMouseX, oVertAxis );
		Vec3 oHoriAxis = g_pCurrentSceneDesc->pCamera->getStrafeAxis();
		oHoriAxis = oMat * oHoriAxis;
		pMesh->getTransformer().rotate( g_iLastMouseY - iMouseY, oHoriAxis );
	}

	g_oCamPos = g_pCurrentSceneDesc->pCamera->getPosition();
	g_pGiFx->refreshParameter( g_iCamPosFxID );

	g_iLastMouseX = iMouseX;
	g_iLastMouseY = iMouseY;
}

/**
* @brief Program clean up
*/
void cleanup()
{
	delete g_pRenderer;
	delete g_pGiFx;

	delete g_pScene1Desc;
	delete g_pScene2Desc;
	delete g_pScene3Desc;

	delete g_pHelpHud;
}

/**
* @brief Keyboard callback
* @param iKey ID of the key pressed
* @param iState Key state
*/
void GLFWCALL keyboard( int iKey, int iState )
{
	if( iKey == GLFW_KEY_F1 && iState == GLFW_PRESS )
	{
		if( g_bDisplayHelp )
			g_bDisplayHelp = false;
		else
			g_bDisplayHelp = true;
	}
	if( iKey == GLFW_KEY_LCTRL && iState == GLFW_PRESS )
	{
		g_bMoveObjectMode = true;
	}
	if( iKey == GLFW_KEY_LCTRL && iState == GLFW_RELEASE )
	{
		g_bMoveObjectMode = false;
	}
	if( iKey == GLFW_KEY_ESC && iState == GLFW_PRESS )
	{
		if( g_bDisplayHelp )
			g_bDisplayHelp = false;
		else 
			g_bRun = false;
	}
	if( iKey == 'Z' && iState == GLFW_PRESS && g_pCurrentSceneDesc->iSceneID != 1 )
	{
		glfwLockMutex( g_pScene2Desc->oSceneNeedUpdateLock );
		g_pCurrentSceneDesc = g_pScene2Desc;
		g_pCurrentSceneDesc->bSceneNeedUpdate = true;
		g_fCurrentLum = 0.f;
		glfwUnlockMutex( g_pScene2Desc->oSceneNeedUpdateLock );
	}
	if( iKey == 'A' && iState == GLFW_PRESS && g_pCurrentSceneDesc->iSceneID != 0 )
	{
		glfwLockMutex( g_pScene1Desc->oSceneNeedUpdateLock );
		g_pCurrentSceneDesc = g_pScene1Desc;
		g_pCurrentSceneDesc->bSceneNeedUpdate = true;
		g_fCurrentLum = 0.f;
		glfwUnlockMutex( g_pScene1Desc->oSceneNeedUpdateLock );
	}
	if( iKey == 'E' && iState == GLFW_PRESS && g_pCurrentSceneDesc->iSceneID != 2 )
	{
		glfwLockMutex( g_pScene3Desc->oSceneNeedUpdateLock );
		g_pCurrentSceneDesc = g_pScene3Desc;
		g_pCurrentSceneDesc->bSceneNeedUpdate = true;
		g_fCurrentLum = 0.f;
		glfwUnlockMutex( g_pScene3Desc->oSceneNeedUpdateLock );
	}

	g_oCamPos = g_pCurrentSceneDesc->pCamera->getPosition();
	g_pGiFx->refreshParameter( g_iCamPosFxID );
}

/**
* @brief Thread used to compute the average luminance in a texture
* @param pData the texture data
*/
void GLFWCALL computeAvgColor( void* pData )
{
	GLfloat* pfData = ( GLfloat* )pData;
	int iPixelsNb = g_iTMtexSize * g_iTMtexSize;
	GLfloat r, g, b;
	r = g = b = 0.f;

	for( int i = 0; i < iPixelsNb; ++i )
	{
		r += pfData[ 4 * i ];
		g += pfData[ 4 * i + 1 ];
		b += pfData[ 4 * i + 2 ];
	}

	g_fAvgLuminance = ( r + g + b ) / 3.f;
	g_fAvgLuminance /= ( GLfloat )iPixelsNb;

	// Compute the luminance at most every 100 ms interval
	Sleep( 100 );
}

void GLFWCALL loadMesh( void* pData )
{
	SceneDesc* pDesc = ( SceneDesc* )pData;

	string sMeshPath = "./obj/" + pDesc->sMeshName + ".obj";

	// Load scene geometry
	//
	pDesc->pMesh->importOBJ( sMeshPath );
	pDesc->pMesh->centerPivotToObjectCenter();
	pDesc->pMesh->boundSize(1.0);

	glfwLockMutex( pDesc->oSceneNeedUpdateLock );
	pDesc->bSceneNeedUpdate = true;
	pDesc->bMeshNeedUpdate = true;
	glfwUnlockMutex( pDesc->oSceneNeedUpdateLock );
}

void loadSceneData( void* pData )
{
	SceneDesc* pDesc = ( SceneDesc* )pData;

	string sSkyBoxEnvPath = "./img/EXR_RLE/" + pDesc->sEnvName + "_env.exr";
	string sSkyBoxDiffPath = "./img/EXR_RLE/" + pDesc->sEnvName + "_diffuse.exr";
	string sSkyBoxSpecPath = "./img/EXR_RLE/" + pDesc->sEnvName + "_specular.exr";

	if( pDesc->pMesh != NULL )
		delete pDesc->pMesh;
	if( pDesc->pSkyBoxEnv != NULL )
		delete pDesc->pSkyBoxEnv;
	if( pDesc->pSkyBoxDif != NULL )
		delete pDesc->pSkyBoxDif;
	if( pDesc->pSkyBoxSpec != NULL )
		delete pDesc->pSkyBoxSpec;

	pDesc->pMesh = new Mesh( pDesc->sMeshName );
	glfwCreateThread( loadMesh, pDesc );

	pDesc->pScene->removeAllMeshes();
	pDesc->pScene->addRenderingFX( *g_pGiFx );
	pDesc->pScene->addMesh( *pDesc->pMesh, g_iGiFxID );


	// Load scene global lighting environment
	//
	pDesc->pSkyBoxDif = new CubeMap;
	pDesc->pSkyBoxDif->setFilters( LINEAR, LINEAR );
	pDesc->pSkyBoxDif->setWrapMode( CLAMP_TO_EDGE );
	pDesc->pSkyBoxDif->loadFile( EXR, sSkyBoxDiffPath );

	pDesc->pSkyBoxSpec = new CubeMap;
	pDesc->pSkyBoxSpec->setFilters( LINEAR, LINEAR );
	pDesc->pSkyBoxSpec->setWrapMode( CLAMP_TO_EDGE );
	pDesc->pSkyBoxSpec->loadFile( EXR, sSkyBoxSpecPath );

	pDesc->pSkyBoxEnv = new CubeMap( true );
	pDesc->pSkyBoxEnv->setFilters( LINEAR, LINEAR );
	pDesc->pSkyBoxEnv->setWrapMode( CLAMP_TO_EDGE );
	pDesc->pSkyBoxEnv->loadFile( EXR, sSkyBoxEnvPath );
}

int main(int argc, char* argv[])
{
	if( !createApplication() )
	{
		system( "pause" );
		return -1;
	}

	try
	{
		// set up the renderer
		//
		g_pRenderer = new Renderer(wWidth, wHeight);
		//pRenderer->disablePostProcessings();

		// Load Help HUD
		//
		g_pHelpHud = new Hud( "img/help.tga", 0.1f, 0.1f, 0.9f, 0.7f );

		// Prepare post-processing effects
		//
		Texture2D oHighPassTex( wWidth, wHeight );
		oHighPassTex.setFilters( LINEAR, LINEAR );
		oHighPassTex.setWrapMode( CLAMP_TO_EDGE );
		oHighPassTex.setData( EXR, NULL);

		Texture2D oBlurTex( wWidth, wHeight );
		oBlurTex.setFilters( LINEAR, LINEAR );
		oBlurTex.setWrapMode( CLAMP_TO_EDGE );
		oBlurTex.setData( EXR, NULL);

		// add HDRR post-processes
		PostProcessingFX oHDRpostProcessing( "Glow+Tonemapping", g_pRenderer );
		// High-pass filter
		oHDRpostProcessing.addPass( "HighPass", "shaders/HighPass.frag", "IN", NULL, &oHighPassTex );
		// Vertical blur for Glow effect
		oHDRpostProcessing.addPass( "Hblur", "shaders/Hblur.frag", "IN", &oHighPassTex, &oBlurTex );
		// Final pass: Horizontal blur + tone-mapping
		oHDRpostProcessing.addPass( "FinalGlow", "shaders/FinalGlow.frag", "IN", &oBlurTex, NULL );
		PassFx* pFinalGlowPass = oHDRpostProcessing.getPass( 2 );
		pFinalGlowPass->addParameter( g_pRenderer->getRenderOutputTexture(), 1, "u_oRenderTexSampler" );
		
		// Add a luminance shader parameter for exposition time effect
		int iAvgLumID = pFinalGlowPass->addParameter( &g_fCurrentLum, FR_FLOAT, 1, "u_fAvgLuminance" );


		// Prepare global lighting effect
		//
		g_pGiFx = new RenderingFX;
		g_pGiFx->setShaders("shaders/GI.vert", "shaders/GI.frag");
		g_iCubeDiffSamplerID = g_pGiFx->addTexture( NULL, 0, "u_cubeMapDiffuseSampler" );
		g_iCubeSpecSamplerID = g_pGiFx->addTexture( NULL, 1, "u_cubeMapSpecularSampler" );
		g_iCamPosFxID = g_pGiFx->addParameter( g_oCamPos.v, FR_FLOAT_VEC3, 1, "u_wsvEyePos" );


		// Create the scenes
		//
		HDRdemoRenderingConfiguration tesRConf( *g_pRenderer );

		g_pScene1Desc->sEnvName  = g_sEnvPresetArray[ 1 ];
		g_pScene1Desc->sMeshName = g_sMeshPresetArray[ 6 ];
		g_pScene1Desc->pScene	= new Scene;
		g_pScene1Desc->iSceneID  = 0;
		g_pScene1Desc->pCamera = new Camera(g_eCameraMode, 1.72221f, 0.130611f, 0.130098f , 0.f, 0.f, 0.f, wWidth, wHeight);
		g_pScene1Desc->pCamera->setPerspectiveProj(60, 0.1, 100.0);
		g_pScene1Desc->pCamera->setMoveXspeed(0.01);
		g_pScene1Desc->pCamera->setMoveYspeed(0.01);
		g_pScene1Desc->pScene->addCamera( *g_pScene1Desc->pCamera );
		g_pScene1Desc->pScene->addRenderingConfiguration( tesRConf );
		
		g_pScene2Desc->sEnvName	= g_sEnvPresetArray[ 0 ];
		g_pScene2Desc->sMeshName = g_sMeshPresetArray[ 2 ];
		g_pScene2Desc->pScene	= new Scene;
		g_pScene2Desc->iSceneID  = 1;
		g_pScene2Desc->pCamera = new Camera(g_eCameraMode, 0.767144f, 0.0578506f, 1.24567f , 0.f, 0.f, 0.f, wWidth, wHeight);
		g_pScene2Desc->pCamera->setPerspectiveProj(60, 0.1, 100.0);
		g_pScene2Desc->pCamera->setMoveXspeed(0.01);
		g_pScene2Desc->pCamera->setMoveYspeed(0.01);
		g_pScene2Desc->pScene->addCamera( *g_pScene2Desc->pCamera );		
		g_pScene2Desc->pScene->addRenderingConfiguration( tesRConf );

		g_pScene3Desc->sEnvName	= g_sEnvPresetArray[ 3 ];
		g_pScene3Desc->sMeshName = g_sMeshPresetArray[ 5 ];
		g_pScene3Desc->pScene	= new Scene;
		g_pScene3Desc->iSceneID  = 2;
		g_pScene3Desc->pCamera = new Camera(g_eCameraMode, 0.842112f, -0.426883f, -2.80972f , 0.f, 0.f, 0.f, wWidth, wHeight);
		g_pScene3Desc->pCamera->setPerspectiveProj(60, 0.1, 100.0);
		g_pScene3Desc->pCamera->setMoveXspeed(0.01);
		g_pScene3Desc->pCamera->setMoveYspeed(0.01);
		g_pScene3Desc->pScene->addCamera( *g_pScene3Desc->pCamera );		
		g_pScene3Desc->pScene->addRenderingConfiguration( tesRConf );


		// Fill renderer input
		//
		g_pRenderer->addScene( *g_pScene1Desc->pScene );
		g_pRenderer->addScene( *g_pScene2Desc->pScene );
		g_pRenderer->addScene( *g_pScene3Desc->pScene );

		loadSceneData( g_pScene1Desc );
		loadSceneData( g_pScene2Desc );
		loadSceneData( g_pScene3Desc );
		g_pCurrentSceneDesc = g_pScene1Desc;

		g_pGiFx->updateTextureLocation( g_iCubeDiffSamplerID, *g_pCurrentSceneDesc->pSkyBoxDif );
		g_pGiFx->updateTextureLocation( g_iCubeSpecSamplerID, *g_pCurrentSceneDesc->pSkyBoxSpec );


		Error::checkGLerror("main::");

		glfwSetKeyCallback( keyboard );
		glfwSetMousePosCallback( mouseMove );
		glfwSetMouseButtonCallback( mouseClick );
		glfwSetWindowSizeCallback( windowResize );
		//glfwSetWindowRefreshCallback( draw );

		glfwSwapInterval( 1 );

		Texture2DCopier oTexCopier( g_pRenderer );

		// create a texture to compute average scene luminance
		Texture2D oTMtex( g_iTMtexSize, g_iTMtexSize );
		oTMtex.setFilters( LINEAR, LINEAR );
		oTMtex.setWrapMode( CLAMP_TO_EDGE );
		oTMtex.setData( EXR, NULL);

		GLfloat* pTMdata = new GLfloat[ 4 * g_iTMtexSize * g_iTMtexSize ];

		GLFWthread oThread = -1;

		bool bWait = true;
		while( bWait )
		{
			glfwLockMutex( g_pCurrentSceneDesc->oSceneNeedUpdateLock );
			if( g_pCurrentSceneDesc->bSceneNeedUpdate )
				bWait = false;
			glfwUnlockMutex( g_pCurrentSceneDesc->oSceneNeedUpdateLock );
		}

		float fShutterSpeed = 0.1f;

		// Render loop
		do
		{
			// If the thread that compute the average scene luminance is terminated,
			// create it once again a recompute the luminance.
			if( glfwWaitThread( oThread, GLFW_NOWAIT ) && !g_bDisplayHelp )
			{
				oTexCopier.copy( g_pRenderer->getRenderOutputTexture(), &oTMtex );
				oTMtex.bind();
				glGetTexImage( GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pTMdata ); 
				oTMtex.bind( false );

				oThread = glfwCreateThread( computeAvgColor, pTMdata );
			}
			if( g_bDisplayHelp )
			{
				g_fAvgLuminance = 20.f;
			}

			g_fCurrentLum += fShutterSpeed * ( g_fAvgLuminance - g_fCurrentLum );
			pFinalGlowPass->refreshParameter( iAvgLumID );

			glfwLockMutex( g_pCurrentSceneDesc->oSceneNeedUpdateLock );
			if( g_pCurrentSceneDesc->bSceneNeedUpdate )
			{
				g_pCurrentSceneDesc->bSceneNeedUpdate = false;
				g_pRenderer->setActiveScene( g_pCurrentSceneDesc->iSceneID );
				g_pRenderer->setSkyBox( *g_pCurrentSceneDesc->pSkyBoxEnv );
				g_pGiFx->updateTextureLocation( g_iCubeDiffSamplerID, *g_pCurrentSceneDesc->pSkyBoxDif );
				g_pGiFx->updateTextureLocation( g_iCubeSpecSamplerID, *g_pCurrentSceneDesc->pSkyBoxSpec );
				
				if( g_pCurrentSceneDesc->bMeshNeedUpdate )
				{	
					g_pCurrentSceneDesc->pMesh->prepareRenderableBatch();
					g_pCurrentSceneDesc->bMeshNeedUpdate = false;
				}
			}
			glfwUnlockMutex( g_pCurrentSceneDesc->oSceneNeedUpdateLock );

			draw();

			glfwSwapBuffers();

			g_bRun = g_bRun && glfwGetWindowParam( GLFW_OPENED );
		}
		while( g_bRun );

		glfwDestroyThread( oThread );
		glfwTerminate();

		cleanup();
		delete pTMdata;
	}
	catch (Error e)
	{
		e.showError();
#ifdef WIN32
		system( "pause" );
#endif
	}
	catch( exception e )
	{
		system( "pause" );
	}

	return 0;
}
