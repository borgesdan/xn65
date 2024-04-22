#ifndef XNA_GRAPHICS_RENDERTARGET_HPP
#define XNA_GRAPHICS_RENDERTARGET_HPP

#include "texture.hpp"


namespace xna {
	class IRenderTarget2D {
	public:
		virtual ~IRenderTarget2D(){}

		virtual bool Initialize(GraphicsDevice& device) = 0;
		virtual bool Apply(GraphicsDevice& device) = 0;
	};	
}

#endif