#include "xna/graphics/gresource.hpp"

namespace xna {
	GraphicsResource::GraphicsResource(sptr<GraphicsDevice> const& device) : BaseGraphicsDevice(device) {}

	sptr<GraphicsDevice> GraphicsResource::Device() const {
		return BaseGraphicsDevice;
	}

	bool GraphicsResource::Bind(sptr<GraphicsDevice> const& device) {
		if (!device || device == BaseGraphicsDevice)
			return false;

		BaseGraphicsDevice = device;

		return true;
	}
}