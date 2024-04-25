#ifndef XNA_GRAPHICS_VERTEXBUFFER_HPP
#define XNA_GRAPHICS_VERTEXBUFFER_HPP

#include "../default.hpp"

namespace xna {
	class IVertexBuffer {
	public:
		virtual ~IVertexBuffer(){}
		virtual bool Initialize(xna_error_nullarg) = 0;
		virtual bool Apply(xna_error_nullarg) = 0;
	};
}

#endif