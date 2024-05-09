#include "rendertarget-dx.hpp"
#include "../graphics/device.hpp"
#include "swapchain-dx.hpp"
#include "device-dx.hpp"

namespace xna {
	bool RenderTarget2D::Initialize(xna_error_ptr_arg) {
		if (!m_device || !m_device->_device) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (dxTexture2D) {
			dxTexture2D->Release();
			dxTexture2D = nullptr;
		}

		if (!m_device->_swapChain->GetBackBuffer(dxTexture2D))
			return false;

		auto& dxdevice = m_device->_device;
		
		const auto hr = dxdevice->CreateRenderTargetView(dxTexture2D, NULL, &_renderTargetView);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;		
		}

		return true;
	}

	bool RenderTarget2D::Apply(xna_error_ptr_arg) {
		if (!m_device || !m_device->_context) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		auto& context = m_device->_context;
		context->OMSetRenderTargets(1, &_renderTargetView, nullptr);
		return true;
	}
}