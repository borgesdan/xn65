#ifndef XNA_GRAPHICS_GRESOURCE_HPP
#define XNA_GRAPHICS_GRESOURCE_HPP

#include "../default.hpp"

namespace xna {
	class GraphicsResource {
	public:
		GraphicsResource(GraphicsDevice* device) : m_device(device){}

		virtual ~GraphicsResource(){}

		virtual bool Bind(GraphicsDevice* device) {
			if (device == m_device)
				return false;

			m_device = device;

			return true;
		}

	protected:
		GraphicsDevice* m_device = nullptr;
	};
}

#endif