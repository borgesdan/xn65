#include "swapchain-dx.hpp"
#include "adapter-dx.hpp"
#include "device-dx.hpp"

namespace xna {
    static bool internalInit(GraphicsDevice& device, GameWindow const& gameWindow, IDXGISwapChain1*& swapChain, DXGI_SWAP_CHAIN_DESC1 const& desc, DXGI_SWAP_CHAIN_FULLSCREEN_DESC const& fdesc) {
        if (!device._device || !gameWindow.WindowHandle())
            return false;
        
        if (swapChain) {
            swapChain->Release();
            swapChain = nullptr;
        }

        auto adapter = device.Adapter();
        auto dxAdapter = adapter->_adapter;

        IDXGIFactory1* dxFactory1 = nullptr;
        auto hr = dxAdapter->GetParent(IID_IDXGIFactory1, (void**)&dxFactory1);

        if (FAILED(hr)) return false;

        IDXGIFactory2* dxFactory2 = nullptr;
        hr = dxFactory1->QueryInterface(IID_IDXGIFactory2, (void**)&dxFactory2);

        if (FAILED(hr)) return false;

        dxFactory2->CreateSwapChainForHwnd(
            device._device,
            gameWindow.WindowHandle(),
            &desc,
            &fdesc,
            nullptr,
            &swapChain);


        return true;
    }

    bool SwapChain::Initialize(GraphicsDevice& device, GameWindow const& gameWindow) {
        const auto bounds = gameWindow.ClientBounds();        

        _description.Width = static_cast<UINT>(bounds.Width);
        _description.Height = static_cast<UINT>(bounds.Height);
        _description.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        _description.SampleDesc.Count = 1;
        _description.SampleDesc.Quality = 0;
        _description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        _description.BufferCount = 2;
        _description.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        _description.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        _description.AlphaMode = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED;
        _fullScreenDescription.RefreshRate.Numerator = 60;        
        _fullScreenDescription.RefreshRate.Denominator = 1;        
        _fullScreenDescription.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        _fullScreenDescription.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        _fullScreenDescription.Windowed = gameWindow.Mode() != GameWindowMode::Fullscreen;

        return internalInit(device, gameWindow, _swapChain, _description, _fullScreenDescription);
    }

    bool SwapChain::Initialize(GraphicsDevice& device, GameWindow const& gameWindow, DXGI_SWAP_CHAIN_DESC1 const& desc, DXGI_SWAP_CHAIN_FULLSCREEN_DESC const& fullScreenDesc)
    {
        return internalInit(device, gameWindow, _swapChain, _description, _fullScreenDescription);
    }
}