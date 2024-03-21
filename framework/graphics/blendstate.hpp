#ifndef XNA_GRAPHICS_BLENDSTATE_HPP
#define XNA_GRAPHICS_BLENDSTATE_HPP

#include "../types.hpp"
#include "../forward.hpp"
#include "../enums.hpp"

namespace xna {
	class IBlendState {
	public:
		virtual ~IBlendState(){}

		static PBlendState Opaque();
		static PBlendState AlphaBlend();
		static PBlendState Additive();
		static PBlendState NonPremultiplied();
		
		virtual bool Apply(GraphicsDevice* device) = 0;

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
	};	
}

#endif