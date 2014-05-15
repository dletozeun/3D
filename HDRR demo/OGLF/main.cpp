#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <IL/il.h>

#ifdef WIN32
#include <Windows.h>
#include <time.h>
#endif

#include <GL/glfw.h>
#include "main.h"

using namespace std;
using namespace Oglf;

/**
 * @brief Renders a text on the screen using glut functions.
 * @param x the horizontal text position in pixels from the upper left corner
 * @param y the vertical text position in pixels from the upper left corner
 * @param spacing the spacing between each text character
 * @param font the text font
 * @param the text to be displayed
 */
// void renderSpacedBitmapString(int x, int y,int spacing, void *font,char *string) {
//   char *c;
//   for (c=string; *c != '\0'; c++) {
// 	glRasterPos2f(x,y);
//     glutBitmapCharacter(font, *c);
// 	x = x + glutBitmapWidth(font,*c) + spacing;
//   }
// }

/**
 * @brief This functions is called each time the window size changes and refrest the opengl viewport
 * @param width the window width
 * @param height the window height
 */
void GLFWCALL windowResize(int width, int height)
{
	wWidth = width;
	wHeight = height;

	try {
		r->initViewport(0, 0, width, height);
	}
	catch(Error e) { e.showError(); }
}

/**
 * @brief Sets the orthographic projection to draw 2D objects. Here it is used to display texts on the screen.
 * Note that origin is the upper left corner and coordinates unit is the pixel.
 */
void setOrthographicProjection()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	// set a 2D orthographic projection
	glOrtho(0, wWidth, wHeight, 0, -1, 1);
	// invert the y axis, down is positive
	//glScalef(1, -1, 1);
	// mover the origin from the bottom left corner
	// to the upper left corner
	//glTranslatef(0, -h, 0);
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
 * @brief draws the scene
 */
void draw()
{
	try {
		r->render();
	}
	catch(Error e) { e.showError(); }

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH);
	glPushMatrix();
	glLoadIdentity();
	setOrthographicProjection();

	glPopMatrix();
	resetPerspectiveProjection();
	glEnable(GL_DEPTH);
	glEnable(GL_LIGHTING);

	glfwSwapBuffers();

	frame++;
}

/**
 * @brief Handles opengl initialization
 * @TODO should be moved to the Renderer class
 */
void init()
{
	glewInit();
	ilInit();
	ilEnable(IL_ORIGIN_SET);
}

/**
 * @brief Called when there is nothing to do
 */
void idle()
{
// 	static int time = 0;
// 	static int timebase = 0;
// 
// 	time=glutGet(GLUT_ELAPSED_TIME);
// 
// 	if (time - timebase > 1000)
// 	{
// 		float fps =(frame*1000.0/(time-timebase));
// 		timebase = time;
// 		frame = 0;
// 
// 		sprintf(fpsString, "%4.2f FPS", fps);
// 	}
// 
// 	int col = rand()%255;
// 	color[0] = col/255.0;
// 	col = rand()%255;
// 	color[1] = col/255.0;
// 	col = rand()%255;
// 	color[2] = col/255.0;
// 
// 	simpleFX->refreshParameter(colorParamID);
// 
// 	Sleep(10);
// 	glutPostRedisplay();
}

/**
 * @brief Handles mouse buttons clicks
 * @param button the button id
 * @param x the mouse cursor horizontal position
 * @param y the mouse cursor vertical position
 */
void GLFWCALL mouseClick(int iButton, int iState )
{
// controls the camera rotation with the left click
	if( iButton == GLFW_MOUSE_BUTTON_LEFT && iState == GLFW_PRESS )
	{
		rotateCamera=true;
		glfwGetMousePos( &g_iLastMouseX, &g_iLastMouseY );

		cout << "** mouse click **" << endl;
		cout << "lx = " << g_iLastMouseX << "    " << "ly = " << g_iLastMouseY << endl << endl;
	}
	if( iButton == GLFW_MOUSE_BUTTON_LEFT && iState == GLFW_RELEASE )
	{
		rotateCamera=false;
	}

// controls the camera tracking with the middle click
	if( iButton == GLFW_MOUSE_BUTTON_MIDDLE && iState == GLFW_PRESS )
	{
		trackCamera=true;
		glfwGetMousePos( &g_iLastMouseX, &g_iLastMouseY );
	}
	if( iButton == GLFW_MOUSE_BUTTON_MIDDLE && iState == GLFW_RELEASE )
	{
		trackCamera=false;
	}

// controls the camera forward or backward movement with the right click
	if( iButton == GLFW_MOUSE_BUTTON_RIGHT && iState == GLFW_PRESS )
	{
		zoomCamera=true;
		glfwGetMousePos( &g_iLastMouseX, &g_iLastMouseY );
	}
	if( iButton == GLFW_MOUSE_BUTTON_RIGHT && iState == GLFW_RELEASE )
	{
		zoomCamera=false;
	}
}

/**
 * @brief called each time the mouse moves
 * @param x the mouse cursor horizontal position
 * @param y the mouse cursor vertical position
 */
