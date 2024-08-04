#ifndef XNA_GRAPHICS_RENDERTARGET_HPP
#define XNA_GRAPHICS_RENDERTARGET_HPP

#include "../default.hpp"
#include "texture.hpp"

namespace xna {
	class RenderTarget2D : public Texture2D {
	public:
		RenderTarget2D();
		RenderTarget2D(sptr<GraphicsDevice> const& device);	
		
		//Gets the data format for the depth stencil data.
		constexpr DepthFormat DepthStencilFormat() const { return depthStencilFormat; }
		//Gets the number of sample locations during multisampling. 
		constexpr Int MultiSampleCount() const { return multiSampleCount; }
		//Gets or sets the render target usage. 
		constexpr RenderTargetUsage TargetUsage() const { return targetUsage; }

		void Initialize();
		void Apply();
	private:
		DepthFormat depthStencilFormat{ DepthFormat::None };
		Int multiSampleCount{ 0 };
		RenderTargetUsage targetUsage{ RenderTargetUsage::DiscardContents };

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl2 = nullptr;
	};	
}

#endif