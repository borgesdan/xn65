#include "xna/graphics/blendstate.hpp"
#include "xna/graphics/gresource.hpp"
#include "xna/platform/dx.hpp"

namespace xna {
	BlendState::BlendState() : BlendState(nullptr) {}

	BlendState::BlendState(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		impl = unew<PlatformImplementation>();
		impl->dxDescription.AlphaToCoverageEnable = false;
		impl->dxDescription.IndependentBlendEnable = false;
		impl->dxDescription.RenderTarget[0].BlendEnable = true;
		impl->dxDescription.RenderTarget[0].SrcBlend = DxHelpers::BlendToDx(Blend::One);
		impl->dxDescription.RenderTarget[0].DestBlend = DxHelpers::BlendToDx(Blend::One);
		impl->dxDescription.RenderTarget[0].BlendOp = DxHelpers::BlendOperationToDx(BlendFunction::Add);
		impl->dxDescription.RenderTarget[0].SrcBlendAlpha = DxHelpers::BlendToDx(Blend::One);
		impl->dxDescription.RenderTarget[0].DestBlendAlpha = DxHelpers::BlendToDx(Blend::One);
		impl->dxDescription.RenderTarget[0].BlendOpAlpha = DxHelpers::BlendOperationToDx(BlendFunction::Add);
		impl->dxDescription.RenderTarget[0].RenderTargetWriteMask = DxHelpers::ColorWriteChannelsToDx(ColorWriteChannels::All);
	}

	BlendFunction BlendState::AlphaBlendFunction() const {
		return DxHelpers::BlendOperationToXna(impl->dxDescription.RenderTarget[0].BlendOpAlpha);
	}

	void BlendState::AlphaBlendFunction(BlendFunction value) {
		impl->dxDescription.RenderTarget[0].BlendOpAlpha = DxHelpers::BlendOperationToDx(value);
	}

	Blend BlendState::AlphaDestinationBlend() const {
		return DxHelpers::BlendToXna(impl->dxDescription.RenderTarget[0].DestBlendAlpha);
	}

	void BlendState::AlphaDestinationBlend(Blend value) {
		impl->dxDescription.RenderTarget[0].DestBlendAlpha = DxHelpers::BlendToDx(value);
	}

	Blend BlendState::AlphaSourceBlend() const {
		return DxHelpers::BlendToXna(impl->dxDescription.RenderTarget[0].SrcBlendAlpha);
	}

	void BlendState::AlphaSourceBlend(Blend value) {
		impl->dxDescription.RenderTarget[0].SrcBlendAlpha = DxHelpers::BlendToDx(value);
	}

	BlendFunction BlendState::ColorBlendFunction() const {
		return DxHelpers::BlendOperationToXna(impl->dxDescription.RenderTarget[0].BlendOp);
	}

	void BlendState::ColorBlendFunction(BlendFunction value) {
		impl->dxDescription.RenderTarget[0].BlendOp = DxHelpers::BlendOperationToDx(value);
	}

	Blend BlendState::ColorDestinationBlend() const {
		return DxHelpers::BlendToXna(impl->dxDescription.RenderTarget[0].DestBlend);
	}

	void BlendState::ColorDestinationBlend(Blend value) {
		impl->dxDescription.RenderTarget[0].DestBlend = DxHelpers::BlendToDx(value);
	}

	Blend BlendState::ColorSourceBlend() const {
		return DxHelpers::BlendToXna(impl->dxDescription.RenderTarget[0].SrcBlend);
	}

	void BlendState::ColorSourceBlend(Blend value) {
		impl->dxDescription.RenderTarget[0].SrcBlend = DxHelpers::BlendToDx(value);
	}
	
	Color BlendState::BlendFactor() const {
		auto color = Color(
			impl->blendFactor[0],
			impl->blendFactor[1],
			impl->blendFactor[2],
			impl->blendFactor[3]
		);

		return color;
	}

