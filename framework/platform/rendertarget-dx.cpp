#include "rendertarget-dx.hpp"
#include "../graphics/device.hpp"
#include "swapchain-dx.hpp"
#include "device-dx.hpp"

namespace xna {
	bool RenderTarget2D::Initialize(GraphicsDevice& device) {
		if (!device._device)
			return false;

		if (_texture2D) {
			_texture2D->Release();
			_texture2D = nullptr;
		}

		if (!device._swapChain->GetBackBuffer(_texture2D))
			return false;

		auto& dxdevice = device._device;
		
		const auto hr = dxdevice->CreateRenderTargetView(_texture2D, NULL, &_renderTargetView);

		if (FAILED(hr))
			return false;		

		return true;
	}

	bool RenderTarget2D::Apply(GraphicsDevice& device) {
		auto& context = device._context;
		context->OMSetRenderTargets(1, &_renderTargetView, nullptr);
		return true;
	}
}