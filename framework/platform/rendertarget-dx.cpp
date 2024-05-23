#include "graphics/device.hpp"
#include "platform-dx/device-dx.hpp"
#include "platform-dx/implementations.hpp"
#include "graphics/rendertarget.hpp"

namespace xna {
	RenderTarget2D::RenderTarget2D() : Texture2D() {
		render_impl = unew<PlatformImplementation>();
	}

	RenderTarget2D::RenderTarget2D(sptr<GraphicsDevice> const& device) : Texture2D(device) {
		render_impl = unew<PlatformImplementation>();
	}

	RenderTarget2D::~RenderTarget2D() {
		render_impl = nullptr;
	}

	bool RenderTarget2D::Initialize(xna_error_ptr_arg) {
		if (!impl || !m_device || !m_device->_device) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		if (impl->dxTexture2D) {
			impl->dxTexture2D->Release();
			impl->dxTexture2D = nullptr;
		}

		if (!m_device->_swapChain->impl->GetBackBuffer(impl->dxTexture2D))
			return false;

		auto& dxdevice = m_device->_device;
		
		const auto hr = dxdevice->CreateRenderTargetView(impl->dxTexture2D, NULL, &render_impl->_renderTargetView);

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
		context->OMSetRenderTargets(1, &render_impl->_renderTargetView, nullptr);
		return true;
	}
}