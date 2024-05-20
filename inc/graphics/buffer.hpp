#ifndef XNA_GRAPHICS_BUFFER_HPP
#define XNA_GRAPHICS_BUFFER_HPP

#include "../default.hpp"
#include "gresource.hpp"

namespace xna {
	class ConstantBuffer : public GraphicsResource {
	public:
		ConstantBuffer();
		ConstantBuffer(sptr<GraphicsDevice> const&);
		~ConstantBuffer();
		bool Initialize(xna_error_nullarg);

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
}

#endif