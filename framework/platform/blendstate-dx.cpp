#include "blendstate-dx.hpp"
#include "device-dx.hpp"

namespace xna {
	BlendState::BlendState(GraphicsDevice* device) {
		_device = device;
	}

	bool BlendState::Apply(GraphicsDevice* device) {
		
		D3D11_BLEND_DESC blendDesc{};
        blendDesc.AlphaToCoverageEnable = AlphaToCoverage;
        blendDesc.IndependentBlendEnable = IndependentBlendEnable;
        blendDesc.RenderTarget[0].BlendEnable = RenderTargets[0]->Enabled;
        blendDesc.RenderTarget[0].SrcBlend = BlendMapper::ConvertBlend(RenderTargets[0]->Source);
        blendDesc.RenderTarget[0].DestBlend = BlendMapper::ConvertBlend(RenderTargets[0]->Destination);
        blendDesc.RenderTarget[0].BlendOp = BlendMapper::ConvertOperation(RenderTargets[0]->Operation);
        blendDesc.RenderTarget[0].SrcBlendAlpha = BlendMapper::ConvertBlend(RenderTargets[0]->SourceAlpha);
        blendDesc.RenderTarget[0].DestBlendAlpha = BlendMapper::ConvertBlend(RenderTargets[0]->DestinationAlpha);
        blendDesc.RenderTarget[0].BlendOpAlpha = BlendMapper::ConvertOperation(RenderTargets[0]->OperationAlpha);
        blendDesc.RenderTarget[0].RenderTargetWriteMask = BlendMapper::ConvertColorWrite(RenderTargets[0]->WriteMask);

        if (_device == nullptr || _device != device)
            _device = device;

        if FAILED(_device->_device->CreateBlendState(&blendDesc, &_blendState))
            return false;

        _device->_context->OMSetBlendState(_blendState, nullptr, 0xffffffff);

        return true;
	}

	PBlendState IBlendState::Opaque() {
		auto blendState = New<BlendState>(nullptr);
		blendState->RenderTargets[0] = New<BlendRenderTarget>();
		blendState->RenderTargets[0]->Source = Blend::SourceAlpha;
		blendState->RenderTargets[0]->SourceAlpha = Blend::SourceAlpha;
		blendState->RenderTargets[0]->Destination = Blend::Zero;
		blendState->RenderTargets[0]->DestinationAlpha = Blend::Zero;

		return blendState;
	}

	PBlendState IBlendState::AlphaBlend() {
		auto blendState = New<BlendState>(nullptr);
		blendState->RenderTargets[0] = New<BlendRenderTarget>();
		blendState->RenderTargets[0]->Source = Blend::One;
		blendState->RenderTargets[0]->SourceAlpha = Blend::One;
		blendState->RenderTargets[0]->Destination = Blend::InverseSourceAlpha;
		blendState->RenderTargets[0]->DestinationAlpha = Blend::InverseSourceAlpha;

		return blendState;
	}

	PBlendState IBlendState::Additive() {
		auto blendState = New<BlendState>(nullptr);
		blendState->RenderTargets[0] = New<BlendRenderTarget>();
		blendState->RenderTargets[0]->Source = Blend::SourceAlpha;
		blendState->RenderTargets[0]->SourceAlpha = Blend::SourceAlpha;
		blendState->RenderTargets[0]->Destination = Blend::One;
		blendState->RenderTargets[0]->DestinationAlpha = Blend::One;

		return blendState;
	}

	PBlendState IBlendState::NonPremultiplied() {
		auto blendState = New<BlendState>(nullptr);
		blendState->RenderTargets[0] = New<BlendRenderTarget>();
		blendState->RenderTargets[0]->Source = Blend::SourceAlpha;
		blendState->RenderTargets[0]->SourceAlpha = Blend::SourceAlpha;
		blendState->RenderTargets[0]->Destination = Blend::InverseSourceAlpha;
		blendState->RenderTargets[0]->DestinationAlpha = Blend::InverseSourceAlpha;

		return blendState;
	}
}