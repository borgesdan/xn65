#include "xna/graphics/blendstate.hpp"
#include "xna/graphics/gresource.hpp"
#include "xna-dx/framework.hpp"

namespace xna {
	BlendState::BlendState() : BlendState(nullptr) {}

	BlendState::BlendState(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		Implementation = unew<BlendStateImplementation>();
		Implementation->Description.AlphaToCoverageEnable = false;
		Implementation->Description.IndependentBlendEnable = false;
		Implementation->Description.RenderTarget[0].BlendEnable = true;
		Implementation->Description.RenderTarget[0].SrcBlend = DxHelpers::BlendToDx(Blend::One);
		Implementation->Description.RenderTarget[0].DestBlend = DxHelpers::BlendToDx(Blend::One);
		Implementation->Description.RenderTarget[0].BlendOp = DxHelpers::BlendOperationToDx(BlendFunction::Add);
		Implementation->Description.RenderTarget[0].SrcBlendAlpha = DxHelpers::BlendToDx(Blend::One);
		Implementation->Description.RenderTarget[0].DestBlendAlpha = DxHelpers::BlendToDx(Blend::One);
		Implementation->Description.RenderTarget[0].BlendOpAlpha = DxHelpers::BlendOperationToDx(BlendFunction::Add);
		Implementation->Description.RenderTarget[0].RenderTargetWriteMask = DxHelpers::ColorWriteChannelsToDx(ColorWriteChannels::All);
	}

	BlendFunction BlendState::AlphaBlendFunction() const {
		return DxHelpers::BlendOperationToXna(Implementation->Description.RenderTarget[0].BlendOpAlpha);
	}

	void BlendState::AlphaBlendFunction(BlendFunction value) {
		Implementation->Description.RenderTarget[0].BlendOpAlpha = DxHelpers::BlendOperationToDx(value);
	}

	Blend BlendState::AlphaDestinationBlend() const {
		return DxHelpers::BlendToXna(Implementation->Description.RenderTarget[0].DestBlendAlpha);
	}

	void BlendState::AlphaDestinationBlend(Blend value) {
		Implementation->Description.RenderTarget[0].DestBlendAlpha = DxHelpers::BlendToDx(value);
	}

	Blend BlendState::AlphaSourceBlend() const {
		return DxHelpers::BlendToXna(Implementation->Description.RenderTarget[0].SrcBlendAlpha);
	}

	void BlendState::AlphaSourceBlend(Blend value) {
		Implementation->Description.RenderTarget[0].SrcBlendAlpha = DxHelpers::BlendToDx(value);
	}

	BlendFunction BlendState::ColorBlendFunction() const {
		return DxHelpers::BlendOperationToXna(Implementation->Description.RenderTarget[0].BlendOp);
	}

	void BlendState::ColorBlendFunction(BlendFunction value) {
		Implementation->Description.RenderTarget[0].BlendOp = DxHelpers::BlendOperationToDx(value);
	}

	Blend BlendState::ColorDestinationBlend() const {
		return DxHelpers::BlendToXna(Implementation->Description.RenderTarget[0].DestBlend);
	}

	void BlendState::ColorDestinationBlend(Blend value) {
		Implementation->Description.RenderTarget[0].DestBlend = DxHelpers::BlendToDx(value);
	}

	Blend BlendState::ColorSourceBlend() const {
		return DxHelpers::BlendToXna(Implementation->Description.RenderTarget[0].SrcBlend);
	}

	void BlendState::ColorSourceBlend(Blend value) {
		Implementation->Description.RenderTarget[0].SrcBlend = DxHelpers::BlendToDx(value);
	}
	
	Color BlendState::BlendFactor() const {
		auto color = Color(
			Implementation->BlendFactor[0],
			Implementation->BlendFactor[1],
			Implementation->BlendFactor[2],
			Implementation->BlendFactor[3]
		);

		return color;
	}

