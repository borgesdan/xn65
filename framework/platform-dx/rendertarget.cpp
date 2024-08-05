#include "xna/xna-dx.hpp"

namespace xna {
	RenderTarget2D::RenderTarget2D() : Texture2D(nullptr) {
		impl2 = unew<PlatformImplementation>();
	}

	RenderTarget2D::RenderTarget2D(sptr<GraphicsDevice> const& device) : Texture2D(device) {
		impl2 = unew<PlatformImplementation>();
	}	

	P_RenderTarget2D RenderTarget2D::FromBackBuffer(P_GraphicsDevice const& device) {
		auto& swapChain = device->impl->_swapChain;
		auto rt = snew<RenderTarget2D>(device);
		auto& implementation = rt->impl;
		auto& implementation2 = rt->impl2;

		if (!swapChain->impl->GetBackBuffer(implementation->dxTexture2D))
		{
			Exception::Throw(Exception::FAILED_TO_CREATE);
		}

		rt->Initialize();		

		return rt;
	}

	void RenderTarget2D::Initialize() {
		if (!impl || !m_device || !m_device->impl->_device) {
			Exception::Throw(Exception::UNABLE_TO_INITIALIZE);
		}				

		if (impl2->_renderTargetView)
			return;

		impl->dxDescription.Width = width;
		impl->dxDescription.Height = height;
		impl->dxDescription.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;

		Texture2D::Initialize();
		
		auto& dxdevice = m_device->impl->_device;
		
		const auto hr = dxdevice->CreateRenderTargetView(
			impl->dxTexture2D.Get(),
			NULL, 
			impl2->_renderTargetView.ReleaseAndGetAddressOf());

		if (FAILED(hr)) {
			Exception::Throw(Exception::FAILED_TO_CREATE);
		}

		impl2->_renderTargetView->GetDesc(&impl2->_renderTargetDesc);
	}
}