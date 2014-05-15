#ifndef POSTPROCESSINGFX_H
#define POSTPROCESSINGFX_H

#include "Texture2D.h"
#include "RenderTexture.h"
#include "RenderingFX.h"
#include "Namable.h"
#include <map>
#include <string>


namespace Oglf
{
	class Renderer;

	struct PassFx : public Namable
	{
		RenderingFX*	m_pRfx;
		Texture2D*		m_pOutputTex;
		std::string		m_sInputTexSamplerName;

		PassFx( std::string sName, std::string& sShaderFilename, std::string& sInputTexSamplerName, Texture2D* pInputTex, Texture2D* pOutputTex )
			: Namable( sName )
			, m_pOutputTex( pOutputTex )
			, m_sInputTexSamplerName( sInputTexSamplerName )
		{
			m_pRfx = new RenderingFX();
			m_pRfx->setShaders( "shaders/DiffuseTex2D.vert", sShaderFilename.c_str() );
			m_pRfx->addTexture( pInputTex, 0, m_sInputTexSamplerName.c_str() );
		}

		~PassFx()
		{
			delete m_pRfx;
		}

		unsigned int addParameter( const void* oParam, RFXparamType oType, const short iSize, const std::string sName )
		{
			return m_pRfx->addParameter( oParam, oType, iSize, sName.c_str() );
		}

		unsigned int addParameter( Texture* pTex, const unsigned short oTextureUnit, const std::string sName )
		{
			return m_pRfx->addTexture( pTex, oTextureUnit, sName.c_str() );
		}

		void setInputTexture( Texture2D* pTex )
		{
			m_pRfx->updateTextureLocation( 0, *pTex );
		}

		void refreshParameter( int iID )
		{
			m_pRfx->refreshParameter( iID );
		}
	};


	/**
	* class PostProcessingFX general class for the post-processing effects
	*/
	class PostProcessingFX : public Namable
	{

		std::vector< PassFx* >				m_vPasses;
		RenderTexture*						m_pOutputRT;
		Renderer*							m_pRenderer;
		bool								m_bRenderToScreen;

	public:

		PostProcessingFX( const std::string& sName, Renderer* pRenderer );

		~PostProcessingFX();

		void addPass( std::string sName, std::string sFXshaderFilename , std::string sInputTexSampler, Texture2D* pInputTex, Texture2D* pOutputTex );

		PassFx* getPass( int iPass )
		{
			return m_vPasses[ iPass ];
		}

		void compute();

		/**
		* @brief returns the texture output
		* @return the texture output
		*/
		Texture2D* getTextureOutput() const
		{
			return  &(m_pOutputRT->getTexture());
		}
	};
}

#endif // POSTPROCESSINGFX_H
