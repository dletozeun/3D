#include <string>
#include "PostProcessingFX.h"
#include "Renderer.h"

using namespace std;


namespace Oglf
{
	PostProcessingFX::PostProcessingFX( const string& sName, Renderer* pRenderer )
		: Namable( sName )
		, m_pOutputRT( NULL )
		, m_pRenderer( pRenderer )
		, m_bRenderToScreen( false )
	{
			if( m_pRenderer == NULL )
				throw Error( "PostProcessingFX::PostProcessingFX error : Invalid renderer!" ); 

			m_pOutputRT = new RenderTexture();
	}

	PostProcessingFX::~PostProcessingFX()
	{
		delete m_pOutputRT;

		vector< PassFx* >::iterator oPassIt = m_vPasses.begin();
		for( oPassIt; oPassIt != m_vPasses.end(); ++oPassIt )
			delete *oPassIt;
	}

	void PostProcessingFX::addPass( string sName, std::string sFXshaderFilename , string sInputTexSamplerName, Texture2D* pInputTex, Texture2D* pOutputTex )
	{
		if( m_bRenderToScreen )
			throw Error( "PostProcessingFX::addPass error : you can't add an additional pass!" );
		PassFx* pPass = new PassFx( sName, sFXshaderFilename, sInputTexSamplerName, pInputTex, pOutputTex );
		m_vPasses.push_back( pPass );

		if( pOutputTex == NULL )
		{
			m_bRenderToScreen = true;
			m_pRenderer->addPostProcessingFX( *this );
		}
	}

	void PostProcessingFX::compute()
	{
		vector< PassFx* >::iterator oPassIt = m_vPasses.begin();

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

		RenderingFX* pRfx = NULL;
		Texture2D* pOutputTex = NULL;

		for( oPassIt; oPassIt != m_vPasses.end(); ++oPassIt )
		{
			pRfx	   = ( *oPassIt )->m_pRfx;
			pOutputTex = ( *oPassIt )->m_pOutputTex;

			if( pOutputTex != NULL )
			{
				m_pOutputRT->setTexture( *pOutputTex );
				m_pOutputRT->bind();
				glViewport(0, 0, pOutputTex->getWidth(), pOutputTex->getHeight());
			}
			else
			{
				m_pOutputRT->bind( false );
				glViewport(0, 0, m_pRenderer->getRenderTargetWidth(), m_pRenderer->getRenderTargetHeight() );
			}

			pRfx->enable();

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

			if( pOutputTex!= NULL && pOutputTex->isMipmapped() )
			{
				pOutputTex->bind();
				glGenerateMipmapEXT( GL_TEXTURE_2D );
				pOutputTex->bind( false );
			}
		}

		pRfx->disable();

		if( bDepthTest )
			glEnable( GL_DEPTH_TEST );

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glPopAttrib();
	}
}
