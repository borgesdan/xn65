#ifndef XNA_GRAPHICS_RENDERTARGET_HPP
#define XNA_GRAPHICS_RENDERTARGET_HPP

#include "../default.hpp"
#include "texture.hpp"

namespace xna {
	class RenderTarget2D : public Texture2D {
	public:
		RenderTarget2D();
		RenderTarget2D(sptr<GraphicsDevice> const& device);		
		

		static P_RenderTarget2D FromBackBuffer(P_GraphicsDevice const& device);

		void Initialize();
		void Apply();
	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl2 = nullptr;
	};	
}

#endif