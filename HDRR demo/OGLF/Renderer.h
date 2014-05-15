#ifndef RENDERER_H
#define RENDERER_H


#include <vector>
#include "Scene.h"
#include "Error.h"
#include "Texture.h"
#include "Core.h"
#include "CubeMap.h"
#include "RenderTexture.h"
#include "PostProcessingFX.h"
#include "RenderingFX.h"


namespace Oglf
{
	//type definitions

	enum HintTarget
	{
		FOG_HINT                     = GL_FOG_HINT,
		LINE_SMOOTH_HINT             = GL_LINE_SMOOTH_HINT,
		PERSPECTIVE_CORRECTION_HINT  = GL_PERSPECTIVE_CORRECTION_HINT,
		POINT_SMOOTH_HINT            = GL_POINT_SMOOTH_HINT,
		POLYGON_SMOOTH_HINT          = GL_POLYGON_SMOOTH_HINT
	};

	enum HintMode
	{
		FASTEST   = GL_FASTEST,
		NICEST    = GL_NICEST,
		DONT_CARE = GL_DONT_CARE
	};


	class Renderer : public Core
	{
		std::vector< Scene* >					m_vScenes;
		int									m_iActiveScene;
		GLuint								m_iScreenWidth;
		GLuint								m_iScreenHeight;
		CubeMap*							m_pSkyBox;
		std::vector< PostProcessingFX* >	m_vPostProcessingFXs;
		RenderTexture*						m_pRenderOutputRTex;
		Texture2D*							m_pRendererOutputTex;
		bool								m_bEnablePostProcessings;
		int									m_iRenderTargetTexWidth;
		int									m_iRenderTargetTexHeight;

	public:


		/**
		* @brief constructor initialize the renderer
		*/
		Renderer ( GLuint iScreenWidth, GLuint iScreenHeight )
			: m_iActiveScene( -1 )
			, m_iScreenWidth( iScreenWidth )
			, m_iScreenHeight( iScreenHeight )
			, m_pSkyBox( NULL )
			, m_pRenderOutputRTex( NULL )
			, m_pRendererOutputTex( NULL )
			, m_bEnablePostProcessings( true )
			, m_iRenderTargetTexWidth( 0 )
			, m_iRenderTargetTexHeight( 0 )
		{
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_CULL_FACE);
			glDisable(GL_LIGHTING);
			glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
			glClearColor(0.1, 0.2, 0.0, 1.0);

			glPixelStorei(GL_PACK_ALIGNMENT, 1);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			
			m_iRenderTargetTexWidth = m_iScreenWidth;
			m_iRenderTargetTexHeight = m_iScreenHeight;

			m_pRendererOutputTex = new Texture2D( m_iRenderTargetTexWidth, m_iRenderTargetTexHeight );
			m_pRendererOutputTex->setFilters( LINEAR, LINEAR );
			m_pRendererOutputTex->setWrapMode( CLAMP_TO_EDGE );
			m_pRendererOutputTex->setData( EXR, NULL );

			m_pRenderOutputRTex = new RenderTexture( true );
			m_pRenderOutputRTex->setTexture( *m_pRendererOutputTex );
		}

		/**
		* @brief destructor deletes allocated ressouces
		*/
		~Renderer()
		{
			delete m_pRenderOutputRTex;
			delete m_pRendererOutputTex;
		}

		Texture2D* getRenderOutputTexture()
		{
			return m_pRendererOutputTex;
		}

		void enablePostProcessings()
		{
			m_bEnablePostProcessings = true;
		}

		void disablePostProcessings()
		{
			m_bEnablePostProcessings = false;
		}

		/**
		* @brief renders the scene
		*/
		void render();

		/**
		* @brief Enable/Disable depth test
		* @param state true: enable, false: disable
		*/
		void setDepthTest(bool state)
		{
			if(state) {
				glEnable(GL_DEPTH_TEST);
			}
			else {
				glDisable(GL_DEPTH_TEST);
			}
		}

		/**
		* @brief Enable/Disable culling
		* @param state true: enable, false: disable
		*/
		void setCulling(bool state)
		{
			if(state) {
				glEnable(GL_CULL_FACE);
			}
			else {
				glDisable(GL_CULL_FACE);
			}
		}

		/**
		* @brief Enable/Disable lighting
		* @param state true: enable, false: disable
		*/
		void setLighting(bool state)
		{
			if(state) {
				glEnable(GL_LIGHTING);
			}
			else {
				glDisable(GL_LIGHTING);
			}
		}

