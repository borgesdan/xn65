#include "xna/platform-dx/dx.hpp"

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

	bool RenderTarget2D::Initialize() {
		if (!impl || !m_device || !m_device->impl->_device) {
			Exception::Throw(ExMessage::InitializeComponent);
		}

		if (impl->dxTexture2D) {
			impl->dxTexture2D->Release();
			impl->dxTexture2D = nullptr;
		}

		if (!m_device->impl->_swapChain->impl->GetBackBuffer(impl->dxTexture2D))
			return false;

		auto& dxdevice = m_device->impl->_device;
		
		const auto hr = dxdevice->CreateRenderTargetView(impl->dxTexture2D, NULL, &render_impl->_renderTargetView);

		if (FAILED(hr)) {
			Exception::Throw(ExMessage::CreateComponent);
		}

		return true;
	}

	bool RenderTarget2D::Apply() {
		if (!m_device || !m_device->impl->_context) {
			Exception::Throw(ExMessage::ApplyComponent);
		}

		if(!render_impl->_renderTargetView)
			Exception::Throw(ExMessage::UnintializedComponent);

		auto& context = m_device->impl->_context;
		context->OMSetRenderTargets(1, &render_impl->_renderTargetView, nullptr);
		return true;
	}
}