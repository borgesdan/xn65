#ifndef XNA_GRAPHICS_BLENDSTATE_HPP
#define XNA_GRAPHICS_BLENDSTATE_HPP

#include "../types.hpp"
#include "../forward.hpp"
#include "../enums.hpp"

namespace xna {
	class BlendState {
	public:
		PLATFORM_DEVELOPMENT
		BlendState(GraphicsDevice* device);

		static PBlendState Opaque() {
			auto blendState = New<BlendState>(nullptr);
			blendState->_source = Blend::SourceAlpha;
			blendState->_sourceAlpha = Blend::SourceAlpha;
			blendState->_destination = Blend::Zero;
			blendState->_destinationAlpha = Blend::Zero;

			return blendState;
		}

		static PBlendState AlphaBlend() {
			auto blendState = New<BlendState>(nullptr);
			blendState->_source = Blend::One;
			blendState->_sourceAlpha = Blend::One;
			blendState->_destination = Blend::InverseSourceAlpha;
			blendState->_destinationAlpha = Blend::InverseSourceAlpha;

			return blendState;
		}

		static PBlendState Additive() {
			auto blendState = New<BlendState>(nullptr);
			blendState->_source = Blend::SourceAlpha;
			blendState->_sourceAlpha = Blend::SourceAlpha;
			blendState->_destination = Blend::One;
			blendState->_destinationAlpha = Blend::One;

			return blendState;
		}

		static PBlendState NonPremultiplied() {
			auto blendState = New<BlendState>(nullptr);
			blendState->_source = Blend::SourceAlpha;
			blendState->_sourceAlpha = Blend::SourceAlpha;
			blendState->_destination = Blend::InverseSourceAlpha;
			blendState->_destinationAlpha = Blend::InverseSourceAlpha;

			return blendState;
		}

		PLATFORM_DEVELOPMENT
			bool Apply(GraphicsDevice* device);

	public:
		bool _alphaToCoverage{ false };
		bool _enabled{ true };
		Blend _source{ Blend::SourceAlpha };
		Blend _destination{ Blend::InverseSourceAlpha };
		BlendOperation _operation{ BlendOperation::Add };
		Blend _sourceAlpha{ Blend::One };
		Blend _destinationAlpha{ Blend::Zero };
		BlendOperation _operationAlpha{ BlendOperation::Add };
		ColorWriteChannels _writeMask{ ColorWriteChannels::All };
		GraphicsDevice* _device{nullptr};

	public:
		class InternalProperty;
		friend class InternalProperty;
		sptr<InternalProperty> ip_BlendState{ nullptr };
	};
}

#endif