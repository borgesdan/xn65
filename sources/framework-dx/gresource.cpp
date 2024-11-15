#include "xna/graphics/gresource.hpp"
#include "xna-dx/framework.hpp"

namespace xna {
	GraphicsResource::GraphicsResource(std::shared_ptr<GraphicsDevice> const& device) : BaseGraphicsDevice(device) {}

	std::shared_ptr<GraphicsDevice> GraphicsResource::Device() const {
		return BaseGraphicsDevice;
	}

	bool GraphicsResource::Bind(std::shared_ptr<GraphicsDevice> const& device) {
		if (!device || device == BaseGraphicsDevice)
			return false;

		BaseGraphicsDevice = device;

		return true;
	}
}