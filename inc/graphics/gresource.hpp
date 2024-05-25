#ifndef XNA_GRAPHICS_GRESOURCE_HPP
#define XNA_GRAPHICS_GRESOURCE_HPP

#include "../default.hpp"

namespace xna {
	class GraphicsResource {
	public:
		GraphicsResource(sptr<GraphicsDevice> const& device) : m_device(device){}

		virtual ~GraphicsResource(){}

		virtual bool Bind(sptr<GraphicsDevice> const& device) {
			if (!device || device == m_device)
				return false;

			m_device = device;

			return true;
		}

	protected:
		sptr<GraphicsDevice> m_device = nullptr;
	};
}

#endif