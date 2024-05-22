#include "platform-dx/swapchain-dx.hpp"
#include "platform-dx/device-dx.hpp"
#include "platform-dx/dxhelpers.hpp"
#include "graphics/adapter.hpp"
#include "platform-dx/implementations.hpp"

namespace xna {
    static bool internalInit(GraphicsDevice& device, HWND windowHandle, IDXGISwapChain1*& swapChain, DXGI_SWAP_CHAIN_DESC1 const& desc, DXGI_SWAP_CHAIN_FULLSCREEN_DESC const& fdesc) {
        if (!device._device || !windowHandle)
            return false;
        
        if (swapChain) {
            swapChain->Release();
            swapChain = nullptr;
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
            device._device,
            windowHandle,
            &desc,
            &fdesc,
            nullptr,
            &swapChain);


        return true;
    }

    bool SwapChain::Initialize(xna_error_ptr_arg) {
        if (!m_device || !m_device->_device) {
            xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
            return false;
        }
        
        const auto parameters = m_device->_presentationParameters;

        dxDescription.Width = static_cast<UINT>(parameters.BackBufferWidth);
        dxDescription.Height = static_cast<UINT>(parameters.BackBufferHeight);
        dxDescription.Format = DxHelpers::ConvertSurfaceToDXGIFORMAT(parameters.BackBufferFormat);
        dxDescription.SampleDesc.Count = 1;
        dxDescription.SampleDesc.Quality = 0;
        dxDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        dxDescription.BufferCount = 2;
        dxDescription.SwapEffect = static_cast<DXGI_SWAP_EFFECT>(parameters.PresentationSwapEffect);
        dxDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        dxDescription.AlphaMode = DXGI_ALPHA_MODE::DXGI_ALPHA_MODE_UNSPECIFIED;
        dxFullScreenDescription.RefreshRate.Numerator = 60;        
        dxFullScreenDescription.RefreshRate.Denominator = 1;        
        dxFullScreenDescription.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        dxFullScreenDescription.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        dxFullScreenDescription.Windowed = !parameters.Fullscreen;

        HWND hwnd = reinterpret_cast<HWND>(parameters.DeviceWindowHandle);
        return internalInit(*m_device, hwnd, dxSwapChain, dxDescription, dxFullScreenDescription);
    }

    bool SwapChain::Initialize(GameWindow const& gameWindow, DXGI_SWAP_CHAIN_DESC1 const& desc, DXGI_SWAP_CHAIN_FULLSCREEN_DESC const& fullScreenDesc, xna_error_ptr_arg)
    {
        if (!m_device || !m_device->_device) {
            xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
            return false;
        }

        dxDescription = desc;
        dxFullScreenDescription = fullScreenDesc;
        return internalInit(*m_device, gameWindow.WindowHandle(), dxSwapChain, dxDescription, dxFullScreenDescription);
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