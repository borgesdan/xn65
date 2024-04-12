#ifndef XNA_GRAPHICS_SHADER_HPP
#define XNA_GRAPHICS_SHADER_HPP

#include "../default.hpp"

namespace xna {
	class IShader {
	public:
		virtual ~IShader() {}		
		virtual bool Initialize(GraphicsDevice& device, DataBuffer& buffer, xna_error_nullarg) = 0;
	};
}

#endif