#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "RenderingFX.h"
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"
#include "RenderingConfiguration.h"


namespace Oglf
{
	/**
	* @brief Contains the meshes list attached to a rendering effect.
	*/
	struct RenderingFXmeshAttachment
	{
		RenderingFX* rFX;
		std::vector<Mesh*> attachedMeshes;

		RenderingFXmeshAttachment(RenderingFX& FX) : rFX(&FX)
		{

		}
	};

	/**
	* @brief Manage a scene for real-time rendering with OpenGL.
	*/
	class Scene
	{
		std::vector<Camera*> m_cameras;                                         // constains all the cameras in the scene
		std::vector<Light*> m_lights;                                           // contains all the lights in the scene
		std::vector<RenderingFXmeshAttachment> m_renderingFXmeshAttachmentList; // list of rendering effects associated to a list of meshes
		std::vector<Mesh*> m_defaultShadingMeshList;                            // list of all meshes rendered using the fixed pipeline
		RenderingConfiguration* m_rConf;                                        // a pointer to a rendering configuration object

		int curActiveCam; // current active camera in the scene

		std::vector<Camera*>::iterator cIt;
		std::vector<Light*>::iterator lIt;
		std::vector<Mesh*>::iterator mIt;
		std::vector<RenderingFXmeshAttachment>::iterator rIt;

		int m_iMatID;
		Matrix4x4* m_pWorldMat;

	public:
		Scene()
			: m_rConf(NULL)
			, curActiveCam(-1)
			, m_iMatID( -1 )
			, m_pWorldMat( NULL )
		{
		}

		/**
		* @brief Draws the scene content.
		*/
		void drawElements ();

		/**
		* @brief Returns the camera by ID.
		* @param id the camera ID.
		*/
		inline Camera& getCamera(int id);

		/**
		* @brief Returns the current active camera.
		* @return the current active camera
		*/
		inline Camera& getCamera();

		/**
		* @brief Returns the light by ID.
		* @param id the light ID
		*/
		inline Light& getLight(int id);

		/**
		* @brief Configures the renderer using the rendering configuration object.
		*/
		void configureRenderer();

		/**
		* @brief Adds a rendering effect to the scene and creates a attachment list to this rendering effect.
		* @param rFX a reference to a rendering effect
		*/
		inline int addRenderingFX(RenderingFX& rFX);

		/**
		* @brief Adds a mesh in the scene.
		* @param  m a mesh
		* @param rFXid the rendering effect id to attach to.
		*/
		void addMesh ( Mesh& m, int rFXid = -1 );

		bool removeMesh ( const std::string& sName );

		void removeAllMeshes();

		/**
		* @brief Adds a mesh in the scene.
		* @param  m a mesh
		*/
		inline void addLight(Light& l);

		/**
		* @brief Adds a primitive in the scene and sets it active.
		* @param  c a camera
		*/
		inline void addCamera (Camera& c );

		/**
		* @brief Adds a rendering configuration to the scene.
		* @param rConf a rendering configuration
		*/
		inline void addRenderingConfiguration(RenderingConfiguration& rConf);

		/**
		* @brief Sets the current active camera.
		* @param the camera ID in the scene camera list
		*/
		inline void setActiveCamera(int id);
	};


	/**
	* @brief Return the camera by ID.
	* @param id the camera ID.
	*/
	inline Camera& Scene::getCamera(int id)
	{
		if (id > -2 && id < (int) m_cameras.size() && curActiveCam != -1)
		{
			return *m_cameras[id];
		}
		else
		{
			throw Error("Scene::getCamera error: trying to select a camera with an invalid ID");
		}
	}

	/**
	* @brief Returns the current active camera.
	* @return the current active camera
	*/
	inline Camera& Scene::getCamera()
	{
		return *m_cameras[curActiveCam];
	}

	/**
	* @brief Return the light by ID.
	* @param id the light ID
	*/
	inline Light& Scene::getLight(int id)
	{
		if (id>-1 && id<(int)m_lights.size())
		{
			return *m_lights[id];
		}
		else
		{
			throw Error("Scene::getLight error: trying to select a light with an invalid ID");
		}
	}

	/**
	* @brief Adds a rendering effect to the scene and creates a attachment list to this rendering effect.
	* @param rFX a reference to a rendering effect
	*/
	inline int Scene::addRenderingFX(RenderingFX& rFX)
	{
		m_renderingFXmeshAttachmentList.push_back(RenderingFXmeshAttachment(rFX));

		m_iMatID = rFX.addParameter( m_pWorldMat, FR_FLOAT_MAT4, 1, "u_mWorldMatrix" );

		return m_renderingFXmeshAttachmentList.size()-1;
	}

	/**
	* @brief Adds a mesh in the scene.
	* @param  m a mesh
	*/
	inline void Scene::addLight(Light& l)
	{
		m_lights.push_back(&l);
	}

	/**
	* @brief Adds a primitive in the scene and sets it active.
	* @param  c a camera
	*/
	inline void Scene::addCamera (Camera& c )
	{
		m_cameras.push_back(&c);
		curActiveCam=m_cameras.size()-1;
	}

	/**
	* @brief Adds a rendering configuration to the scene.
	* @param rConf a rendering configuration
	*/
	inline void Scene::addRenderingConfiguration(RenderingConfiguration& rConf)
	{
		m_rConf=&rConf;
	}

	/**
	* @brief Sets the current active camera.
	* @param the camera ID in the scene camera list
	*/
	inline void Scene::setActiveCamera(int id)
	{
		if(id>=0 && id<(int)m_cameras.size()) {
			curActiveCam=id;
		}
		else
		{
			throw Error("Scene::setActiveCamera error: Trying to activate a camera with an Invalid ID");
		}
	}
}

#endif // SCENE_H
