#ifndef XNA_GRAPHICS_BLENDSTATE_HPP
#define XNA_GRAPHICS_BLENDSTATE_HPP

#include "../default.hpp"

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
		virtual bool Initialize(GraphicsDevice& device, xna_error_nullarg) = 0;
		virtual void AlphaToCoverageEnable(bool value) = 0;
		virtual void IndependentBlendEnable(bool value) = 0;
		virtual void RenderTargets(std::vector<BlendRenderTarget> const& value) = 0;

		virtual bool Apply(GraphicsDevice& device, xna_error_nullarg) = 0;

		static PBlendState Opaque();
		static PBlendState AlphaBlend();
		static PBlendState Additive();
		static PBlendState NonPremultiplied();
	};	
}

#endif