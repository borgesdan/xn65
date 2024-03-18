#include "blendstate-dx.hpp"
#include "device-dx.hpp"

namespace xna {
	BlendState::BlendState(GraphicsDevice* device) : _device(device) {
        ip_BlendState = New<InternalProperty>();
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

        auto& p = _device->ip_GraphicsDevice;
        if FAILED(p->_device->CreateBlendState(&blendDesc, &ip_BlendState->_blendState))
            return false;

        p->_context->OMSetBlendState(ip_BlendState->_blendState, nullptr, 0xffffffff);

        return true;
	}
}