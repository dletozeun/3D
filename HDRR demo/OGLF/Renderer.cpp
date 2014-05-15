#include <math.h>
#include "Renderer.h"
#include "CubeMap.h"
#include "PostProcessingFX.h"
#include "RenderTexture.h"

using namespace std;

namespace Oglf
{
	/**
	*/
	void Renderer::render ( )
	{
		//static float a=0.01;
		//a+=0.0001;

		try {
			// drawing the activated scene
			if(m_iActiveScene>=0)
			{
				// drawing from the camera point of view
				m_vScenes[m_iActiveScene]->getCamera().setActive();

				if( m_bEnablePostProcessings )
				{
					glViewport( 0, 0, m_iRenderTargetTexWidth, m_iRenderTargetTexHeight );
					m_pRenderOutputRTex->bind();
				}
				else
					m_pRenderOutputRTex->bind( false );

				// light rotation

				// clear frame buffer and depth buffer
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				m_vScenes[m_iActiveScene]->drawElements();
				//Error::checkGLerror("Renderer::render");

// 				glDisable( GL_LIGHTING );
// 				glDisable( GL_TEXTURE_2D );
// 				glBegin(GL_LINES);
// 
// 				glColor3f(1.0, 0.0, 0.0);
// 				glVertex3f(0.0, 0.0, 0.0);
// 				glVertex3f(10.0, 0.0, 0.0);
// 
// 				glColor3f(0.0, 1.0, 0.0);
// 				glVertex3f(0.0, 0.0, 0.0);
// 				glVertex3f(0.0, 10.0, 0.0);
// 
// 				glColor3f(0.0, 0.0, 1.0);
// 				glVertex3f(0.0, 0.0, 0.0);
// 				glVertex3f(0.0, 0.0, 10.0);
// 
// 				glEnd();

				if( m_pSkyBox != NULL )
					m_pSkyBox->drawSkyBox();

				if( m_bEnablePostProcessings )
				{
					m_pRenderOutputRTex->bind( false );
					vector< PostProcessingFX* >::iterator oPPit;

					for( oPPit = m_vPostProcessingFXs.begin(); oPPit != m_vPostProcessingFXs.end(); ++oPPit )
					{
						(*oPPit)->compute();
					}
				}
			}
			else
			{
				throw Error("Renderer::Render Error: No active scene in this renderer", NULL, true);
			}
		}
		catch(Error e) { e.showError(); }

	}
}
