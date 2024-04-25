#ifndef XNA_PLATFORM_BLENDSTATE_HPP
#define XNA_PLATFORM_BLENDSTATE_HPP

#include "../graphics/blendstate.hpp"
#include "../graphics/gresource.hpp"
#include "dxheaders.hpp"

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

	class BlendState : public IBlendState, public GraphicsResource {
	public:
		BlendState(GraphicsDevice* device) : GraphicsResource(device) {};

		virtual ~BlendState() override {
			if (dxBlendState) {
				dxBlendState->Release();
				dxBlendState = nullptr;
			}
		}
		virtual bool Initialize(xna_error_nullarg) override;
		
		virtual constexpr void AlphaToCoverageEnable(bool value) override {
			dxDescription.AlphaToCoverageEnable = value;
		}

		virtual constexpr void IndependentBlendEnable(bool value) override {
			dxDescription.IndependentBlendEnable = value;
		}

		virtual void RenderTargets(std::vector<BlendRenderTarget> const& value) override {
			for (size_t i = 0; i < value.size() && i < 8; ++i) {
				dxDescription.RenderTarget[i].BlendEnable = value[i].Enabled;
				dxDescription.RenderTarget[i].SrcBlend = ConvertBlend(value[i].Source);
				dxDescription.RenderTarget[i].DestBlend = ConvertBlend(value[i].Destination);
				dxDescription.RenderTarget[i].BlendOp = ConvertOperation(value[i].Operation);
				dxDescription.RenderTarget[i].SrcBlendAlpha = ConvertBlend(value[i].SourceAlpha);
				dxDescription.RenderTarget[i].DestBlendAlpha = ConvertBlend(value[i].DestinationAlpha);
				dxDescription.RenderTarget[i].BlendOpAlpha = ConvertOperation(value[i].OperationAlpha);
				dxDescription.RenderTarget[i].RenderTargetWriteMask = ConvertColorWrite(value[i].WriteMask);
			}
		}

		virtual bool Apply(xna_error_nullarg) override;

		static uptr<BlendState> Opaque();
		static uptr<BlendState> AlphaBlend();
		static uptr<BlendState> Additive();
		static uptr<BlendState> NonPremultiplied();

	public:
		ID3D11BlendState* dxBlendState{ nullptr };
		D3D11_BLEND_DESC dxDescription{};
		float blendFactor[4] { 1.0F, 1.0F, 1.0F, 1.0F };
		UINT sampleMask{ 0xffffffff }; 

	private:
		BlendState() : GraphicsResource(nullptr){}

	public:
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
			return static_cast<D3D11_BLEND_OP>(static_cast<int>(op) + 1);
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