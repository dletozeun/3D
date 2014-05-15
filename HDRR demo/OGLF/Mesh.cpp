#include "Mesh.h"
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <cstdlib>
#include "Error.h"
#include "utils.h"

using namespace std;

namespace Oglf
{
	/**
	* @brief center the pivot to the object center
	*/
	void Mesh::centerPivotToObjectCenter()
	{
		Vec3 pivot;

		for(int i=0; i<(int)vertices.size(); i++)
		{
			pivot = pivot + vertices[i];
		}
		pivot= pivot/(float)vertices.size();

		for(int i=0; i<(int)vertices.size(); i++)
		{
			vertices[i] -= pivot;
		}
	}

	/**
	* @brief scales the x,y and z vertices position components between -limit and +limit
	* @param limit x, y and z components will stuck in the range [-limit +limit]
	*/
	void Mesh::boundSize(float limit)
	{
		float ScaleFactor= limit / maxT(maxT(bBox->maxX, bBox->maxY), bBox->maxZ);

		for(int i=0; i<(int)vertices.size(); i++)
		{
			vertices[i] *= ScaleFactor;
		}

		bBox->build();
	}

	/**
	* @brief create all faces tangent spaces
	*/
	void Mesh::createFacesTangentSpaces()
	{
		Vec3 v0, v1, v2; // a 3D vector that stores each face (triangles) vertices
		Vec3 edge1, edge2; // two edges of a face
		float dS1, dS2; // variation along edge 1 and 2  of the S texture coordinates component
		float dT1, dT2; // variation along edge 1 and 2  of the T texture coordinates component
		float detInv;
		Vec3  normal, tangent, binormal; // to store each face normal, tangent and binormal
		int progress; // computation progress (percent)

		// run through each face and find its tangent space
		//
		for(int i=0; i<(int)vertIndices.size(); i+=3)
		{
			// show progress in console
			if (i%100==0)
			{
				progress=(int)(100.0*(float)i/(float)vertices.size());
				cout<<"Computing tangent spaces..."<<progress<<"% \r"<<flush;
			}

			// storing each face vertices
			v0 = vertices[vertIndices[i]];
			v1 = vertices[vertIndices[i+1]];
			v2 = vertices[vertIndices[i+2]];

			// calculate 2 edges in order to compute their cross product which is the face normal
			edge1= v1-v0;
			edge2= v2-v0;
			normal= edge1^edge2; // cross product gives face normal
			normal.normalize();

			// storing all normals in an array
			facesNormals.push_back(normal);

			// computing tangent spaces at each face
			dS1= uv[2*uvIndices[i+1]] - uv[2*uvIndices[i]];
			dS2= uv[2*uvIndices[i+2]] - uv[2*uvIndices[i]];

			dT1= uv[2*uvIndices[i+1]+1] - uv[2*uvIndices[i]+1];
			dT2= uv[2*uvIndices[i+2]+1] - uv[2*uvIndices[i]+1];

			detInv= 1.f/(dS1 * dT2 - dS2 * dT1);

			tangent= (dT2 * edge1 - dT1 * edge2);
			binormal= (dS1 * edge2 - dS2 * edge1);

			tangent.normalize();
			binormal.normalize();

			facesTangents.push_back(tangent);
			facesBinormals.push_back(binormal);
		}

		cout<<"Computing tangent spaces...100%"<<endl;
	}

	/**
	* @brief Computes vertices normals interpolating the face normals of each face that uses one vertex.
	* Then computes the tangent and the binormal using the normal
	*/
	void Mesh::createVerticesTangentSpace()
	{
		// an array of vector that stores the faces indices that use a vertex for each vertex
		vector<int>* facesCreated= new vector<int>[vertices.size()];
		Vec3 normal, tangent, binormal;
		int progress; // computation progress (percent)

		for (int i=0; i<(int)vertIndices.size(); i++)
		{
			facesCreated[vertIndices[i]].push_back(i/3);
		}

		for (int i=0; i<(int)vertices.size(); i++)
		{
			if (i%100==0)
			{
				progress=(int)(100.0*(float)i/(float)vertices.size());
				cout<<"Computing vertices tangents spaces..."<<progress<<"% \r"<<flush;
			}

			normal = tangent = binormal = Vec3();

			for (int j=0; j<(int)facesCreated[i].size(); j++)
			{
				normal   = normal   + facesNormals[facesCreated[i][j]];

				tangent  = tangent  + facesTangents[facesCreated[i][j]];

				binormal = binormal + facesBinormals[facesCreated[i][j]];
			}
			normal.normalize();
			tangent.normalize();
			binormal.normalize();

			// if this occurs there are problems in the object mesh
			// 		if(isnan(tangent.length()) || isnan(binormal.length()))
			// 		{
			tangent  = Vec3();
			binormal = Vec3();
			/*		}*/

			vertNormals.push_back(normal);
			vertTangents.push_back(tangent);
			vertBinormals.push_back(binormal);
		}

		cout<<"Computing vertices tangents spaces...100%"<<endl;
	}

