#include <cstdlib>
#include "CubeMap.h"
#include "Texture.h"
#include "Error.h"

using namespace std;

namespace Oglf
{
	RenderingFX* CubeMap::s_oDiffuseTex2dFx = NULL;

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
	GLubyte* extract(int i, int j, int sizei, int sizej, int w, int h, int bulkSize, GLubyte* data, bool rowMajorOrder, bool iPositive, bool jPositive)
	{	
		if(i+sizei>h || j+sizej>w)
		{
			//throw Error("extract() Error: bad size", NULL, true);
		}

		int endi, stepi, endj, stepj;

		GLubyte* bulk=new GLubyte[sizei*sizej*bulkSize];

		if(iPositive && jPositive)
		{
			endi=i+sizei;
			stepi=1;

			endj=j+sizej;
			stepj=1;
		}
		else if(iPositive && !jPositive)
		{
			endi=i+sizei;
			stepi=1;

			endj=j-sizej;
			stepj=-1;
		}
		else if(!iPositive && jPositive)
		{
			endi=i-sizei;
			stepi=-1;

			endj=j+sizej;
			stepj=1;
		}
		else
		{
			endi=i-sizei;
			stepi=-1;

			endj=j-sizej;
			stepj=-1;
		}

		if(rowMajorOrder)
		{
			for(int k=i; k!=endi; k+=stepi)
			{
				for(int m=j; m!=endj; m+=stepj)
				{
					unsigned int iBulkOffset = ( ((k-i)*stepi)*sizej + (m-j)*stepj ) * bulkSize;
					unsigned int iDataOffset = ( k*w + m ) * bulkSize;

					memcpy(bulk + iBulkOffset, data + iDataOffset, bulkSize);

					//std::cout<<"bulk="<<( (k-i)*sizej + m-j ) * bulkSize<<" data="<<( k*w + m ) * bulkSize<<std::endl;
					//std::cout<<"  k-i"<<k-i<<std::endl;
				}
			}
		}
		else
		{
			for(int k=j; k!=endj; k+=stepj)
			{
				for(int m=i; m!=endi; m+=stepi)
				{
					memcpy(bulk + ( ((k-j)*stepj)*sizei + (m-i)*stepi ) * bulkSize, data + ( m*w + k ) * bulkSize, bulkSize);

					//std::cout<<"bulk="<<( (k-j)*sizei + m-i ) * bulkSize<<" data="<<( m*w + k ) * bulkSize<<std::endl;
					//std::cout<<"  k-j"<<k-j<<std::endl;
				}
			}
		}

		return bulk;
	}

