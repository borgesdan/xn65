#include "graphics/blendstate.hpp"
#include "graphics/gresource.hpp"
#include "platform-dx/headers.hpp"
#include "platform-dx/helpers.hpp"
#include "graphics/blendstate.hpp"
#include "platform-dx/implementations.hpp"

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

	bool BlendState::Initialize(xna_error_ptr_arg)
	{
		if (!m_device || !m_device->impl->_device) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (impl->dxBlendState) {
			impl->dxBlendState->Release();
			impl->dxBlendState = nullptr;
		}

		const auto hr = m_device->impl->_device->CreateBlendState(
			&impl->dxDescription,
			&impl->dxBlendState);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		return true;
	}

	bool BlendState::Apply(xna_error_ptr_arg) {
		if (!m_device || !m_device->impl->_context) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (!impl->dxBlendState) {
			xna_error_apply(err, XnaErrorCode::UNINTIALIZED_RESOURCE);
			return false;
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