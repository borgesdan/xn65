#ifndef XNA_GRAPHICS_BLENDSTATE_HPP
#define XNA_GRAPHICS_BLENDSTATE_HPP

#include "../default.hpp"
#include "gresource.hpp"

namespace xna {
	struct BlendRenderTarget;

	class BlendState : public GraphicsResource {
	public:
		BlendState();
		BlendState(sptr<GraphicsDevice> const& device);
		~BlendState() override;
		bool Initialize(xna_error_nullarg) ;
		void AlphaToCoverageEnable(bool value) ;
		void IndependentBlendEnable(bool value) ;
		void RenderTargets(std::vector<BlendRenderTarget> const& value);
		bool Apply(xna_error_nullarg);

		static uptr<BlendState> Opaque();
		static uptr<BlendState> AlphaBlend();
		static uptr<BlendState> Additive();
		static uptr<BlendState> NonPremultiplied();

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};	
}

#endif