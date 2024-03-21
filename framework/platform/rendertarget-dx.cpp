#include "rendertarget-dx.hpp"
#include "../graphics/device.hpp"
#include "swapchain-dx.hpp"
#include "device-dx.hpp"

namespace xna {
	RenderTarget2D::RenderTarget2D(GraphicsDevice* device) {
		_device = device;
	}

	bool RenderTarget2D::Apply() {
		if (_texture2D) {
			_texture2D->Release();
			_texture2D = nullptr;
		}

		if (!_device->GetSwapChainBackBuffer(_texture2D))
			return false;

		auto& device = _device->_device;

		if FAILED(device->CreateRenderTargetView(_texture2D, NULL, &_renderTargetView))
			return false;

		auto& context = _device->_context;
		context->OMSetRenderTargets(1, &_renderTargetView, nullptr);

		return true;
	}
}