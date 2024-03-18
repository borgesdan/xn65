#include "rendertarget-dx.hpp"
#include "../graphics/device.hpp"
#include "swapchain-dx.hpp"
#include "device-dx.hpp"

namespace xna {
	RenderTarget2D::RenderTarget2D(GraphicsDevice* device) : _device(device) {
		this->ip_RenderTarget2D = New<InternalProperty>();		
	}

	bool RenderTarget2D::Apply() {
		auto& p1 = this->ip_Texture2D;
		auto swapChain = _device->GetSwapChain();

		if (p1->_texture2D) {
			p1->_texture2D->Release();
			p1->_texture2D = nullptr;
		}

		if (!swapChain->ip_SwapChain->GetBackBuffer(p1->_texture2D))
			return false;

		auto& device = _device->ip_GraphicsDevice->_device;
		auto& p2 = this->ip_RenderTarget2D;

		if FAILED(device->CreateRenderTargetView(p1->_texture2D, NULL, &p2->_renderTargetView))
			return false;

		auto& context = _device->ip_GraphicsDevice->_context;
		context->OMSetRenderTargets(1, &p2->_renderTargetView, nullptr);

		return true;
	}
}