#ifndef XNA_GRAPHICS_INDEXBUFFER_HPP
#define XNA_GRAPHICS_INDEXBUFFER_HPP

#include "../default.hpp"

namespace xna {
	class IIndexBuffer {
	public:
		virtual ~IIndexBuffer() {}
		virtual bool Initialize(xna_error_nullarg) = 0;
	};
}

#endif