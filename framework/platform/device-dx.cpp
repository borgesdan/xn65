#include "device-dx.hpp"
#include "window-dx.hpp"
#include "swapchain-dx.hpp"
#include "rendertarget-dx.hpp"
#include "adapter-dx.hpp"

namespace xna {
    GraphicsDevice::GraphicsDevice() {
        _adapter = New<GraphicsAdapter>();            
        _blendState = BlendState::NonPremultiplied();  
        _adapter = GraphicsAdapter::DefaultAdapter();

        ip_GraphicsDevice = New<InternalProperty>();
    }

	bool GraphicsDevice::Initialize(GameWindow& gameWindow) {
		auto& p = ip_GraphicsDevice;
		p->_createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

		if (p->_device) {
			p->_device->Release();
			p->_device = nullptr;
		}

		if (p->_context) {
			p->_context->Release();
			p->_context = nullptr;
		}

        const auto bounds = gameWindow.ClientBounds();
        _viewport = xna::Viewport(0.0F, 0.0F, 
            static_cast<float>(bounds.Width), 
            static_cast<float>(bounds.Height),
            0.0F, 1.F);

        if FAILED(
            D3D11CreateDevice(
                _adapter->ip_GraphicsAdapter->_adapter,                           // adaptador de vídeo (NULL = adaptador padrão)
                D3D_DRIVER_TYPE_HARDWARE,       // tipo de driver D3D (Hardware, Reference ou Software)
                NULL,                           // ponteiro para rasterizador em software
                p->_createDeviceFlags,              // modo de depuração ou modo normal
                NULL,                           // featureLevels do Direct3D (NULL = maior suportada)
                0,                              // tamanho do vetor featureLevels
                D3D11_SDK_VERSION,              // versão do SDK do Direct3D
                &p->_device,                        // guarda o dispositivo D3D criado
                &p->_featureLevel,                  // versão do Direct3D utilizada
                &p->_context))                      // contexto do dispositivo D3D
        {
            // sistema não suporta dispositivo D3D11
            // fazendo a criação de um WARP Device que 
            // implementa um rasterizador em software
            if FAILED(D3D11CreateDevice(
                NULL, 
                D3D_DRIVER_TYPE_WARP,
                NULL, 
                p->_createDeviceFlags, 
                NULL, 
                0, 
                D3D11_SDK_VERSION,
                &p->_device, 
                &p->_featureLevel, 
                &p->_context))
                return false;

            OutputDebugString("---> Usando Adaptador WARP: não há suporte ao D3D11\n");
        }                       

        COLORREF color = gameWindow.ip_GameWindow->Color();
        p->_backgroundColor[0] = GetRValue(color) / 255.0f;
        p->_backgroundColor[1] = GetGValue(color) / 255.0f;
        p->_backgroundColor[2] = GetBValue(color) / 255.0f;
        p->_backgroundColor[3] = 1.0f;

        if (!_swapChain)
            _swapChain = New<xna::SwapChain>(this);

        _swapChain->Initialize(gameWindow);

        if (!_swapChain->Apply())
            return false;

        IDXGIFactory1* dxgiFactory = nullptr;
        if FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&dxgiFactory))
            return false;

        if FAILED(dxgiFactory->MakeWindowAssociation(gameWindow.ip_GameWindow->WindowHandle(), DXGI_MWA_NO_ALT_ENTER))
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

        p->_context->RSSetViewports(1, &view);

        _blendState->Apply(this);

        return true;
	}

    bool GraphicsDevice::Present() {
        _swapChain->ip_SwapChain->_swapChain->Present(false, NULL);
        auto& p = ip_GraphicsDevice;
        auto& pr = _renderTarget2D->ip_RenderTarget2D;

        p->_context->OMSetRenderTargets(1, &pr->_renderTargetView, nullptr);

        return true;
    }

    void GraphicsDevice::Clear() {
        auto& p = ip_GraphicsDevice;
        auto& pr = _renderTarget2D->ip_RenderTarget2D;

        p->_context->ClearRenderTargetView(pr->_renderTargetView, p->_backgroundColor);
    }
}