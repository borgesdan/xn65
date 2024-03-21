#include "device-dx.hpp"
#include "window-dx.hpp"
#include "swapchain-dx.hpp"
#include "rendertarget-dx.hpp"
#include "adapter-dx.hpp"
#include "blendstate-dx.hpp"

namespace xna {
    GraphicsDevice::GraphicsDevice() {        
        _blendState = BlendState::NonPremultiplied();  
        _adapter = GraphicsAdapter::DefaultAdapter();        
    }

	bool GraphicsDevice::Initialize(GameWindow& gameWindow) {		
	    _createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

		if (_device) {
			_device->Release();
			_device = nullptr;
		}

		if (_context) {
			_context->Release();
			_context = nullptr;
		}

        const auto bounds = gameWindow.ClientBounds();
        _viewport = xna::Viewport(0.0F, 0.0F, 
            static_cast<float>(bounds.Width), 
            static_cast<float>(bounds.Height),
            0.0F, 1.F);

        if (!createDevice())
            return false;

        COLORREF color = gameWindow.Color();
        _backgroundColor[0] = GetRValue(color) / 255.0f;
        _backgroundColor[1] = GetGValue(color) / 255.0f;
        _backgroundColor[2] = GetBValue(color) / 255.0f;
        _backgroundColor[3] = 1.0f;

        if (!_swapChain)
            _swapChain = New<xna::SwapChain>(this);

        _swapChain->Initialize(gameWindow);

        if (!_swapChain->Apply())
            return false;

        IDXGIFactory1* dxgiFactory = nullptr;
        if FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&dxgiFactory))
            return false;

        if FAILED(dxgiFactory->MakeWindowAssociation(gameWindow.WindowHandle(), DXGI_MWA_NO_ALT_ENTER))
            return false;

        if (!_renderTarget2D) {
            _renderTarget2D = New<RenderTarget2D>(this);
        }

        if (!_renderTarget2D->Apply())
            return false;

        D3D11_VIEWPORT view{};
        view.TopLeftX = _viewport.X;
        view.TopLeftY = _viewport.Y;
        view.Width = _viewport.Width;
        view.Height = _viewport.Height;
        view.MinDepth = _viewport.MinDetph;
        view.MaxDepth = _viewport.MaxDepth;

        _context->RSSetViewports(1, &view);

        _blendState->Apply(this);

        return true;
	}

    bool GraphicsDevice::Present() {
        _swapChain->_swapChain->Present(_usevsync, NULL);
        _context->OMSetRenderTargets(1, &_renderTarget2D->_renderTargetView, nullptr);

        return true;
    }

    bool GraphicsDevice::GetSwapChainBackBuffer(ID3D11Texture2D*& texture2D) {
        if FAILED(_swapChain->_swapChain->GetBuffer(0, __uuidof(texture2D), (void**)(&texture2D)))
            return false;

        return true;
    }

    bool GraphicsDevice::createDevice() {
#if _DEBUG
        _createDeviceFlags |= D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
#endif

        if FAILED(
            D3D11CreateDevice(
                _adapter->_adapter,
                D3D_DRIVER_TYPE_UNKNOWN,
                NULL,
                _createDeviceFlags,
                NULL,
                0,
                D3D11_SDK_VERSION,
                &_device,
                &_featureLevel,
                &_context)) {
            
            if FAILED(D3D11CreateDevice(
                NULL,
                D3D_DRIVER_TYPE_WARP,
                NULL,
                _createDeviceFlags,
                NULL,
                0,
                D3D11_SDK_VERSION,
                &_device,
                &_featureLevel,
                &_context))
                return false;

            OutputDebugString("---> Usando Adaptador WARP: não há suporte ao D3D11\n");
        }

        return true;
    }

    void GraphicsDevice::Clear() {        
        _context->ClearRenderTargetView(_renderTarget2D->_renderTargetView, _backgroundColor);
    }
}