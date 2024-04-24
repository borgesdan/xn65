#include "blendstate-dx.hpp"
#include "device-dx.hpp"

namespace xna {
	bool BlendState::Initialize(GraphicsDevice& device, xna_error_ptr_arg)
	{
		if (!device._device) {
			xna_error_apply(err, XnaErrorCode::ARGUMENT_IS_NULL);
			return false;
		}

		if (_blendState) {
			_blendState->Release();
			_blendState = nullptr;
		}

		const auto hr = device._device->CreateBlendState(&_description, &_blendState);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		return true;
	}

	bool BlendState::Apply(GraphicsDevice& device, xna_error_ptr_arg) {
		if (!device._context) {
			xna_error_apply(err, XnaErrorCode::ARGUMENT_IS_NULL);
			return false;
		}

		if (!_blendState) {
			const auto init = Initialize(device, err);
			if (!init) return false;
		}
		
        device._context->OMSetBlendState(_blendState, blendFactor, sampleMask);

        return true;
	}

	PBlendState IBlendState::Opaque() {
		auto blendState = New<BlendState>();		
		blendState->_description.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendState->_description.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blendState->_description.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_ALPHA;
		blendState->_description.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

		return blendState;
	}

	PBlendState IBlendState::AlphaBlend() {
		auto blendState = New<BlendState>();
		blendState->_description.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendState->_description.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendState->_description.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendState->_description.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;

		return blendState;
	}

	PBlendState IBlendState::Additive() {
		auto blendState = New<BlendState>();
		blendState->_description.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendState->_description.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendState->_description.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendState->_description.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		return blendState;
	}

	PBlendState IBlendState::NonPremultiplied() {
		auto blendState = New<BlendState>();
		blendState->_description.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendState->_description.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendState->_description.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendState->_description.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;

		return blendState;
	}
}