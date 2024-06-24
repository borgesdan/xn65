#ifndef XNA_GRAPHICS_RENDERTARGET_HPP
#define XNA_GRAPHICS_RENDERTARGET_HPP

#include "../default.hpp"
#include "texture.hpp"

namespace xna {
	class RenderTarget2D : public Texture2D {
	public:
		RenderTarget2D();
		RenderTarget2D(sptr<GraphicsDevice> const& device);

		~RenderTarget2D() override;
		bool Initialize();
		bool Apply();

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> render_impl = nullptr;
	};	
}

#endif