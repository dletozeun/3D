#ifndef HUD_H
#define HUD_H

#include "Texture2D.h"


namespace Oglf
{
	class Hud
	{
		Texture2D* m_pHudTex;
		RenderingFX* m_pRfx;
		float m_fX;
		float m_fY;
		float m_fWidth;
		float m_fHeight;

	public:

		Hud( const std::string& sTex, float fX, float fY, float fWidth, float fHeight )
			: m_fX( fX )
			, m_fY( fY )
			, m_fWidth( fWidth )
			, m_fHeight( fHeight )
		{
			m_pHudTex = new Texture2D;
			m_pHudTex->setFilters( LINEAR, LINEAR );
			m_pHudTex->setWrapMode( CLAMP_TO_EDGE );
			m_pHudTex->loadFile( TGA, sTex.c_str() );

			m_pRfx = new RenderingFX;
			m_pRfx->setShaders( "shaders/DiffuseTex2D.vert", "shaders/DiffuseTex2D.frag" );
			m_pRfx->addTexture( m_pHudTex, 0, "u_texSampler");
		}

		~Hud()
		{
			delete m_pHudTex;
		}

		void draw()
		{
			glMatrixMode(GL_PROJECTION);
			glPushMatrix();
			glLoadIdentity();
			glOrtho(0, 1, 1, 0, -1, 1);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			bool bDepthTest = false;
			if( glIsEnabled( GL_DEPTH_TEST ) == GL_TRUE )
			{
				glDisable( GL_DEPTH_TEST );
				bDepthTest = true;
			}

			glEnable( GL_BLEND );
			glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

			m_pRfx->enable();

			glBegin(GL_QUADS);

			glTexCoord2f( 0.0, 0.0 );
			glVertex2f( m_fX, m_fY );

			glTexCoord2f( 0.0, 1.0 );
			glVertex2f( m_fX, m_fHeight );

			glTexCoord2f( 1.0, 1.0 );
			glVertex2f( m_fWidth, m_fHeight );

			glTexCoord2f( 1.0, 0.0 );
			glVertex2f( m_fWidth, m_fY );

			glEnd();

			m_pRfx->disable();

			glDisable( GL_BLEND );

			if( bDepthTest )
				glEnable( GL_DEPTH_TEST );

			glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
			glPopMatrix();
		}
	};
}

#endif