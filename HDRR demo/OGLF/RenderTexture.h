#ifndef RENDERTEXTURE_H
#define RENDERTEXTURE_H

#include <GL/glew.h>
#include "Texture2D.h"

namespace Oglf
{
	/**
	 * @brief class that manages FBO for render to texture
	 */
	class RenderTexture
	{
		GLuint		m_iFramebufferID;
		GLuint		m_iDepthBufferID;
		Texture2D*	m_pTexture;
		bool		m_bDepthTest;

	public:

	    /**
		 * @brief Constructor
		 */
		RenderTexture( bool bDepthTest = true )
			: m_iFramebufferID( 0 )
			, m_iDepthBufferID( 0 )
			, m_pTexture( NULL )
			, m_bDepthTest( bDepthTest )
		{
			// setting up the Framebuffer Object
			glGenFramebuffersEXT( 1,  &m_iFramebufferID );
			glGenRenderbuffersEXT( 1, &m_iDepthBufferID );
		}

		~RenderTexture()
		{
			glDeleteFramebuffersEXT(1, &m_iFramebufferID);
			glDeleteRenderbuffersEXT(1, &m_iDepthBufferID);
		}

		/**
		* @brief get back the Texture2D object drawn.
		* @return Texture2D& return a reference to the texture2D object
		*/
		Texture2D& getTexture() const
		{
			return *m_pTexture;
		}

		/**
		 * @brief  set the texture to render to, which is set up in a Texture2D object
		 * @param  tex the texture to render to
		 */
		void setTexture( Texture2D& tex );

		/**
		* @brief  binds or unbinds the fbo
		* @param  state true: fbo is bound, false: framebuffer is bound
		*/
		void bind( bool state = true ) const
		{
			if(state)
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_iFramebufferID);
			else
				glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
		}
	};
}

#endif // RENDERTEXTURE_H