	void BlendState::BlendFactor(Color const& value) {
		auto v4 = value.ToVector4();

		Implementation->BlendFactor[0] = v4.X;
		Implementation->BlendFactor[1] = v4.Y;
		Implementation->BlendFactor[2] = v4.Z;
		Implementation->BlendFactor[3] = v4.W;
	}

	Int BlendState::MultiSampleMask() const {
		return static_cast<Int>(Implementation->SampleMask);
	}
	
	void BlendState::MultiSampleMask(Int value) {
		Implementation->SampleMask = static_cast<UINT>(value);
	}

	bool BlendState::Initialize()
	{
		if (!m_device || !m_device->Implementation->Device) {
			Exception::Throw(Exception::UNABLE_TO_INITIALIZE);
		}

		if (Implementation->BlendState) {
			Implementation->BlendState = nullptr;
		}

		const auto hr = m_device->Implementation->Device->CreateBlendState(
			&Implementation->Description,
			Implementation->BlendState.GetAddressOf());

		if (FAILED(hr)) {
			Exception::Throw(Exception::FAILED_TO_CREATE);
		}

		return true;
	}

	bool BlendState::Apply() {
		if (!m_device || !m_device->Implementation->Context) {
			Exception::Throw(Exception::FAILED_TO_APPLY);
		}

		if (!Implementation->BlendState) {
			Initialize();
		}

		m_device->Implementation->Context->OMSetBlendState(
			Implementation->BlendState.Get(),
			Implementation->BlendFactor,
			Implementation->SampleMask);

		return true;
	}

	void BlendState::AlphaToCoverageEnable(bool value) {
		Implementation->Description.AlphaToCoverageEnable = value;
	}

	void BlendState::IndependentBlendEnable(bool value) {
		Implementation->Description.IndependentBlendEnable = value;
	}
	
	void BlendState::RenderTargets(BlendRenderTarget const* value, size_t size) {
		for (size_t i = 0; i < size && i < Implementation->MAX_RENDER_TARGETS; ++i) {
			Implementation->Description.RenderTarget[i].BlendEnable = value[i].Enabled;
			Implementation->Description.RenderTarget[i].SrcBlend = DxHelpers::BlendToDx(value[i].Source);
			Implementation->Description.RenderTarget[i].DestBlend = DxHelpers::BlendToDx(value[i].Destination);
			Implementation->Description.RenderTarget[i].BlendOp = DxHelpers::BlendOperationToDx(value[i].Operation);
			Implementation->Description.RenderTarget[i].SrcBlendAlpha = DxHelpers::BlendToDx(value[i].SourceAlpha);
			Implementation->Description.RenderTarget[i].DestBlendAlpha = DxHelpers::BlendToDx(value[i].DestinationAlpha);
			Implementation->Description.RenderTarget[i].BlendOpAlpha = DxHelpers::BlendOperationToDx(value[i].OperationAlpha);
			Implementation->Description.RenderTarget[i].RenderTargetWriteMask = DxHelpers::ColorWriteChannelsToDx(value[i].WriteMask);
		}
	}

	uptr<BlendState> BlendState::Opaque() {
		auto blendState = unew<BlendState>();
		blendState->Implementation->Description.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendState->Implementation->Description.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendState->Implementation->Description.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blendState->Implementation->Description.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		return blendState;
	}

	uptr<BlendState> BlendState::AlphaBlend() {
		auto blendState = unew<BlendState>();
		blendState->Implementation->Description.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendState->Implementation->Description.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendState->Implementation->Description.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendState->Implementation->Description.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;

		return blendState;
	}

	uptr<BlendState> BlendState::Additive() {
		auto blendState = unew<BlendState>();
		blendState->Implementation->Description.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendState->Implementation->Description.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendState->Implementation->Description.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendState->Implementation->Description.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		return blendState;
	}

	uptr<BlendState> BlendState::NonPremultiplied() {
		auto blendState = unew<BlendState>();
		blendState->Implementation->Description.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendState->Implementation->Description.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendState->Implementation->Description.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendState->Implementation->Description.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;

		return blendState;
	}
}