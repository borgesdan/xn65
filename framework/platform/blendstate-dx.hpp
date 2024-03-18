#ifndef XNA_PLATFORM_BLENDSTATE_HPP
#define XNA_PLATFORM_BLENDSTATE_HPP

#include "../graphics/blendstate.hpp"
#include "dxgi.h"
#include "d3d11.h"

namespace xna {
	class BlendState::InternalProperty {
		friend class BlendState;
		friend class GraphicsDevice;
	public:
		~InternalProperty() {
			if (_blendState) {
				_blendState->Release();
				_blendState = nullptr;
			}
		}

	private:
		ID3D11BlendState* _blendState{ nullptr };
	};

	struct BlendMapper {
		static constexpr D3D11_BLEND ConvertBlend(Blend blend) {
			switch (blend)
			{	
			case xna::Blend::Zero:
				return D3D11_BLEND_ZERO;
			case xna::Blend::One:
				return D3D11_BLEND_ONE;
			case xna::Blend::SourceColor:
				return D3D11_BLEND_SRC_COLOR;
			case xna::Blend::InverseSourceColor:
				return D3D11_BLEND_INV_SRC_COLOR;
			case xna::Blend::SourceAlpha:
				return D3D11_BLEND_SRC_ALPHA;
			case xna::Blend::InverseSourceAlpha:
				return D3D11_BLEND_INV_SRC_ALPHA;
			case xna::Blend::DestinationAlpha:
				return D3D11_BLEND_DEST_ALPHA;
			case xna::Blend::InverseDestinationAlpha:
				return D3D11_BLEND_INV_DEST_ALPHA;
			case xna::Blend::DestinationColor:
				return D3D11_BLEND_DEST_COLOR;
			case xna::Blend::InverseDestinationColor:
				return D3D11_BLEND_INV_DEST_COLOR;
			case xna::Blend::SourceAlphaSaturation:
				return D3D11_BLEND_SRC_ALPHA_SAT;
			case xna::Blend::BlendFactor:
				return D3D11_BLEND_BLEND_FACTOR;
			case xna::Blend::InverseBlendFactor:
				return D3D11_BLEND_INV_BLEND_FACTOR;
			case xna::Blend::Source1Color:
				return D3D11_BLEND_SRC1_COLOR;
			case xna::Blend::InverseSource1Color:
				return D3D11_BLEND_INV_SRC1_COLOR;
			case xna::Blend::Source1Alpha:
				return D3D11_BLEND_SRC1_ALPHA;
			case xna::Blend::InverseSource1Alpha:
				return D3D11_BLEND_INV_SRC1_ALPHA;
			default:
				return D3D11_BLEND_ZERO;
			}
		}

		static constexpr D3D11_BLEND_OP ConvertOperation(BlendOperation op) {
			switch (op)
			{
			case BlendOperation::Add:
				return D3D11_BLEND_OP_ADD;
			case BlendOperation::Subtract:
				return D3D11_BLEND_OP_SUBTRACT;
			case BlendOperation::ReverseSubtract:
				return D3D11_BLEND_OP_REV_SUBTRACT;
			case BlendOperation::Min:
				return D3D11_BLEND_OP_MIN;
			case BlendOperation::Max:
				return D3D11_BLEND_OP_MAX;
			default:
				return D3D11_BLEND_OP_ADD;
			}
		}

		static constexpr D3D11_COLOR_WRITE_ENABLE ConvertColorWrite(ColorWriteChannels colorWrite) {
			switch (colorWrite)
			{
			case xna::ColorWriteChannels::Red:
				return D3D11_COLOR_WRITE_ENABLE_RED;
			case xna::ColorWriteChannels::Green:
				return D3D11_COLOR_WRITE_ENABLE_GREEN;
			case xna::ColorWriteChannels::Blue:
				return D3D11_COLOR_WRITE_ENABLE_BLUE;
			case xna::ColorWriteChannels::Alpha:
				return D3D11_COLOR_WRITE_ENABLE_ALPHA;
			case xna::ColorWriteChannels::All:
				return D3D11_COLOR_WRITE_ENABLE_ALL;
			default:
				return D3D11_COLOR_WRITE_ENABLE_ALL;
			}
		}
	};
}

#endif