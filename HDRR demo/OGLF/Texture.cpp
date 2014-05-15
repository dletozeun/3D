#include <iostream>
#include <IL/il.h>
#include "Texture.h"

// #include <ImfRgbaFile.h>
// #include <ImfArray.h>
// #include <ImfOutputFile.h>
// #include <ImfInputFile.h>
// #include <ImfChannelList.h>
// #include <ImfStringAttribute.h>
// #include <ImfMatrixAttribute.h>


using namespace std;

namespace Oglf
{
	PicFormatInfo JPG =
	{
		JPG_PIC,
		RGB8,
		RGB,
		UNSIGNED_BYTE,
		3
	};

	PicFormatInfo TGA =
	{
		TGA_PIC,
		RGBA8,
		RGBA,
		UNSIGNED_BYTE,
		4
	};

	PicFormatInfo PNG =
	{
		PNG_PIC,
		RGB8,
		RGB,
		UNSIGNED_BYTE,
		3
	};

	PicFormatInfo EXR =
	{
		EXR_PIC,
		RGBA16F,
		RGBA,
		FLOAT, // or HALF if it read with the OpenEXR lib
		16
	};

	PicFormatInfo HDR =
	{
		HDR_PIC,
		RGB32F,
		RGB,
		UNSIGNED_BYTE,
		12
	};

	PicFormatInfo DEPTH_16 =
	{
		DEPTH16_PIC,
		DEPTH16,
		DEPTH_COMPONENT,
		UNSIGNED_BYTE,
		2
	};

	PicFormatInfo DEPTH_24 =
	{
		DEPTH24_PIC,
		DEPTH24,
		DEPTH_COMPONENT,
		UNSIGNED_BYTE,
		3
	};

	PicFormatInfo DEPTH_32 =
	{
		DEPTH32_PIC,
		DEPTH32,
		DEPTH_COMPONENT,
		UNSIGNED_BYTE,
		3
	};


	/**
	* @brief loads a 48 bits openEXR picture in memory
	* @param filename file path
	* @param width address of a Texture.width
	* @param height address of a Texture.height
	* @return Timage the picture data
	*/
// 	GLubyte* loadEXR48( const char* filename, GLuint& width, GLuint& height )
// 	{
// 		GLubyte*Timage=NULL;
// 		Imf::Array2D<Imf::Rgba> pixels;
// 
// 		const int bulkSize=8;
// 		//GLuint r, g, b, a;
// 
// 		Imf::RgbaInputFile file (filename);
// 		Imath::Box2i dw = file.dataWindow();
// 		width = dw.max.x - dw.min.x + 1;
// 		height = dw.max.y - dw.min.y + 1;
// 		pixels.resizeErase ((long int)height, (long int)width);
// 		file.setFrameBuffer ( &pixels[0][0] - dw.min.x - dw.min.y * width, 1, width );
// 		file.readPixels (dw.min.y, dw.max.y);
// 
// 		int size=width * height;
// 
// 		Timage=new GLubyte[size*bulkSize];
// 
// 		for(int i=0;i<(int)height;i++)
// 		{
// 			for(int j=0;j<(int)width;j++)
// 			{
// 				memcpy(Timage+bulkSize*(i*width+j), &pixels[i][j], bulkSize*sizeof(char));
// 
// 				float r = pixels[i][j].r;
// 				float g = pixels[i][j].g;
// 				float b = pixels[i][j].b;
// 				float a = pixels[i][j].a;
// 
// 				//if( r > 1.f  || g > 1.f || b > 1.f )
// 					cout<< "R = " << r << "  G = " << g << "  B = " << b << "  A = " << b << endl;
// 			}
// 		}
// 
// 		return Timage;
// 	}

	GLubyte* loadFileDevIL( const char* filename, PicFormatInfo& formatInfo, GLuint& width, GLuint& height )
	{
		GLubyte* pData;
		ILuint iHandle;
		ilGenImages( 1, &iHandle );
		ilBindImage(iHandle);

		if( ilLoadImage( filename ) == IL_TRUE )
		{
			width = ilGetInteger( IL_IMAGE_WIDTH );
			height = ilGetInteger( IL_IMAGE_HEIGHT );
			int iBpp = ilGetInteger( IL_IMAGE_BPP );

			int iDataSize = width * height * formatInfo.iPixelSize;
			pData = new GLubyte[ iDataSize ];

			if( formatInfo.eName == EXR_PIC )
			{
// 				int iSize = width * height * 4;
// 				GLfloat* pfData = new GLfloat[ iSize ];
				ilCopyPixels( 0, 0, 0, width, height, 1, IL_RGBA, IL_FLOAT, pData );
				
// 				GLfloat* pfData = ( GLfloat* )pData;
// 
// 				int size = width * height * 4;
// 				for( int i = 0; i < size; ++i )
// 				{
// 					 float val = pfData[ i ];
// 
// 					if( val > 1.f )
// 						cout << "Val = " << val << endl;
// 				}
				
				//delete[] pfData;
			}
			else
			{
				if( formatInfo.ePixelFormat == RGBA )
					ilCopyPixels( 0, 0, 0, width, height, 1, IL_RGBA, IL_UNSIGNED_BYTE, pData );
				else
					ilCopyPixels( 0, 0, 0, width, height, 1, IL_RGB, IL_UNSIGNED_BYTE, pData );
			}
		}
		else
		{
			throw Error("Texture::loadFile error: Failed to load file", filename);
		}
	
		ilDeleteImages( 1, &iHandle ); 

		if( ilGetError() != IL_NO_ERROR )
			throw Error("Texture::loadFile error: DevIL returned an error opening file", filename);

		return pData;;
	}

	/**
	* @brief  loads a picture in memory with the specified format
	* @param  format the picture format: TGA, JPG, EXR48...
	* @param  filename the picture filename
	* @return GLubyte* the picture data
	*/
	GLubyte* Texture::loadFile( PicFormatInfo& formatInfo, const string& filename )
	{
		GLubyte* pData = NULL;

		if( formatInfo.eName & DEVIL_SUPPORTED_PIC )
			pData = loadFileDevIL( filename.c_str(), formatInfo, width, height );
// 		else if( formatInfo.eName & EXR48_PIC )
// 			pData = loadEXR48( filename, width, height );
		else
			throw Error( "Texture::loadFile error: picture format is not supported for reading" );

		return pData;
	}
}