	void BlendState::BlendFactor(Color const& value) {
		auto v4 = value.ToVector4();

		impl->blendFactor[0] = v4.X;
		impl->blendFactor[1] = v4.Y;
		impl->blendFactor[2] = v4.Z;
		impl->blendFactor[3] = v4.W;
	}

	Int BlendState::MultiSampleMask() const {
		return static_cast<Int>(impl->sampleMask);
	}
	
	void BlendState::MultiSampleMast(Int value) {
		impl->sampleMask = static_cast<UINT>(value);
	}

	bool BlendState::Initialize()
	{
		if (!m_device || !m_device->impl->_device) {
			Exception::Throw(Exception::UNABLE_TO_INITIALIZE);
		}

		if (impl->dxBlendState) {
			impl->dxBlendState = nullptr;
		}

		const auto hr = m_device->impl->_device->CreateBlendState(
			&impl->dxDescription,
			impl->dxBlendState.GetAddressOf());

		if (FAILED(hr)) {
			Exception::Throw(Exception::FAILED_TO_CREATE);
		}

		return true;
	}

	bool BlendState::Apply() {
		if (!m_device || !m_device->impl->_context) {
			Exception::Throw(Exception::FAILED_TO_APPLY);
		}

		if (!impl->dxBlendState) {
			Exception::Throw(Exception::INVALID_OPERATION);
		}

		m_device->impl->_context->OMSetBlendState(
			impl->dxBlendState.Get(),
			impl->blendFactor,
			impl->sampleMask);

		return true;
	}

	void BlendState::AlphaToCoverageEnable(bool value) {
		impl->dxDescription.AlphaToCoverageEnable = value;
	}

	void BlendState::IndependentBlendEnable(bool value) {
		impl->dxDescription.IndependentBlendEnable = value;
	}

	void BlendState::RenderTargets(std::vector<BlendRenderTarget> const& value) {
		for (size_t i = 0; i < value.size() && i < 8; ++i) {
			impl->dxDescription.RenderTarget[i].BlendEnable = value[i].Enabled;
			impl->dxDescription.RenderTarget[i].SrcBlend = DxHelpers::BlendToDx(value[i].Source);
			impl->dxDescription.RenderTarget[i].DestBlend = DxHelpers::BlendToDx(value[i].Destination);
			impl->dxDescription.RenderTarget[i].BlendOp = DxHelpers::BlendOperationToDx(value[i].Operation);
			impl->dxDescription.RenderTarget[i].SrcBlendAlpha = DxHelpers::BlendToDx(value[i].SourceAlpha);
			impl->dxDescription.RenderTarget[i].DestBlendAlpha = DxHelpers::BlendToDx(value[i].DestinationAlpha);
			impl->dxDescription.RenderTarget[i].BlendOpAlpha = DxHelpers::BlendOperationToDx(value[i].OperationAlpha);
			impl->dxDescription.RenderTarget[i].RenderTargetWriteMask = DxHelpers::ColorWriteChannelsToDx(value[i].WriteMask);
		}
	}

	uptr<BlendState> BlendState::Opaque() {
		auto blendState = unew<BlendState>();
		blendState->impl->dxDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendState->impl->dxDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendState->impl->dxDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blendState->impl->dxDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		return blendState;
	}

	uptr<BlendState> BlendState::AlphaBlend() {
		auto blendState = unew<BlendState>();
		blendState->impl->dxDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendState->impl->dxDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendState->impl->dxDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendState->impl->dxDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;

		return blendState;
	}

	uptr<BlendState> BlendState::Additive() {
		auto blendState = unew<BlendState>();
		blendState->impl->dxDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendState->impl->dxDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendState->impl->dxDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendState->impl->dxDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		return blendState;
	}

	uptr<BlendState> BlendState::NonPremultiplied() {
		auto blendState = unew<BlendState>();
		blendState->impl->dxDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendState->impl->dxDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendState->impl->dxDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendState->impl->dxDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;

		return blendState;
	}
}