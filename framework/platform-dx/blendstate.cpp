#include "xna/graphics/blendstate.hpp"
#include "xna/graphics/gresource.hpp"
#include "xna/platform-dx/dx.hpp"

namespace xna {
	BlendState::BlendState() : GraphicsResource(nullptr) {
		impl = uNew<PlatformImplementation>();
	}

	BlendState::BlendState(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		impl = uNew<PlatformImplementation>();
	}

	BlendState::~BlendState() {
		impl = nullptr;
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
		auto blendState = uNew<BlendState>();
		blendState->impl->dxDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendState->impl->dxDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendState->impl->dxDescription.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_ALPHA;
		blendState->impl->dxDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		return blendState;
	}

	uptr<BlendState> BlendState::AlphaBlend() {
		auto blendState = std::unique_ptr<BlendState>(new BlendState());
		blendState->impl->dxDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendState->impl->dxDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendState->impl->dxDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendState->impl->dxDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;

		return blendState;
	}

	uptr<BlendState> BlendState::Additive() {
		auto blendState = std::unique_ptr<BlendState>(new BlendState());
		blendState->impl->dxDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendState->impl->dxDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendState->impl->dxDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendState->impl->dxDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		return blendState;
	}

	uptr<BlendState> BlendState::NonPremultiplied() {
		auto blendState = std::unique_ptr<BlendState>(new BlendState());
		blendState->impl->dxDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendState->impl->dxDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendState->impl->dxDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendState->impl->dxDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;

		return blendState;
	}
}