#ifndef XNA_GRAPHICS_RENDERTARGET_HPP
#define XNA_GRAPHICS_RENDERTARGET_HPP

#include "texture.hpp"


namespace xna {
	class IRenderTarget2D {
	public:
		virtual ~IRenderTarget2D(){}

		virtual bool Bind() = 0;
	};	
}

#endif