#include "swapchain-dx.hpp"
#include "../graphics/device.hpp"
#include "adapter-dx.hpp"
#include "device-dx.hpp"

namespace xna {
	SwapChain::SwapChain(GraphicsDevice* device) :
	 _device(device) {		
		ip_SwapChain = New<InternalProperty>();        
	}	

    bool SwapChain::Initialize(GameWindow const& gameWindow) {
        auto& p = ip_SwapChain;

        const auto bounds = gameWindow.ClientBounds();        

        p->_swapDescription.BufferDesc.Width = static_cast<UINT>(bounds.Width);
        p->_swapDescription.BufferDesc.Height = static_cast<UINT>(bounds.Height);
        p->_swapDescription.BufferDesc.RefreshRate.Numerator = 60;
        p->_swapDescription.BufferDesc.RefreshRate.Denominator = 1;
        p->_swapDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        p->_swapDescription.SampleDesc.Count = 1;
        p->_swapDescription.SampleDesc.Quality = 0;
        p->_swapDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        p->_swapDescription.BufferCount = 2;
        p->_swapDescription.OutputWindow = gameWindow.ip_GameWindow->WindowHandle();
        p->_swapDescription.Windowed = gameWindow.ip_GameWindow->Mode() != GameWindowMode::Fullscreen;
        p->_swapDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        p->_swapDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        return true;
    }

	bool SwapChain::Apply() {
        auto& p = ip_SwapChain;
        
        auto adapter = _device->Adapter();
        auto dxAdapter = adapter->ip_GraphicsAdapter->_adapter;

        IDXGIFactory* dxFactory = nullptr;
        if FAILED(dxAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxFactory))
            return false;

        auto dxdevice = _device->ip_GraphicsDevice->_device;

        if FAILED(dxFactory->CreateSwapChain(dxdevice, &p->_swapDescription, &p->_swapChain))
            return false;

        dxFactory->Release();
        dxFactory = nullptr;

        return true;
	}
}