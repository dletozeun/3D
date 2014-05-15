#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "Texture.h"
#include "Texture2D.h"
#include "RenderingFX.h"

namespace Oglf
{
	/**
	* @brief  extract a piece of data from from a picture data
	* @param  i the vertical coordinate of the copy beginning in the picture data (pixels)
	* @param  j the horizontal coordinate of the copy beginning in the picture data (pixels)
	* @param  sizei the piece height (pixels)
	* @param  sizej the piece width (pixels)
	* @param  w the picture data area width (pixels)
	* @param  h the picture data area height (pixels)
	* @param  bulkSize the size of each copied elements (bytes)
	* @param  data the picture data address
	* @param  rowMajOrder true: the data is read in row major order, false: the data is read in column major order
	* @param  direction true: data is from left to right or top to bottom, false: data is read from right to left or from bottom to top
	* @return  bulk the extracted piece of data
	*/
	GLubyte* extract(int i, int j, int sizei, int sizej, int w, int h, int bulkSize, GLubyte* data, bool rowMajorOrder, bool direction);

	/**
	* class Cubemap manages cube mapping with openGL
	*/
	class CubeMap : public Texture
	{
		bool				m_bAutoGenerateMipmap;
		bool				m_bCreateSkyBox;
		Texture2D			m_pSkyBoxTexArray[6];
		static RenderingFX*	s_oDiffuseTex2dFx;
		int					m_iSkyBoxTexParamID;

	public:

		/**
		* @param  tex
		*/
		CubeMap ( bool bCreateSkyBox = false, GLuint size = 0 )
			: Texture(size, size)
			, m_bCreateSkyBox( bCreateSkyBox )
			, m_bAutoGenerateMipmap( false )
			, m_iSkyBoxTexParamID( 0 )
		{
			setWrapMode( CLAMP_TO_EDGE );
			setFilters( NEAREST, NEAREST );

			for( int i = 0; i < 6; ++ i )
			{
				m_pSkyBoxTexArray[ i ].setWrapMode( wrapMode );
				m_pSkyBoxTexArray[ i ].setFilters( minFilter, magFilter );
			}

			if( s_oDiffuseTex2dFx == NULL )
			{
				s_oDiffuseTex2dFx = new RenderingFX;
				s_oDiffuseTex2dFx->setShaders( "shaders/DiffuseTex2D.vert", "shaders/DiffuseTex2D.frag" );
				m_iSkyBoxTexParamID = s_oDiffuseTex2dFx->addTexture( &m_pSkyBoxTexArray[0], 0, "u_texSampler" );
			}
		}

		~CubeMap()
		{
			if( s_oDiffuseTex2dFx != NULL )
			{
				delete s_oDiffuseTex2dFx;
				s_oDiffuseTex2dFx = NULL;
			}
		}


		/**
		* @brief  loads a picture in memory with the specified format
		* @param  formatInfo the picture format info
		* @param  filename the picture filename
		* @return GLubyte* the picture data 
		*/
		void loadFile(PicFormatInfo& formatInfo, const std::string& sFilename );

		/**
		* @brief  set the texture storage
		* @param  formatInfo the picture format info
		* @param  data a pointer to the picture data
		* @param  border border size : 0 or 1
		*/
		void setData (PicFormatInfo& formatInfo, const GLvoid* data, GLuint border = 0);

		/**
		* @brief  binds or unbinds a gl texture
		* @param  state true: texture is bound, false: texture is unbound
		*/
		void bind (bool state=true) const
		{
			if(state)
			{
				glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, glID);
			}
			else
			{
				glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, 0);
			}
		}

		/**
		* @brief  sets the minification and magnification texture filters
		* @param  type the minification filter type
		* @param  type the magnification filter type
		*/
		void setFilters (TexFilter minType, TexFilter magType)
		{
			this->bind();
			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, minType);
			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, magType);

			if( minType == LINEAR_MIPMAP_LINEAR  || minType == NEAREST_MIPMAP_LINEAR ||
				minType == LINEAR_MIPMAP_NEAREST || minType == NEAREST_MIPMAP_NEAREST )
				m_bAutoGenerateMipmap = true;
		}

		/**
		* @brief  sets the texture wrap mode on all axis
		* @param  wm the wrap mode
		*/
		virtual void setWrapMode(WrapMode wm)
		{
			this->bind();
			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, wm);
			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, wm);
		}

		/**
		* @brief  generates mipmap for this texture
		*/
		void generateMipMap()
		{
			if(m_bStorageDefined)
			{
				this->bind();
				glGenerateMipmapEXT(GL_TEXTURE_CUBE_MAP_ARB);
			}
			else
			{
				throw Error("CubeMap::Warning: tempting to generate mipmap whereas the texture storage is not defined");
			}
		}

		/**
		* @brief  sets the texture mipmap base level
		* @param   lod the texture mipmap base level
		*/
		void setMipMapBaseLevel (int lod )
		{
			this->bind();
			glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_BASE_LEVEL, lod);
		}

		/**
		* @brief  Draw the cube map as a sky box
		*/
		void drawSkyBox();
	};
}

#endif // CUBEMAP_H
