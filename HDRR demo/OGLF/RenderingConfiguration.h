#ifndef RENDERINGCONFIGURATION_H
#define RENDERINGCONFIGURATION_H

namespace Oglf
{
	class Renderer;

	/**
	* @brief Sets the rendering configuration used in each scene for a given renderer.
	*/
	class RenderingConfiguration
	{
	protected:
		Renderer* m_renderer;

	public:
		/**
		* @brief Constructor.
		* @param a reference to a renderer
		*/
		RenderingConfiguration(Renderer& renderer) : m_renderer(&renderer)
		{

		}

		virtual ~RenderingConfiguration()
		{

		}

		/**
		* @brief Configure the renderer. This function must be overloaded to do something.
		*/
		virtual void configureRenderer()
		{

		}
	};
}


#endif /*RENDERINGCONFIGURATION_H*/
