#ifndef XNA_GRAPHICS_GRESOURCE_HPP
#define XNA_GRAPHICS_GRESOURCE_HPP

#include <memory>

namespace xna {
	class GraphicsDevice;

	//Queries and prepares resources. 
	class GraphicsResource {
	public:
		GraphicsResource(std::shared_ptr<GraphicsDevice> const& device);

		virtual ~GraphicsResource(){}

		virtual bool Bind(std::shared_ptr<GraphicsDevice> const& device);

		//Gets the GraphicsDevice associated with this GraphicsResource.
		std::shared_ptr<GraphicsDevice> Device() const;

	protected:
		std::shared_ptr<GraphicsDevice> BaseGraphicsDevice = nullptr;
	};
}

#endif