#include "xna/xna-dx.hpp"

namespace xna {
	RenderTarget2D::RenderTarget2D(sptr<GraphicsDevice> const& device) : Texture2D(device) {
		impl2 = unew<PlatformImplementation>();
	}	

	void RenderTarget2D::Initialize() {
		if (!impl || !m_device || !m_device->impl->_device) {
			Exception::Throw(Exception::UNABLE_TO_INITIALIZE);
		}		

		auto& swapChain = m_device->impl->_swapChain;

		if (!swapChain->impl->GetBackBuffer(impl->dxTexture2D))
		{
			Exception::Throw(Exception::FAILED_TO_CREATE);
		}

		impl->dxTexture2D->GetDesc(&impl->dxDescription);
		auto& dxdevice = m_device->impl->_device;
		
		const auto hr = dxdevice->CreateRenderTargetView(impl->dxTexture2D.Get(), NULL, impl2->_renderTargetView.ReleaseAndGetAddressOf());

		if (FAILED(hr)) {
			Exception::Throw(Exception::FAILED_TO_CREATE);
		}

		impl2->_renderTargetView->GetDesc(&impl2->_renderTargetDesc);

		//depthStencilFormat = DepthFormat::None;
		multiSampleCount = impl->dxDescription.SampleDesc.Count;
		//targetUsage = RenderTargetUsage::DiscardContent;
	}

	void RenderTarget2D::Apply() {
		if (!m_device || !m_device->impl->_context) {
			Exception::Throw(Exception::FAILED_TO_APPLY);
		}

		if (!impl2->_renderTargetView)
		{
			Initialize();
		}

		auto& context = m_device->impl->_context;
		context->OMSetRenderTargets(1, impl2->_renderTargetView.GetAddressOf(), nullptr);		
	}
}