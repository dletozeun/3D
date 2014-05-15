#ifndef TEXTURE2D_H
#define TEXTURE2D_H

#include <GL/glew.h>
#include "Texture.h"
#include "Error.h"

namespace Oglf
{
	/**
	* class Texture2D
	*/
	class Texture2D : public Texture
	{
		bool          m_bAutoGenerateMipmap;

	public:

		/**
		* @brief  constructor: generates a texture for OpenGL
		* @param  width the texture width
		* @param  height the texture height
		* @param  storageDefined true: texture storage is defined, false: not defined
		*/
		Texture2D (GLuint width=0, GLuint height=0 )
			: Texture(width, height)
			, m_bAutoGenerateMipmap( false )
		{
			setWrapMode( REPEAT );
			setFilters( NEAREST, NEAREST );
		}

		/**
		* @brief  set the texture storage
		* @param  formatInfo the picture format info
		* @param  data a pointer to the picture data
		* @param  border border size : 0 or 1
		*/
		void setData (PicFormatInfo& formatInfo, const GLvoid* data = NULL, GLuint border = 0)
		{
			if( width == 0 || height == 0 )
			{
				throw Error("Texture2D::storage error: invalid texture dimensions");
				return;
			}

			this->bind();
			if( m_bAutoGenerateMipmap )
			{
#ifdef LINUX_ATI
				glTexImage2D(GL_TEXTURE_2D, 0, formatInfo.eDataFormat, width, height, border, formatInfo.ePixelFormat, formatInfo.eDataType, NULL);
#else
				glTexImage2D(GL_TEXTURE_2D, 0, formatInfo.eDataFormat, width, height, border, formatInfo.ePixelFormat, formatInfo.eDataType, data);
#endif
				glGenerateMipmapEXT( GL_TEXTURE_2D );

#ifdef LINUX_ATI
				glTexImage2D(GL_TEXTURE_2D, 0, formatInfo.eDataFormat, width, height, border, formatInfo.ePixelFormat, formatInfo.eDataType, data);
#endif
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, formatInfo.eDataFormat, width, height, border, formatInfo.ePixelFormat, formatInfo.eDataType, data);
			}

			
			
			Error::checkGLerror("Texture2D::storage");
			m_bStorageDefined=true;
		}

		/**
		* @brief  loads a picture in memory with the specified format
		* @param  formatInfo the picture format info
		* @param  filename the picture filename
		* @return GLubyte* the picture data
		*/
		void loadFile(PicFormatInfo& formatInfo, const char* filename);

		/**
		* @brief  binds or unbinds a gl texture
		* @param  state true: texture is bound, false: texture is unbound
		*/
		void bind (bool state=true) const
		{
			if( state )
			{
				glBindTexture(GL_TEXTURE_2D, glID);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		/**
		* @brief  sets the minification and magnification texture filters
		* @param  type the minification filter type
		* @param  type the magnification filter type
		*/
		void setFilters (TexFilter minType, TexFilter magType)
		{
			Texture::setFilters( minType, magType );
			this->bind();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minType);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magType);

			if( minType == LINEAR_MIPMAP_LINEAR  || minType == NEAREST_MIPMAP_LINEAR ||
				minType == LINEAR_MIPMAP_NEAREST || minType == NEAREST_MIPMAP_NEAREST )
			{
				glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); 

				m_bAutoGenerateMipmap = true;
				if( m_bStorageDefined )
					glGenerateMipmapEXT( GL_TEXTURE_2D );
			}
		}

		/**
		* @brief  sets the texture wrap mode on all axis
		* @param  wm the wrap mode : REPEAT or CLAMP
		*/
		virtual void setWrapMode(WrapMode wm)
		{
			Texture::setWrapMode( wm );
			this->bind();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wm);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wm);
		}

		/**
		* @brief  sets the texture mipmap base level
		* @param   lod the texture mipmap base level
		*/
		void setMipMapBaseLevel (int lod )
		{
			this->bind();
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, lod);
		}

		bool isMipmapped()
		{
			return m_bAutoGenerateMipmap;
		}
	};
}

#endif // TEXTURE2D_H
