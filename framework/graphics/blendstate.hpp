#ifndef XNA_GRAPHICS_BLENDSTATE_HPP
#define XNA_GRAPHICS_BLENDSTATE_HPP

#include "../types.hpp"
#include "../forward.hpp"
#include "../enums.hpp"

namespace xna {
	struct BlendRenderTarget {
		bool Enabled{ true };
		Blend Source{ Blend::SourceAlpha };
		Blend Destination{ Blend::InverseSourceAlpha };
		BlendOperation Operation{ BlendOperation::Add };
		Blend SourceAlpha{ Blend::One };
		Blend DestinationAlpha{ Blend::Zero };
		BlendOperation OperationAlpha{ BlendOperation::Add };
		ColorWriteChannels WriteMask{ ColorWriteChannels::All };

		constexpr BlendRenderTarget() = default;
	};

	using PBlendRenderTarget = sptr<BlendRenderTarget>;

	class IBlendState {
	public:
		virtual ~IBlendState() {}

		static PBlendState Opaque();
		static PBlendState AlphaBlend();
		static PBlendState Additive();
		static PBlendState NonPremultiplied();
		
		virtual bool Apply(GraphicsDevice* device) = 0;

	public:
		bool AlphaToCoverage{ false };
		bool IndependentBlendEnable{ false };		
		PBlendRenderTarget RenderTargets[8];
	};	
}

#endif