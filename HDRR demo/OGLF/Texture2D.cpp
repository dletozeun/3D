#include "Texture.h"
#include "Texture2D.h"


namespace Oglf
{
	/**
	* @brief  loads a picture in memory with the specified format
	* @param  formatInfo the picture format info
	* @param  filename the picture filename
	* @return GLubyte* the picture data
	*/
	void Texture2D::loadFile(PicFormatInfo& formatInfo, const char* filename)
	{
		GLubyte* data=Texture::loadFile( formatInfo,filename );

		setData(formatInfo, data);

		delete[] data;
	}
}
