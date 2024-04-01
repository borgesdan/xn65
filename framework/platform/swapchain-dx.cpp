#include "swapchain-dx.hpp"
#include "../graphics/device.hpp"
#include "adapter-dx.hpp"
#include "device-dx.hpp"

namespace xna {
    SwapChain::SwapChain(GraphicsDevice* device){
        _device = device;
	}	

    bool SwapChain::Initialize(GameWindow const& gameWindow) {
        const auto bounds = gameWindow.ClientBounds();        

        _swapDescription.BufferDesc.Width = static_cast<UINT>(bounds.Width);
        _swapDescription.BufferDesc.Height = static_cast<UINT>(bounds.Height);
        _swapDescription.BufferDesc.RefreshRate.Numerator = 60;
        _swapDescription.BufferDesc.RefreshRate.Denominator = 1;
        _swapDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        _swapDescription.SampleDesc.Count = 1;
        _swapDescription.SampleDesc.Quality = 0;
        _swapDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        _swapDescription.BufferCount = 2;
        _swapDescription.OutputWindow = gameWindow.WindowHandle();
        _swapDescription.Windowed = gameWindow.Mode() != GameWindowMode::Fullscreen;
        _swapDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        _swapDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        return true;
    }

	bool SwapChain::Apply() {        
        auto adapter = _device->Adapter();
        auto dxAdapter = adapter->_adapter;

        IDXGIFactory* dxFactory = nullptr;
        if FAILED(dxAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxFactory))
            return false;

        auto dxdevice = _device->_device;

        if FAILED(dxFactory->CreateSwapChain(dxdevice, &_swapDescription, &_swapChain))
        {
            dxFactory->Release();
            dxFactory = nullptr;
            return false;
        }

        dxFactory->Release();
        dxFactory = nullptr;

        return true;
	}
}