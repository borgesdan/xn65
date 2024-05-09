#ifndef XNA_GRAPHICS_VERTEXINPUT_HPP
#define XNA_GRAPHICS_VERTEXINPUT_HPP

#include "../default.hpp"

namespace xna {
	class IVertexInputLayout {
	public:
		virtual ~IVertexInputLayout(){}
		virtual bool Initialize(GraphicsDevice& device, DataBuffer& blob, xna_error_nullarg) = 0;
	};
}

#endif