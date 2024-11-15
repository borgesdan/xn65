#ifndef XNA_GRAPHICS_GRESOURCE_HPP
#define XNA_GRAPHICS_GRESOURCE_HPP

#include "../default.hpp"

namespace xna {
	//Queries and prepares resources. 
	class GraphicsResource {
	public:
		GraphicsResource(sptr<GraphicsDevice> const& device);

		virtual ~GraphicsResource(){}

		virtual bool Bind(sptr<GraphicsDevice> const& device);

		//Gets the GraphicsDevice associated with this GraphicsResource.
		sptr<xna::GraphicsDevice> Device() const;

	protected:
		sptr<GraphicsDevice> BaseGraphicsDevice = nullptr;
	};
}

#endif