	/**
	* @brief  set the texture storage
	* @param  formatInfo the picture format info
	* @param  data a pointer to the picture data (picture data must be a vertical cross)
	* @param  border border size : 0 or 1
	*/
	void CubeMap::setData (PicFormatInfo& formatInfo, const GLvoid* data, GLuint border)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, glID);
		GLubyte* pData = NULL;

		try
		{
			pData = extract(height-1, 2*width-1, height, width, 3*width, 4*height, formatInfo.iPixelSize, (GLubyte*)data, true, false, false);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB, 0, formatInfo.eDataFormat, width, height, border, formatInfo.ePixelFormat, formatInfo.eDataType, pData );
			
			if( m_bCreateSkyBox )
			{
				m_pSkyBoxTexArray[ 1 ].setSize( width, height );
				m_pSkyBoxTexArray[ 1 ].setData( formatInfo, pData );
			}

			pData = extract(2*height-1, width-1, height, width, 3*width, 4*height, formatInfo.iPixelSize, (GLubyte*)data, true, false, false);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB, 0, formatInfo.eDataFormat, width, height, border, formatInfo.ePixelFormat, formatInfo.eDataType, pData );

			if( m_bCreateSkyBox )
			{
				m_pSkyBoxTexArray[ 2 ].setSize( width, height );
				m_pSkyBoxTexArray[ 2 ].setData( formatInfo, pData );
			}

			pData = extract(2*height-1, 2*width-1, height, width, 3*width, 4*height, formatInfo.iPixelSize, (GLubyte*)data, true, false, false);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB, 0, formatInfo.eDataFormat, width, height, border, formatInfo.ePixelFormat, formatInfo.eDataType, pData );

			if( m_bCreateSkyBox )
			{
				m_pSkyBoxTexArray[ 4 ].setSize( width, height );
				m_pSkyBoxTexArray[ 4 ].setData( formatInfo, pData );
			}

			pData = extract(2*height-1, 3*width-1, height, width, 3*width, 4*height, formatInfo.iPixelSize, (GLubyte*)data, true, false, false);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB, 0, formatInfo.eDataFormat, width, height, border, formatInfo.ePixelFormat, formatInfo.eDataType, pData );

			if( m_bCreateSkyBox )
			{
				m_pSkyBoxTexArray[ 3 ].setSize( width, height );
				m_pSkyBoxTexArray[ 3 ].setData( formatInfo, pData );
			}

			pData = extract(3*height-1, 2*width-1, height, width, 3*width, 4*height, formatInfo.iPixelSize, (GLubyte*)data, true, false, false);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB, 0, formatInfo.eDataFormat, width, height, border, formatInfo.ePixelFormat, formatInfo.eDataType, pData);

			if( m_bCreateSkyBox )
			{
				m_pSkyBoxTexArray[ 0 ].setSize( width, height );
				m_pSkyBoxTexArray[ 0 ].setData( formatInfo, pData );
			}

			pData = extract(3*height, width, height, width, 3*width, 4*height, formatInfo.iPixelSize, (GLubyte*)data, true, true, true);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB, 0, formatInfo.eDataFormat, width, height, border, formatInfo.ePixelFormat, formatInfo.eDataType, pData);

			if( m_bCreateSkyBox )
			{
				m_pSkyBoxTexArray[ 5 ].setSize( width, height );
				m_pSkyBoxTexArray[ 5 ].setData( formatInfo, pData );
			}
		}
		catch(Error e) { e.showError(); exit(1); }

		if( m_bAutoGenerateMipmap )
			glGenerateMipmapEXT( GL_TEXTURE_CUBE_MAP_ARB );

		Error::checkGLerror("CubeMap::setData");
		m_bStorageDefined=true;
	}

	/**
	* @brief  loads a picture in memory with the specified format
	* @param  formatInfo the picture format info
	* @param  filename the picture filename
	* @return GLubyte* the picture data 
	*/
	void CubeMap::loadFile(PicFormatInfo& formatInfo, const string& sFilename )
	{
		GLubyte* data=Texture::loadFile(formatInfo, sFilename);

		width=width/3;
		height=width;


		setData(formatInfo, data);

		delete[] data;
	}

	void CubeMap::drawSkyBox()
	{
		GLfloat fSize = 20.f;

		s_oDiffuseTex2dFx->updateTextureLocation( m_iSkyBoxTexParamID, m_pSkyBoxTexArray[ 2 ] );
		s_oDiffuseTex2dFx->enable();

		glBegin(GL_QUADS);

		glTexCoord2f(0.0, 0.0); glVertex3f(-fSize, -fSize, -fSize);
		glTexCoord2f(0.0, 1.0); glVertex3f(-fSize, fSize, -fSize);
		glTexCoord2f(1.0, 1.0); glVertex3f(-fSize, fSize, fSize);
		glTexCoord2f(1.0, 0.0); glVertex3f(-fSize, -fSize, fSize);		

		glEnd();

		s_oDiffuseTex2dFx->updateTextureLocation( m_iSkyBoxTexParamID, m_pSkyBoxTexArray[ 3 ] );
		s_oDiffuseTex2dFx->enable();

		glBegin(GL_QUADS);

		glTexCoord2f(1.0, 0.0); glVertex3f(fSize, -fSize, -fSize);
		glTexCoord2f(0.0, 0.0); glVertex3f(fSize, -fSize, fSize);
		glTexCoord2f(0.0, 1.0); glVertex3f(fSize, fSize, fSize);
		glTexCoord2f(1.0, 1.0); glVertex3f(fSize, fSize, -fSize);

		glEnd();

		s_oDiffuseTex2dFx->updateTextureLocation( m_iSkyBoxTexParamID, m_pSkyBoxTexArray[ 0 ] );
		s_oDiffuseTex2dFx->enable();

		glBegin(GL_QUADS);

		glTexCoord2f(1.0, 1.0); glVertex3f(-fSize, -fSize, -fSize);
		glTexCoord2f(1.0, 0.0); glVertex3f(-fSize, -fSize, fSize);
		glTexCoord2f(0.0, 0.0); glVertex3f(fSize, -fSize, fSize);
		glTexCoord2f(0.0, 1.0); glVertex3f(fSize, -fSize, -fSize);

		glEnd();

		s_oDiffuseTex2dFx->updateTextureLocation( m_iSkyBoxTexParamID, m_pSkyBoxTexArray[ 1 ] );
		s_oDiffuseTex2dFx->enable();

		glBegin(GL_QUADS);

		glTexCoord2f(1.0, 0.0); glVertex3f(-fSize, fSize, -fSize);
		glTexCoord2f(0.0, 0.0); glVertex3f(fSize, fSize, -fSize);
		glTexCoord2f(0.0, 1.0); glVertex3f(fSize, fSize, fSize);
		glTexCoord2f(1.0, 1.0); glVertex3f(-fSize, fSize, fSize);

		glEnd();

		s_oDiffuseTex2dFx->updateTextureLocation( m_iSkyBoxTexParamID, m_pSkyBoxTexArray[ 4 ] );
		s_oDiffuseTex2dFx->enable();

		glBegin(GL_QUADS);

		glTexCoord2f(1.0, 0.0); glVertex3f(-fSize, -fSize, -fSize);
		glTexCoord2f(0.0, 0.0); glVertex3f(fSize, -fSize, -fSize);
		glTexCoord2f(0.0, 1.0); glVertex3f(fSize, fSize, -fSize);
		glTexCoord2f(1.0, 1.0); glVertex3f(-fSize, fSize, -fSize);

		glEnd();

		s_oDiffuseTex2dFx->updateTextureLocation( m_iSkyBoxTexParamID, m_pSkyBoxTexArray[ 5 ] );
		s_oDiffuseTex2dFx->enable();

		glBegin(GL_QUADS);

		glTexCoord2f(0.0, 0.0); glVertex3f(-fSize, -fSize, fSize);
		glTexCoord2f(0.0, 1.0); glVertex3f(-fSize, fSize, fSize);
		glTexCoord2f(1.0, 1.0); glVertex3f(fSize, fSize, fSize);
		glTexCoord2f(1.0, 0.0); glVertex3f(fSize, -fSize, fSize);

		glEnd();
	}
}
