#include "platform-dx/blendstate-dx.hpp"
#include "platform-dx/device-dx.hpp"

namespace xna {
	bool BlendState::Initialize(xna_error_ptr_arg)
	{
		if (!m_device || !m_device->_device) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (dxBlendState) {
			dxBlendState->Release();
			dxBlendState = nullptr;
		}

		const auto hr = m_device->_device->CreateBlendState(&dxDescription, &dxBlendState);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		return true;
	}

	bool BlendState::Apply(xna_error_ptr_arg) {
		if (!m_device || !m_device->_context) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (!dxBlendState) {
			xna_error_apply(err, XnaErrorCode::UNINTIALIZED_RESOURCE);
			return false;
		}
		
        m_device->_context->OMSetBlendState(dxBlendState, blendFactor, sampleMask);

        return true;
	}

	uptr<BlendState> BlendState::Opaque() {
		auto blendState = std::unique_ptr<BlendState>(new BlendState());		
		blendState->dxDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendState->dxDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendState->dxDescription.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_ALPHA;
		blendState->dxDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		return blendState;
	}

	uptr<BlendState> BlendState::AlphaBlend() {
		auto blendState = std::unique_ptr<BlendState>(new BlendState());
		blendState->dxDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendState->dxDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendState->dxDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendState->dxDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;

		return blendState;
	}

	uptr<BlendState> BlendState::Additive() {
		auto blendState = std::unique_ptr<BlendState>(new BlendState());
		blendState->dxDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendState->dxDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendState->dxDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendState->dxDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		return blendState;
	}

	uptr<BlendState> BlendState::NonPremultiplied() {
		auto blendState = std::unique_ptr<BlendState>(new BlendState());
		blendState->dxDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendState->dxDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendState->dxDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendState->dxDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;

		return blendState;
	}
}