void GLFWCALL mouseMove( int iMouseX, int iMouseY )
{
	if(rotateCamera) // if camera rotation allowed, rotate camera
	{
		cam->reComputeTargetPos( g_iLastMouseX - iMouseX, g_iLastMouseY - iMouseY );

		cout << "** mouse move **" << endl;
		cout << "dx = " << g_iLastMouseX - iMouseX << "    " << "dy = " << g_iLastMouseY - iMouseY << endl;
		cout << "x = " << iMouseX << "    " << "y = " << iMouseY << endl;
		cout << "lx = " << g_iLastMouseX << "    " << "ly = " << g_iLastMouseY << endl << endl;

		g_iLastMouseX = iMouseX;
		g_iLastMouseY = iMouseY;
	}

	if(trackCamera) // if camera tracking allowed, move camera
	{
		cam->moveX( g_iLastMouseX - iMouseX );
		cam->moveY( iMouseY - g_iLastMouseY );
		g_iLastMouseX = iMouseX;
		g_iLastMouseY = iMouseY;

		if(cameraMode == TARGET_CAMERA)
		{
			targetPos = cam->getTarget();
			//moveSubWindow->sync_live();
		}
	}

	if(zoomCamera) // if camera zooming allowed, move camera
	{
		cam->moveZ( g_iLastMouseY-iMouseY );
		g_iLastMouseX = iMouseX;
		g_iLastMouseY = iMouseY;
	}
}

void cleanup()
{
	delete r;
	delete cam;
}

void GLFWCALL keyboard( int iKey, int iState )
{
	if( iKey == GLFW_KEY_ESC && iState == GLFW_PRESS )
	{
		g_bRun = false;
	}
}

int main(int argc, char* argv[])
{
	srand(time(NULL));

	try
	{
		glfwInit();
		// Open window
		int iErr = glfwOpenWindow( 800, 600,          // Width and height of window
								   8, 8, 8,           // Number of red, green, and blue bits for color buffer
								   8,                 // Number of bits for alpha buffer
								   24,                // Number of bits for depth buffer (Z-buffer)
								   0,                 // Number of bits for stencil buffer
								   GLFW_WINDOW        // We want a desktop window (could be GLFW_FULLSCREEN)
								 );
		if( !iErr )
		{
			glfwTerminate();
			return -1;
		}

		glfwSetWindowTitle( "Test" );
		
		glfwEnable( GLFW_STICKY_KEYS );
		glfwEnable( GLFW_KEY_REPEAT );

		init();

		// set up the renderer
		//
		r = new Renderer(wWidth, wHeight);

		// create a new camera with perspective projection
		//
		cam = new Camera(cameraMode, cameraPos, targetPos, wWidth, wHeight);
		cam->setPerspectiveProj(45, 0.1, 100.0);
		cam->setMoveXspeed(0.01);
		cam->setMoveYspeed(0.01);

		// adding an omnidirectional light in the scene
		OmniLight omni1(0.0, 2.0, 1.0, true);
		omni1.setRenderable(true);
		omni1.transformable(true);
		omni1.setDiffuseColor(1.0, 1.0, 1.0, 1.0);
		omni1.setSpecularColor(1.0, 1.0, 1.0, 1.0);
		omni1.setAmbientColor(0.2, 0.2, 0.2, 0.0);
		omni1.bind(0);

		ObjMesh oMesh;
		oMesh.importOBJ("obj/armadillo.obj");
		oMesh.centerPivotToObjectCenter();
		oMesh.boundSize(1.0);

		Texture2D checker;
		checker.setFilters(LINEAR_MIPMAP_LINEAR, LINEAR);
		checker.setWrapMode(REPEAT);
		checker.loadFile(JPG, "img/checker.jpg");

		simpleFX = new RenderingFX;
		simpleFX->setShaders("shaders/simple.vert", "shaders/simple.frag");

		colorParamID = simpleFX->addParameter(color, FR_FLOAT_VEC4, 1, "u_color");
		simpleFX->refreshParameter(colorParamID);

		simpleFX->addTexture(checker, 0, "u_tex");


		Scene testScene;
		int simpleFXid = testScene.addRenderingFX(*simpleFX);
		testScene.addCamera(*cam);
		testScene.addMesh(oMesh, simpleFXid );
		testScene.addLight(omni1);

		TestRenderingConfiguration tesRConf(*r);
		testScene.addRenderingConfiguration(tesRConf);

		r->addScene(testScene);

		Error::checkGLerror("main::");

		glfwSetKeyCallback( keyboard );
		glfwSetMousePosCallback( mouseMove );
		glfwSetMouseButtonCallback( mouseClick );
		glfwSetWindowSizeCallback( windowResize );

		do
		{
			draw();

			g_bRun = !glfwGetKey( GLFW_KEY_ESC ) && glfwGetWindowParam( GLFW_OPENED );
		}
		while( g_bRun );

		glfwTerminate();

		cleanup();
	}
	catch (Error e) { e.showError(); }

	return 0;
}
