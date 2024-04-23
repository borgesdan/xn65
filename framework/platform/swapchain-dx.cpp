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
        auto dxAdapter = adapter->dxadapter;

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

        dxDescription.Width = static_cast<UINT>(bounds.Width);
        dxDescription.Height = static_cast<UINT>(bounds.Height);
        dxDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        dxDescription.SampleDesc.Count = 1;
        dxDescription.SampleDesc.Quality = 0;
        dxDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        dxDescription.BufferCount = 2;
        dxDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        dxDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        dxDescription.AlphaMode = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED;
        dxFullScreenDescription.RefreshRate.Numerator = 60;        
        dxFullScreenDescription.RefreshRate.Denominator = 1;        
        dxFullScreenDescription.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        dxFullScreenDescription.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        dxFullScreenDescription.Windowed = gameWindow.Mode() != GameWindowMode::Fullscreen;

        return internalInit(device, gameWindow, dxSwapChain, dxDescription, dxFullScreenDescription);
    }

    bool SwapChain::Initialize(GraphicsDevice& device, GameWindow const& gameWindow, DXGI_SWAP_CHAIN_DESC1 const& desc, DXGI_SWAP_CHAIN_FULLSCREEN_DESC const& fullScreenDesc)
    {
        dxDescription = desc;
        dxFullScreenDescription = fullScreenDesc;
        return internalInit(device, gameWindow, dxSwapChain, dxDescription, dxFullScreenDescription);
    }

    bool SwapChain::GetBackBuffer(ID3D11Texture2D*& texture2D) {
        if (!dxSwapChain)
            return false;

        const auto hr = dxSwapChain->GetBuffer(0, __uuidof(texture2D), (void**)(&texture2D));

        return !FAILED(hr);
    }

    bool SwapChain::Present(bool vsync) {
        if (!dxSwapChain)
            return false;

        const auto hr = dxSwapChain->Present(vsync, NULL);
        return !FAILED(hr);
    }
}