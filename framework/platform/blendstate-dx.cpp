#include "blendstate-dx.hpp"
#include "device-dx.hpp"

namespace xna {
	BlendState::BlendState(GraphicsDevice* device) {
		_device = device;
	}

	bool BlendState::Apply(GraphicsDevice* device) {
		
		D3D11_BLEND_DESC blendDesc{};
        blendDesc.AlphaToCoverageEnable = _alphaToCoverage;
        blendDesc.IndependentBlendEnable = false;
        blendDesc.RenderTarget[0].BlendEnable = _enabled;
        blendDesc.RenderTarget[0].SrcBlend = BlendMapper::ConvertBlend(_source);
        blendDesc.RenderTarget[0].DestBlend = BlendMapper::ConvertBlend(_destination);
        blendDesc.RenderTarget[0].BlendOp = BlendMapper::ConvertOperation(_operation);
        blendDesc.RenderTarget[0].SrcBlendAlpha = BlendMapper::ConvertBlend(_sourceAlpha);
        blendDesc.RenderTarget[0].DestBlendAlpha = BlendMapper::ConvertBlend(_destinationAlpha);
        blendDesc.RenderTarget[0].BlendOpAlpha = BlendMapper::ConvertOperation(_operationAlpha);
        blendDesc.RenderTarget[0].RenderTargetWriteMask = BlendMapper::ConvertColorWrite(_writeMask);

        if (_device == nullptr || _device != device)
            _device = device;

        if FAILED(_device->_device->CreateBlendState(&blendDesc, &_blendState))
            return false;

        _device->_context->OMSetBlendState(_blendState, nullptr, 0xffffffff);

        return true;
	}

	PBlendState IBlendState::Opaque() {
		auto blendState = New<BlendState>(nullptr);
		blendState->_source = Blend::SourceAlpha;
		blendState->_sourceAlpha = Blend::SourceAlpha;
		blendState->_destination = Blend::Zero;
		blendState->_destinationAlpha = Blend::Zero;

		return blendState;
	}

	PBlendState IBlendState::AlphaBlend() {
		auto blendState = New<BlendState>(nullptr);
		blendState->_source = Blend::One;
		blendState->_sourceAlpha = Blend::One;
		blendState->_destination = Blend::InverseSourceAlpha;
		blendState->_destinationAlpha = Blend::InverseSourceAlpha;

		return blendState;
	}

	PBlendState IBlendState::Additive() {
		auto blendState = New<BlendState>(nullptr);
		blendState->_source = Blend::SourceAlpha;
		blendState->_sourceAlpha = Blend::SourceAlpha;
		blendState->_destination = Blend::One;
		blendState->_destinationAlpha = Blend::One;

		return blendState;
	}

	PBlendState IBlendState::NonPremultiplied() {
		auto blendState = New<BlendState>(nullptr);
		blendState->_source = Blend::SourceAlpha;
		blendState->_sourceAlpha = Blend::SourceAlpha;
		blendState->_destination = Blend::InverseSourceAlpha;
		blendState->_destinationAlpha = Blend::InverseSourceAlpha;

		return blendState;
	}
}