#ifndef MESH_H
#define MESH_H

#include <vector>
#include <fstream>
#include <math.h>
#include "Vec.h"
#include "utils.h"
#include "Object3D.h"
#include "Namable.h"


namespace Oglf
{
	struct BoundingBox
	{
		float left, right; // planes parallel to Oyz
		float front, back; // planes parallel to Oxy
		float top, bottom; // planes parallel to Oxz
		float maxX; // max absolute x value
		float maxY; // max absolute y value
		float maxZ; // max absolute z value
		std::vector<Vec3>* vertices;

		BoundingBox(std::vector<Vec3>& vertices)
		{
			this->vertices=&vertices;
		}

		void clear()
		{
			left  = (*vertices)[0].v[0]; right  = (*vertices)[0].v[0];
			front = (*vertices)[0].v[2]; back   = (*vertices)[0].v[2];
			top   = (*vertices)[0].v[1]; bottom = (*vertices)[0].v[1];
		}

		void build()
		{
			this->clear();
			for (int i=0; i<(int)(*vertices).size(); i++)
			{
				if((*vertices)[i].v[0] <= left) {
					left=(*vertices)[i].v[0];
				}
				else if((*vertices)[i].v[0] > right) {
					right=(*vertices)[i].v[0];
				}

				if((*vertices)[i].v[2] <= back) {
					back=(*vertices)[i].v[2];
				}
				else if((*vertices)[i].v[2] > front) {
					front=(*vertices)[i].v[2];
				}

				if((*vertices)[i].v[1] <= bottom) {
					bottom=(*vertices)[i].v[1];
				}
				else if((*vertices)[i].v[1] > top) {
					top=(*vertices)[i].v[1];
				}
			}

			maxX= Oglf::maxT<float>( fabs( left ),	fabs( right ) );
			maxY= Oglf::maxT<float>( fabs( top ),	fabs( bottom ) );
			maxZ= Oglf::maxT<float>( fabs( front ),	fabs( back ) );
		}
	};

	/**
	* class Mesh
	*/
	class Mesh : public Object3D, public Namable
	{
	protected:
		std::vector<Vec3> vertices; // vertices coordinates
		std::vector<float> uv; // 2D texture coordinates
		std::vector<Vec3> vertNormals;   // vertices normals
		std::vector<Vec3> vertTangents;  // vertices tangents
		std::vector<Vec3> vertBinormals; // vertices binormals
		std::vector<Vec3> facesNormals; // face normals for flat shading
		std::vector<Vec3> facesTangents; // tangents computed at each index
		std::vector<Vec3> facesBinormals; // binormals computed at each index

		std::vector<int> vertIndices; // vertices indices to build each face (triangles)
		std::vector<int> uvIndices; // texture coordinates indices for each vertices index
		BoundingBox* bBox; // the mesh bounding box;

		GLuint m_iDlID;

	public:

		Mesh( const std::string& sName )
			: Namable( sName )
		{
			bBox= new BoundingBox(this->vertices);
		}

		virtual ~Mesh()
		{
			delete bBox;
		}

		/**
		* @brief Loads a obj file
		* @param fn the obj file name
		*/
		void importOBJ(const std::string& fn);

		/**
		* @brief scales the x,y and z vertices position components between -limit and +limit
		* @param limit x, y and z components will stuck in the range [-limit +limit]
		*/
		void boundSize(float limit);

		/**
		* @brief Prepare mesh data for rendering
		*/
		void prepareRenderableBatch();

		/**
		* @brief draws the object
		*/
		virtual void draw()
		{
			glCallList( m_iDlID );
		}

		/**
		* @brief center the pivot to the object center
		*/
		virtual void centerPivotToObjectCenter();

	private:

		/**
		* @brief create all faces tangent spaces
		*/
		void createFacesTangentSpaces();

		/**
		* @brief Computes vertices normals interpolating the face normals of each face that uses one vertex.
		* Then computes the tangent and the binormal using the normal
		*/
		void createVerticesTangentSpace();

	};
}

#endif // MESH_H
