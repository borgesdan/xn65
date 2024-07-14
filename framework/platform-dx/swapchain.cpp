#include "xna/graphics/adapter.hpp"
#include "xna/graphics/swapchain.hpp"
#include "xna/xna-dx.hpp"
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
        if (!device.impl->_device || !windowHandle)
            return false;
        
        if (swapChain) {
            swapChain.ReleaseAndGetAddressOf();
        }

        auto adapter = device.Adapter();
        auto dxAdapter = adapter->impl->dxadapter;

        IDXGIFactory1* dxFactory1 = nullptr;
        auto hr = dxAdapter->GetParent(IID_IDXGIFactory1, (void**)&dxFactory1);

        if (FAILED(hr)) return false;

        IDXGIFactory2* dxFactory2 = nullptr;
        hr = dxFactory1->QueryInterface(IID_IDXGIFactory2, (void**)&dxFactory2);

        if (FAILED(hr)) return false;

        dxFactory2->CreateSwapChainForHwnd(
            device.impl->_device.Get(),
            windowHandle,
            &desc,
            &fdesc,
            nullptr,
            swapChain.GetAddressOf());


        return true;
    }

    bool SwapChain::Initialize() {
        if (!impl || !m_device || !m_device->impl->_device) {
            Exception::Throw(Exception::UNABLE_TO_INITIALIZE);
        }
        
        const auto parameters = m_device->impl->_presentationParameters;

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
        impl->dxFullScreenDescription.Windowed = !parameters->Fullscreen;

        HWND hwnd = reinterpret_cast<HWND>(parameters->DeviceWindowHandle);
        return internalInit(*m_device, hwnd, impl->dxSwapChain, impl->dxDescription, impl->dxFullScreenDescription);
    }   

    bool SwapChain::GetBackBuffer(Texture2D& texture2D) {
        if (!impl || !impl->dxSwapChain)
            return false;

        const auto hr = impl->dxSwapChain->GetBuffer(0, IID_ID3D11Texture2D, (void**)(&texture2D.impl->dxTexture2D));

        return !FAILED(hr);
    }

    bool SwapChain::Present(bool vsync) {
        if (!impl || !impl->dxSwapChain)
            return false;

        const auto hr = impl->dxSwapChain->Present(vsync, NULL);
        return !FAILED(hr);
    }
}