	//find slash slash == //
	bool findSS(char*s, int length)
	{
		char* c=s;
		int i=0;

		while (i<length)
		{
			//cout<<*(c+i)<<"-";
			if (*(c+i)=='/')
			{
				return true;
			}
			i++;
		}
		return false;
	}

	/**
	* @brief Loads a obj file
	* @param fn the obj file name
	*/
	void Mesh::importOBJ( const string& sFilename )
	{
		ifstream oObjFile( sFilename.c_str(), ios::in );
		if ( !oObjFile )
		{
			throw Error( "Mesh::importOBJ error : Failed to open the obj file", sFilename );
		}
		cout << "Importing OBJ file: " << sFilename << endl;

		string sCom;
		float fVal;

		while ( !oObjFile.eof() )
		{
			oObjFile >> sCom;
			//cout<<com;

			if ( !oObjFile.eof() && sCom !="v" && sCom != "vn" && sCom != "vt" && sCom != "f"
				&& sCom != "g" && sCom != "s" && sCom != "#" && sCom != "usemtl"
				&& sCom != "mtllib" )
			{
				//cout << "com: " << sCom << endl;
				throw Error( "Mesh::importOBJ error : Bad format", sFilename );
			}

			if ( sCom == "v" )
			{
				//cout<<"v"<<endl;
				Vec3 oVec;
				oObjFile >> oVec.v[0]; oObjFile >> oVec.v[1]; oObjFile >> oVec.v[2];

				vertices.push_back( oVec );
			}
			if (sCom=="vt")
			{
				//cout<<"vt"<<endl;
				oObjFile>>fVal;
				uv.push_back(fVal);
				oObjFile>>fVal;
				uv.push_back(fVal);

				getline(oObjFile, sCom);
				sCom.clear();
			}
			if (sCom=="f")
			{
				//cout<<"f"<<endl;
				string s;

				for (int i=0; i<3; i++)
				{
					oObjFile>>s;

					if (!s.empty())
					{
						char* index=(char*)s.c_str();
						char* word;

						//cout<<"chaine "<<index<<endl;
						word = strtok(index, "/");
						vertIndices.push_back(atoi(word)-1);

						word = strtok(NULL, "/");

						if (!findSS(index, s.size()))
						{
							//cout<<"coord de tex"<<endl;
							uvIndices.push_back(atoi(word)-1);

							word = strtok(NULL, "/");
						}
					}
				}
			}

			if (sCom=="vn")
			{
				//cout<<"vn"<<endl;
				getline(oObjFile, sCom);
				sCom.clear();
			}
			if (sCom=="g")
			{
				//cout<<"g"<<endl;
				getline(oObjFile, sCom);
				sCom.clear();
			}
			if (sCom=="s")
			{
				//cout<<"s"<<endl;
				getline(oObjFile, sCom);
				sCom.clear();
			}
			if (sCom=="#")
			{
				//cout<<"#"<<endl;
				getline(oObjFile, sCom);
				sCom.clear();
			}
			if (sCom=="usemtl" || sCom=="mtllib")
			{

				//cout<<"usemtl"<<endl;
				getline(oObjFile, sCom);
				sCom.clear();
			}
		}

		this->bBox->build();
		this->createFacesTangentSpaces();
		this->createVerticesTangentSpace();

		oObjFile.close();
	}

	void Mesh::prepareRenderableBatch()
	{
		m_iDlID = glGenLists( 1 );
		glNewList( m_iDlID, GL_COMPILE );

		glEnable( GL_NORMALIZE );

		glBegin( GL_TRIANGLES );
		
		for (unsigned int i = 0; i < vertIndices.size(); i++)
		{
			glTexCoord2f(uv[2 * uvIndices[i]], uv[2 * uvIndices[i] + 1]);

			// smooth shading
			glNormal3fv(vertNormals[vertIndices[i]].v);
			//flat shading
			//glNormal3fv(facesNormals[i/3].v);

			glVertex3fv(vertices[vertIndices[i]].v);
		}
		glEnd();

		glEnd();

		glEndList();
	}

}