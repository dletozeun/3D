#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glew.h>
#include "Error.h"

namespace Oglf
{
	//type definitions

	enum DataFormat
	{
		RGB16F	 = GL_RGB16F_ARB,
		RGBA16F	 = GL_RGBA16F_ARB,
		RGB32F	 = GL_RGB32F_ARB,
		RGBA32F	 = GL_RGBA32F_ARB,
		RGB8	 = GL_RGB8,
		RGBA8	 = GL_RGBA8,
		DEPTH16  = GL_DEPTH_COMPONENT16,
		DEPTH24  = GL_DEPTH_COMPONENT24,
		DEPTH32  = GL_DEPTH_COMPONENT32
	};

	enum DataType
	{
		UNSIGNED_INT    = GL_UNSIGNED_INT,
		UNSIGNED_BYTE   = GL_UNSIGNED_BYTE,
		FLOAT           = GL_FLOAT,
		HALF			= GL_HALF_FLOAT_ARB
	};

	enum PixelFormat
	{
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		DEPTH_COMPONENT = GL_DEPTH_COMPONENT
	};

	enum TexFilter
	{
		NEAREST=GL_NEAREST,
		LINEAR=GL_LINEAR,
		NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
		LINEAR_MIPMAP_NEAREST  = GL_LINEAR_MIPMAP_NEAREST,
		NEAREST_MIPMAP_LINEAR  = GL_NEAREST_MIPMAP_LINEAR,
		LINEAR_MIPMAP_LINEAR   = GL_LINEAR_MIPMAP_LINEAR
	};

	enum WrapMode
	{
		REPEAT        = GL_REPEAT,
		CLAMP         = GL_CLAMP,
		CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE
	};

	enum PicFormatname
	{
		TGA_PIC		=	0x00000001,
		JPG_PIC		=	0x00000002,
		PNG_PIC		=	0x00000004,
		EXR_PIC		=	0x00000008,
		HDR_PIC		=	0x00000010,
		DEPTH16_PIC	=	0x00000020,
		DEPTH24_PIC	=	0x00000040,
		DEPTH32_PIC =	0x00000080,

		DEVIL_SUPPORTED_PIC = TGA_PIC | JPG_PIC | PNG_PIC | EXR_PIC | HDR_PIC
	};

	/**
	* @brief Stores various picture information like data format or type and pixel format
	*/
	struct PicFormatInfo
	{
		PicFormatname eName;			// picture format name
		DataFormat eDataFormat;			// data format
		PixelFormat ePixelFormat;		// pixel format
		DataType eDataType;				// data type
		unsigned short iPixelSize;		// pixel size in bytes
	};

	extern PicFormatInfo JPG;
	extern PicFormatInfo TGA;
	extern PicFormatInfo PNG;
	extern PicFormatInfo EXR;
	extern PicFormatInfo HDR;
	extern PicFormatInfo DEPTH_16;
	extern PicFormatInfo DEPTH_24;
	extern PicFormatInfo DEPTH_32;

	/**
	* class Texture2D
	*/
	class Texture
	{
	protected:
		GLuint		width;
		GLuint		height;
		GLuint		glID;
		WrapMode	wrapMode;
		TexFilter	minFilter;
		TexFilter	magFilter;
		bool		m_bStorageDefined;

	public:

		/**
		* @brief  constructor: generates a texture for OpenGL
		* @param  width the texture width
		* @param  height the texture height
		* @param  storageDefined true: texture storage is defined, false: not defined
		*/
		Texture ( GLuint width, GLuint height )
			: width(width)
			, height(height)
			, glID( -1 )
			, wrapMode( REPEAT )
			, minFilter( NEAREST )
			, magFilter( NEAREST )
			, m_bStorageDefined(false)
		{
			glGenTextures(1, &glID);
		}

		virtual ~Texture()
		{

		}

		void setSize( GLuint w, GLuint h )
		{
			width  = w;
			height = h;
		}

		/**
		* @brief  sets the texture storage
		* @param  formatInfo the picture format info
		* @param  data a pointer to the picture data
		* @param  border border size : 0 or 1
		*/
		virtual void setData (PicFormatInfo& formatInfo, const GLvoid* data, GLuint border) = 0;

		/**
		* @brief  binds or unbinds a gl texture
		* @param  state true: texture is bound, false: texture is unbound
		*/
		virtual void bind (bool state=true) const = 0;

		/**
		* @brief  sets the minification and magnification texture filters
		* @param  type the minification filter type
		* @param  type the magnification filter type
		*/
		virtual void setFilters (TexFilter minType, TexFilter magType)
		{
			minFilter = minType;
			magFilter = magType;
		}

		/**
		* @brief  sets the texture wrap mode on all axis
		* @param  wm the wrap mode : REPEAT or CLAMP
		*/
		virtual void setWrapMode(WrapMode wm)
		{
			wrapMode = wm;
		}

		/**
		* @brief  sets the texture mipmap base level
		* @param   lod the texture mipmap base level
		*/
		virtual void setMipMapBaseLevel (int lod ) = 0;

		/**
		* @return glID the texture gl handle
		*/
		GLuint getHandle () const
		{
			return glID;
		}

		/**
		* @brief returns the texture width
		* @return texture width
		*/
		GLuint getWidth () const
		{
			return width;
		}

		/**
		* @brief returns the texture height
		* @return texture height
		*/
		GLuint getHeight () const
		{
			return height;
		}

		/**
		* @brief  loads a picture in memory with the specified format
		* @param  formatInfo the picture format info
		* @param  filename the picture filename
		* @return GLubyte* the picture data
		*/
		GLubyte* loadFile(PicFormatInfo& formatInfo, const std::string& filename);

		/**
		* @brief write a picture data on the hard disk in the specified format
		* @param format the picture format: PNG format is the only format supported at the present time
		* @param filename the picture filename
		* @param data the picture data
		*/
		static void writeFile(PicFormatInfo& formatInfo, char* filename, GLuint w, GLuint h, GLubyte* data)
		{
// 			switch (formatInfo.name)
// 			{
// 			case PNG_PIC: writePNG(filename, w, h, data);
// 				break;
// 			default:
// 				throw Error("Texture::writeFile error: invalid format: ", filename);
// 			}
		}
	};
}

#endif // TEXTURE2D_H

