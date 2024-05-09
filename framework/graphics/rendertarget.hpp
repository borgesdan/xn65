#ifndef XNA_GRAPHICS_RENDERTARGET_HPP
#define XNA_GRAPHICS_RENDERTARGET_HPP

#include "../default.hpp"


namespace xna {
	class IRenderTarget2D {
	public:
		virtual ~IRenderTarget2D(){}

		virtual bool Initialize(xna_error_nullarg) = 0;
		virtual bool Apply(xna_error_nullarg) = 0;
	};	
}

#endif