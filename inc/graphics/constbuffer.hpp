#ifndef XNA_GRAPHICS_CONSTBUFFER_HPP
#define XNA_GRAPHICS_CONSTBUFFER_HPP

#include "../default.hpp"

namespace xna {
	class IConstantBuffer {
	public:
		virtual ~IConstantBuffer(){}
		virtual bool Initialize(GraphicsDevice& device, xna_error_nullarg) = 0;
	};
}

#endif