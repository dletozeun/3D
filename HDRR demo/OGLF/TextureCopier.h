#ifndef TEXTURE_COPIER_H
#define TEXTURE_COPIER_H

#include "RenderTexture.h"
#include "RenderingFX.h"

namespace Oglf
{
	class Texture2DCopier
	{
		RenderTexture*	m_pRTex;
		RenderingFX*	m_pRfx;
		Renderer*		m_pRenderer;

	public:

		Texture2DCopier( Renderer* pRenderer )
			: m_pRenderer( pRenderer )
		{
			m_pRTex = new RenderTexture( false );
			m_pRfx = new RenderingFX;
			m_pRfx->setShaders( "shaders/DiffuseTex2D.vert", "shaders/DiffuseTex2D.frag" );
			m_pRfx->addTexture( NULL, 0, "u_texSampler");
		}

		~Texture2DCopier()
		{
			delete m_pRTex;
			delete m_pRfx;
		}

		void copy( Texture2D* pSrcTex, Texture2D* pDstTex )
		{
			if( pSrcTex == NULL )
			{
				throw Error( "Texture2DCopier::copy error : Invalid texture input!" );
			}

			// Set the projection matrix to 2D projection
			glMatrixMode( GL_PROJECTION );
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0 );

			glMatrixMode( GL_MODELVIEW );
			glPushMatrix();
			glLoadIdentity();

			glPushAttrib( GL_VIEWPORT_BIT );

			bool bDepthTest = false;
			if( glIsEnabled( GL_DEPTH_TEST ) == GL_TRUE )
			{
				glDisable( GL_DEPTH_TEST );
				bDepthTest = true;
			}

			if( pDstTex != NULL )
			{
				m_pRTex->setTexture( *pDstTex );
				m_pRTex->bind();
				glViewport(0, 0, pDstTex->getWidth(), pDstTex->getHeight());
			}
			else
				glViewport(0, 0, m_pRenderer->getRenderTargetWidth(), m_pRenderer->getRenderTargetHeight() );
			
			m_pRfx->updateTextureLocation( 0, *pSrcTex );
			m_pRfx->enable();

			glBegin(GL_QUADS);

			glTexCoord2f(0.0, 0.0);
			glVertex2f(0.0, 0.0);

			glTexCoord2f(1.0, 0.0);
			glVertex2f(1.0, 0.0);

			glTexCoord2f(1.0, 1.0);
			glVertex2f(1.0, 1.0);

			glTexCoord2f(0.0, 1.0);
			glVertex2f(0.0, 1.0);

			glEnd();

			m_pRfx->disable();
			m_pRTex->bind( false );

			if( pDstTex != NULL )
			{
				m_pRTex->bind( false );
				if( pDstTex->isMipmapped() )
				{
					pDstTex->bind();
					glGenerateMipmapEXT( GL_TEXTURE_2D );
					pDstTex->bind( false );
				}
			}

			if( bDepthTest )
				glEnable( GL_DEPTH_TEST );

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();

			glPopAttrib();
		}

	};
}

#endif