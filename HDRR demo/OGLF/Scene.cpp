#include "Scene.h"

using namespace std;

namespace Oglf
{
	/**
	* @brief draws the scene content
	*/
	void Scene::drawElements()
	{
		// drawing these meshes with the fixed pipeline
		GLSLshaderProgram::useFixedPipeline();
		for (mIt = m_defaultShadingMeshList.begin(); mIt != m_defaultShadingMeshList.end(); mIt++)
		{
			glPushMatrix();
			(*mIt)->transform();
			(*mIt)->draw();
			glPopMatrix();
		}

		for (lIt = m_lights.begin(); lIt != m_lights.end(); lIt++)
		{
			if ((*lIt)->isTransformable())
			{
				glPushMatrix();
				(*lIt)->transform();
				(*lIt)->draw();
				glPopMatrix();
			}
			else
			{
				(*lIt)->draw();
			}
		}

		for (rIt = m_renderingFXmeshAttachmentList.begin(); rIt != m_renderingFXmeshAttachmentList.end(); rIt++)
		{
			rIt->rFX->enable(); // drawing these meshes with the rendering effect they have been attached to
			for (mIt = (*rIt).attachedMeshes.begin(); mIt != (*rIt).attachedMeshes.end(); mIt++)
			{
				m_pWorldMat = &( ( *mIt )->getTransformer().getTransformMatrix() );
				rIt->rFX->updateParameterLocation( m_iMatID, m_pWorldMat );
				rIt->rFX->refreshParameter( m_iMatID );

				glPushMatrix();
				(*mIt)->transform();
				(*mIt)->draw();
				glPopMatrix();
			}
			(*rIt).rFX->disable();
		}
	}

	/**
	* @brief configure the renderer using the rendering configuration object
	*/
	void Scene::configureRenderer()
	{
		if(m_rConf==NULL) {
			throw Error("Scene::getRenderingConfiguration error: no rendering configuration for this scene");
		}
		else {
			m_rConf->configureRenderer();
		}
	}

	/**
	* @brief adds a mesh in the scene
	* @param  m a mesh
	* @param rFXid the rendering effect id to attach to.
	*/
	void Scene::addMesh ( Mesh& m, int rFXid )
	{
		if(rFXid >= 0 && rFXid < (int)m_renderingFXmeshAttachmentList.size()) {
			m_renderingFXmeshAttachmentList[rFXid].attachedMeshes.push_back(&m);
		}
		else if(rFXid==-1) {
			m_defaultShadingMeshList.push_back(&m);
		}
		else {
			throw Error("Scene::addMesh error: can not attach a mesh to an inexistant rendering effect");
		}
	}

	bool Scene::removeMesh ( const std::string& sName )
	{
		for (rIt = m_renderingFXmeshAttachmentList.begin(); rIt != m_renderingFXmeshAttachmentList.end(); ++rIt )
		{
			for (mIt = (*rIt).attachedMeshes.begin(); mIt != (*rIt).attachedMeshes.end(); ++mIt )
			{
				if( !(* mIt )->getName().compare( sName ) )
				{
					m_renderingFXmeshAttachmentList.erase( rIt );
					return true;
				}
			}
		}
		for (mIt = m_defaultShadingMeshList.begin(); mIt != m_defaultShadingMeshList.end(); ++mIt )
		{
			if( !( *mIt )->getName().compare( sName ) )
			{
				m_defaultShadingMeshList.erase( mIt );
				return true;
			}
		}
		return false;
	}

	void Scene::removeAllMeshes()
	{
		m_renderingFXmeshAttachmentList.clear();
		m_defaultShadingMeshList.clear();
	}
}