		/**
		* @brief sets the OpenGL clear color for the framebuffer
		* @param r red component value
		* @param g green component value
		* @param b blue component value
		* @param a alpha component value
		*/
		void setClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
		{
			glClearColor(r, g, b, a);
		}

		/**
		* @brief Enable/Disable 2D texture mapping
		* @param state true: enable, false: disable
		*/
		void setTextureMapping2D(bool state)
		{
			if(state) {
				//glActiveTexture(GL_TEXTURE0);
				glEnable(GL_TEXTURE_2D);
			}
			else {
				//glActiveTexture(GL_TEXTURE0);
				glDisable(GL_TEXTURE_2D);
			}
		}

		/**
		* @brief active or deactive the wireframe mode
		* @param state true: wireframe mode is activated, false: it is not
		*/
		void setWireframe(bool state)
		{
			if(state) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
		}

		/**
		* @brief  add a scene to the renderer
		* @param  sc the Scene object to add
		*/
		void addScene (Scene& sc)
		{
			m_vScenes.push_back(&sc);
			m_iActiveScene++;
			m_vScenes[ m_iActiveScene ]->getCamera().initViewport(0, 0, m_iScreenWidth, m_iScreenHeight);
			m_vScenes[ m_iActiveScene ]->getCamera().setActive();
			m_vScenes[ m_iActiveScene ]->configureRenderer();
		}

		/**
		* @brief actives the scene which ID is given
		* @param sceneID the scene to activate ID
		*/
		void setActiveScene(int sceneID)
		{
			if(sceneID>=0 && sceneID<(int)m_vScenes.size())
			{
				m_iActiveScene=sceneID;
				m_vScenes[m_iActiveScene]->getCamera().initViewport(0, 0, m_iScreenWidth, m_iScreenHeight);
				m_vScenes[m_iActiveScene]->configureRenderer();
			}
			else
			{
				throw Error("Renderer::setActiveScene error: Can not active a scene with an invalid ID");
			}
		}

		void initViewport(GLuint x, GLuint y, GLuint w, GLuint h)
		{
			m_iScreenWidth=w;
			m_iScreenHeight=h;

			if(m_iActiveScene!=-1)
			{
				m_vScenes[m_iActiveScene]->getCamera().initViewport(x, y, w, h);
				m_vScenes[m_iActiveScene]->getCamera().setActive();
			}
			else
			{
				throw Error("Renderer::Error: No active scene in the renderer");
			}
		}

		/**
		* @brief  write a screenshot on the hard disk in the specified format
		* @param  format the picture format: PNG format is the only format supported at the present time
		* @param  filename the picture filename
		*/
		void screenshot(char* filename)
		{
			GLubyte* data = new GLubyte[3* m_iScreenWidth * m_iScreenHeight];

			glFinish();
			glReadPixels(m_vScenes[m_iActiveScene]->getCamera().getViewportX(),
				m_vScenes[m_iActiveScene]->getCamera().getViewportY(),
				m_vScenes[m_iActiveScene]->getCamera().getViewportWidth(),
				m_vScenes[m_iActiveScene]->getCamera().getViewportHeight(),
				GL_RGB, GL_UNSIGNED_BYTE, data);

			Error::checkGLerror("Renderer::screenshot");

			std::cout<<"screenshot "<<m_vScenes[m_iActiveScene]->getCamera().getViewportX()<<" "
				<<m_vScenes[m_iActiveScene]->getCamera().getViewportY()<<" "
				<<m_vScenes[m_iActiveScene]->getCamera().getViewportWidth()<<" "
				<<m_vScenes[m_iActiveScene]->getCamera().getViewportHeight()<<std::endl
				<<"screen: "<<m_iScreenWidth<<" "<<m_iScreenHeight<<std::endl;


			Texture::writeFile(PNG, filename, m_iScreenWidth, m_iScreenHeight, data);

			delete[] data;
		}

		void setSkyBox( CubeMap& oSkyBox )
		{
			m_pSkyBox = &oSkyBox;
		}

		int getRenderTargetWidth() const
		{
			return m_iScreenWidth;
		}

		int getRenderTargetHeight() const
		{
			return m_iScreenHeight;
		}

		void addPostProcessingFX( PostProcessingFX& oPP )
		{
			m_vPostProcessingFXs.push_back( &oPP );

			RenderingFX* pRfx = oPP.getPass( 0 )->m_pRfx;
			pRfx->updateTextureLocation( 0, *m_pRendererOutputTex );
		}
	};
}

#endif // RENDERER_H
