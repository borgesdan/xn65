#include "xna/graphics/gresource.hpp"

namespace xna {
	GraphicsResource::GraphicsResource(sptr<GraphicsDevice> const& device) : m_device(device) {}

	sptr<GraphicsDevice> GraphicsResource::Device() const {
		return m_device;
	}

	bool GraphicsResource::Bind(sptr<GraphicsDevice> const& device) {
		if (!device || device == m_device)
			return false;

		m_device = device;

		return true;
	}
}