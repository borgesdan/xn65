#include "xna-dx/framework.hpp"

namespace xna {
	RenderTarget2D::RenderTarget2D() : Texture2D(nullptr) {
		Implementation2 = unew<RenderTarget2DImplementation>();
	}

	RenderTarget2D::RenderTarget2D(sptr<GraphicsDevice> const& device) : Texture2D(device) {
		Implementation2 = unew<RenderTarget2DImplementation>();
	}	

	P_RenderTarget2D RenderTarget2D::FromBackBuffer(P_GraphicsDevice const& device) {
		auto& swapChain = device->Implementation->SwapChain;
		auto rt = snew<RenderTarget2D>(device);
		auto& implementation = rt->Implementation;
		auto& implementation2 = rt->Implementation2;

		if (!swapChain->impl->GetBackBuffer(implementation->Texture2D))
		{
			Exception::Throw(Exception::FAILED_TO_CREATE);
		}

		rt->Initialize();		

		return rt;
	}

	void RenderTarget2D::Initialize() {
		if (!Implementation || !BaseGraphicsDevice || !BaseGraphicsDevice->Implementation->Device) {
			Exception::Throw(Exception::UNABLE_TO_INITIALIZE);
		}				

		if (Implementation2->RenderTargetView)
			return;

		Implementation->Description.Width = width;
		Implementation->Description.Height = height;
		Implementation->Description.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET;

		Texture2D::Initialize();
		
		auto& dxdevice = BaseGraphicsDevice->Implementation->Device;
		
		const auto hr = dxdevice->CreateRenderTargetView(
			Implementation->Texture2D.Get(),
			NULL, 
			Implementation2->RenderTargetView.ReleaseAndGetAddressOf());

		if (FAILED(hr)) {
			Exception::Throw(Exception::FAILED_TO_CREATE);
		}

		Implementation2->RenderTargetView->GetDesc(&Implementation2->Description);
	}
}