#include "device-dx.hpp"
#include "window-dx.hpp"
#include "swapchain-dx.hpp"
#include "rendertarget-dx.hpp"
#include "adapter-dx.hpp"
#include "blendstate-dx.hpp"
#include "gdeviceinfo-dx.hpp"
#include "../common/color.hpp"
#include "gdevicemanager-dx.hpp"

namespace xna {
	GraphicsDevice::GraphicsDevice() {		
		_adapter = GraphicsAdapter::DefaultAdapter();
		_adapter->CurrentDisplayMode(SurfaceFormat::Color, GraphicsDeviceManager::DefaultBackBufferWidth, GraphicsDeviceManager::DefaultBackBufferHeight);
	}

	GraphicsDevice::GraphicsDevice(GraphicsDeviceInformation const& info) {
		_adapter = info.Adapter();
		_presentationParameters = info.PresentationParameters();
		_adapter->CurrentDisplayMode(_presentationParameters.backBufferFormat, _presentationParameters.backBufferWidth, _presentationParameters.backBufferHeight);
	}

	bool GraphicsDevice::Initialize(GameWindow& gameWindow) {
		reset();
		
		if (!createDevice()) return false;

		auto hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&_factory);
		if (FAILED(hr)) return false;		

		const auto bounds = gameWindow.ClientBounds();

		_viewport = xna::Viewport(0.0F, 0.0F,
			static_cast<float>(bounds.Width),
			static_cast<float>(bounds.Height),
			0.0F, 1.F);

		COLORREF color = gameWindow.Color();
		_backgroundColor[0] = GetRValue(color) / 255.0f;
		_backgroundColor[1] = GetGValue(color) / 255.0f;
		_backgroundColor[2] = GetBValue(color) / 255.0f;
		_backgroundColor[3] = 1.0f;

		_swapChain = New<xna::SwapChain>(this);
		_swapChain->Initialize();

		hr = _factory->MakeWindowAssociation(gameWindow.WindowHandle(), DXGI_MWA_NO_ALT_ENTER);
		if (FAILED(hr)) return false;

		_renderTarget2D = New<RenderTarget2D>();
		if (!_renderTarget2D->Initialize(*this)) return false;

		_renderTarget2D->Apply(*this);

		D3D11_VIEWPORT view{};
		view.TopLeftX = _viewport.X;
		view.TopLeftY = _viewport.Y;
		view.Width = _viewport.Width;
		view.Height = _viewport.Height;
		view.MinDepth = _viewport.MinDetph;
		view.MaxDepth = _viewport.MaxDepth;

		_context->RSSetViewports(1, &view);

		_blendState = BlendState::NonPremultiplied();
		_blendState->Bind(this);
		_blendState->Apply();

		return true;
	}

	bool GraphicsDevice::Present() {
		const auto result = _swapChain->Present(_usevsync);
		_context->OMSetRenderTargets(1, &_renderTarget2D->_renderTargetView, nullptr);

		return result;
	}

	bool GraphicsDevice::createDevice() {
#if _DEBUG
		_createDeviceFlags |= D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
#endif        

		if FAILED(
			D3D11CreateDevice(
				_adapter->dxadapter,
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

	void GraphicsDevice::reset()
	{
		if (_device) {
			_device->Release();
			_device = nullptr;
		}

		if (_context) {
			_context->Release();
			_context = nullptr;
		}

		if (_factory) {
			_factory->Release();
			_factory = nullptr;
		}

		_blendState = nullptr;
		_swapChain = nullptr;
		_renderTarget2D = nullptr;		
	}

	void GraphicsDevice::Clear() {
		_context->ClearRenderTargetView(_renderTarget2D->_renderTargetView, _backgroundColor);
	}

	void GraphicsDevice::Clear(Color const& color) {
		const auto v4 = color.ToVector4();

		_backgroundColor[0] = v4.X;
		_backgroundColor[1] = v4.Y;
		_backgroundColor[2] = v4.Z;
		_backgroundColor[3] = v4.W;

		_context->ClearRenderTargetView(_renderTarget2D->_renderTargetView, _backgroundColor);
	}
}