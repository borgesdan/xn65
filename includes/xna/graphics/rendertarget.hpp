#ifndef XNA_GRAPHICS_RENDERTARGET_HPP
#define XNA_GRAPHICS_RENDERTARGET_HPP

#include "../default.hpp"
#include "texture.hpp"

namespace xna {
	struct RenderTarget2DImplementation;

	//Contains a 2D texture that can be used as a render target. 
	class RenderTarget2D : public Texture2D {
	public:
		RenderTarget2D();
		RenderTarget2D(sptr<GraphicsDevice> const& device);				

		static P_RenderTarget2D FromBackBuffer(P_GraphicsDevice const& device);

		void Initialize();
	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl2 = nullptr;
	};	
}

#endif