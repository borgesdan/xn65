#include "xna/graphics/blendstate.hpp"
#include "xna/graphics/gresource.hpp"
#include "xna/platform-dx/dx.hpp"

namespace xna {
	BlendState::BlendState() : BlendState(nullptr) {}

	BlendState::BlendState(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		impl = unew<PlatformImplementation>();
		impl->dxDescription.AlphaToCoverageEnable = false;
		impl->dxDescription.IndependentBlendEnable = false;
		impl->dxDescription.RenderTarget[0].BlendEnable = true;
		impl->dxDescription.RenderTarget[0].SrcBlend = DxHelpers::ConvertBlend(Blend::One);
		impl->dxDescription.RenderTarget[0].DestBlend = DxHelpers::ConvertBlend(Blend::One);
		impl->dxDescription.RenderTarget[0].BlendOp = DxHelpers::ConvertOperation(BlendFunction::Add);
		impl->dxDescription.RenderTarget[0].SrcBlendAlpha = DxHelpers::ConvertBlend(Blend::One);
		impl->dxDescription.RenderTarget[0].DestBlendAlpha = DxHelpers::ConvertBlend(Blend::One);
		impl->dxDescription.RenderTarget[0].BlendOpAlpha = DxHelpers::ConvertOperation(BlendFunction::Add);
		impl->dxDescription.RenderTarget[0].RenderTargetWriteMask = DxHelpers::ConvertColorWrite(ColorWriteChannels::All);
	}

	BlendFunction BlendState::AlphaBlendFunction() const {
		return DxHelpers::ConvertOperationDx(impl->dxDescription.RenderTarget[0].BlendOpAlpha);
	}

	void BlendState::AlphaBlendFunction(BlendFunction value) {
		impl->dxDescription.RenderTarget[0].BlendOpAlpha = DxHelpers::ConvertOperation(value);
	}

	Blend BlendState::AlphaDestinationBlend() const {
		return DxHelpers::ConvertBlendDx(impl->dxDescription.RenderTarget[0].DestBlendAlpha);
	}

	void BlendState::AlphaDestinationBlend(Blend value) {
		impl->dxDescription.RenderTarget[0].DestBlendAlpha = DxHelpers::ConvertBlend(value);
	}

	Blend BlendState::AlphaSourceBlend() const {
		return DxHelpers::ConvertBlendDx(impl->dxDescription.RenderTarget[0].SrcBlendAlpha);
	}

	void BlendState::AlphaSourceBlend(Blend value) {
		impl->dxDescription.RenderTarget[0].SrcBlendAlpha = DxHelpers::ConvertBlend(value);
	}

	BlendFunction BlendState::ColorBlendFunction() const {
		return DxHelpers::ConvertOperationDx(impl->dxDescription.RenderTarget[0].BlendOp);
	}

	void BlendState::ColorBlendFunction(BlendFunction value) {
		impl->dxDescription.RenderTarget[0].BlendOp = DxHelpers::ConvertOperation(value);
	}

	Blend BlendState::ColorDestinationBlend() const {
		return DxHelpers::ConvertBlendDx(impl->dxDescription.RenderTarget[0].DestBlend);
	}

	void BlendState::ColorDestinationBlend(Blend value) {
		impl->dxDescription.RenderTarget[0].DestBlend = DxHelpers::ConvertBlend(value);
	}

	Blend BlendState::ColorSourceBlend() const {
		return DxHelpers::ConvertBlendDx(impl->dxDescription.RenderTarget[0].SrcBlend);
	}

	void BlendState::ColorSourceBlend(Blend value) {
		impl->dxDescription.RenderTarget[0].SrcBlend = DxHelpers::ConvertBlend(value);
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
			Exception::Throw(ExMessage::InitializeComponent);
		}

		if (impl->dxBlendState) {
			impl->dxBlendState->Release();
			impl->dxBlendState = nullptr;
		}

		const auto hr = m_device->impl->_device->CreateBlendState(
			&impl->dxDescription,
			&impl->dxBlendState);

		if (FAILED(hr)) {
			Exception::Throw(ExMessage::CreateComponent);
		}

		return true;
	}

	bool BlendState::Apply() {
		if (!m_device || !m_device->impl->_context) {
			Exception::Throw(ExMessage::ApplyComponent);
		}

		if (!impl->dxBlendState) {
			Exception::Throw(ExMessage::UnintializedComponent);
		}

		m_device->impl->_context->OMSetBlendState(
			impl->dxBlendState,
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
			impl->dxDescription.RenderTarget[i].SrcBlend = DxHelpers::ConvertBlend(value[i].Source);
			impl->dxDescription.RenderTarget[i].DestBlend = DxHelpers::ConvertBlend(value[i].Destination);
			impl->dxDescription.RenderTarget[i].BlendOp = DxHelpers::ConvertOperation(value[i].Operation);
			impl->dxDescription.RenderTarget[i].SrcBlendAlpha = DxHelpers::ConvertBlend(value[i].SourceAlpha);
			impl->dxDescription.RenderTarget[i].DestBlendAlpha = DxHelpers::ConvertBlend(value[i].DestinationAlpha);
			impl->dxDescription.RenderTarget[i].BlendOpAlpha = DxHelpers::ConvertOperation(value[i].OperationAlpha);
			impl->dxDescription.RenderTarget[i].RenderTargetWriteMask = DxHelpers::ConvertColorWrite(value[i].WriteMask);
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
		auto blendState = std::unique_ptr<BlendState>();
		blendState->impl->dxDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendState->impl->dxDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendState->impl->dxDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendState->impl->dxDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;

		return blendState;
	}

	uptr<BlendState> BlendState::Additive() {
		auto blendState = std::unique_ptr<BlendState>();
		blendState->impl->dxDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendState->impl->dxDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendState->impl->dxDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendState->impl->dxDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		return blendState;
	}

	uptr<BlendState> BlendState::NonPremultiplied() {
		auto blendState = std::unique_ptr<BlendState>();
		blendState->impl->dxDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendState->impl->dxDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendState->impl->dxDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendState->impl->dxDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;

		return blendState;
	}
}