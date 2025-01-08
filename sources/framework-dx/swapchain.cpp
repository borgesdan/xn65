#include "xna/graphics/adapter.hpp"
#include "xna/graphics/swapchain.hpp"
#include "xna-dx/framework.hpp"
#include "xna/graphics/device.hpp"

namespace xna {
    SwapChain::SwapChain() : GraphicsResource(nullptr) {
        impl = unew<PlatformImplementation>();        
    }

    SwapChain::SwapChain(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
        impl = unew<PlatformImplementation>();
    }

    SwapChain::~SwapChain() {
        impl = nullptr;
    }

    static bool internalInit(GraphicsDevice& device, HWND windowHandle, comptr<IDXGISwapChain1>& swapChain, DXGI_SWAP_CHAIN_DESC1 const& desc, DXGI_SWAP_CHAIN_FULLSCREEN_DESC const& fdesc) {
        if (!device.Implementation->Device || !windowHandle)
            return false;
        
        if (swapChain) {
            swapChain.ReleaseAndGetAddressOf();
        }

        auto adapter = device.Adapter();        

        comptr<IDXGIFactory2> dxFactory2 = nullptr;
        const auto hr = adapter->Implementation->Factory->QueryInterface(IID_IDXGIFactory2, (void**)&dxFactory2);

        if (FAILED(hr)) 
            return false;

        dxFactory2->CreateSwapChainForHwnd(
            device.Implementation->Device.Get(),
            windowHandle,
            &desc,
            &fdesc,
            nullptr,
            swapChain.GetAddressOf());

        return true;
    }

    bool SwapChain::Initialize() {
        if (!impl || !BaseGraphicsDevice || !BaseGraphicsDevice->Implementation->Device) {
            throw csharp::InvalidOperationException();
        }
        
        const auto parameters = BaseGraphicsDevice->PresentParameters();

        impl->dxDescription.Width = static_cast<UINT>(parameters->BackBufferWidth);
        impl->dxDescription.Height = static_cast<UINT>(parameters->BackBufferHeight);
        impl->dxDescription.Format = DxHelpers::SurfaceFormatToDx(parameters->BackBufferFormat);
        impl->dxDescription.SampleDesc.Count = 1;
        impl->dxDescription.SampleDesc.Quality = 0;
        impl->dxDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        impl->dxDescription.BufferCount = 2;
        impl->dxDescription.SwapEffect = static_cast<DXGI_SWAP_EFFECT>(parameters->PresentationSwapEffect);
        impl->dxDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        impl->dxDescription.AlphaMode = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED;
        impl->dxFullScreenDescription.RefreshRate.Numerator = 60;        
        impl->dxFullScreenDescription.RefreshRate.Denominator = 1;        
        impl->dxFullScreenDescription.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        impl->dxFullScreenDescription.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        impl->dxFullScreenDescription.Windowed = !parameters->IsFullscreen;

        HWND hwnd = reinterpret_cast<HWND>(parameters->DeviceWindowHandle);
        return internalInit(*BaseGraphicsDevice, hwnd, impl->dxSwapChain, impl->dxDescription, impl->dxFullScreenDescription);
    }   

    bool SwapChain::GetBackBuffer(Texture2D& texture2D) {
        if (!impl || !impl->dxSwapChain)
            return false;

        const auto hr = impl->dxSwapChain->GetBuffer(0, IID_ID3D11Texture2D, (void**)(&texture2D.Implementation->Texture2D));

        return !FAILED(hr);
    }

    bool SwapChain::Present(bool vsync) const {
        if (!impl || !impl->dxSwapChain)
            return false;

        const auto presentValue = static_cast<UINT>(vsync);

        const auto hr = impl->dxSwapChain->Present(presentValue, NULL);
        return !FAILED(hr);
    }
}