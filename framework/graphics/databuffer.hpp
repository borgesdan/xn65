#ifndef XNA_GRAPHICS_DATABUFFER_HPP
#define XNA_GRAPHICS_DATABUFFER_HPP

#include "../default.hpp"

namespace xna {
	class IDataBuffer {
	public:
		virtual ~IDataBuffer(){}
		virtual bool Initialize(GraphicsDevice& device, xna_error_nullarg) = 0;
	};
}

#endif