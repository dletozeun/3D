#include "RenderTexture.h"


namespace Oglf
{
	/**
	* @brief  set the texture to render to, which is set up in a Texture2D object
	* @param  tex the texture to render to
	*/
	void RenderTexture::setTexture( Texture2D& oTex )
	{
		m_pTexture = &oTex;

		// setting up a texture to render to
		this->bind();
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, oTex.getHandle(), 0);

		if( m_bDepthTest )
		{
			// setting up the depth buffer
			glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, m_iDepthBufferID );
			glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, oTex.getWidth(), oTex.getHeight() );
			glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_iDepthBufferID );
		}

		if( glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT ) != GL_FRAMEBUFFER_COMPLETE_EXT )
		{
			throw Error( "RenderTexture::setTexture Error: the fbo is not complete" );
		}
		this->bind( false );
	}
}


