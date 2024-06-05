#include "xna/platform-dx/implementations.hpp"
#include "xna/game/gdevicemanager.hpp"

namespace xna {
	static void reset(GraphicsDevice::PlatformImplementation& impl)
	{
		if (impl._device) {
			impl._device->Release();
			impl._device = nullptr;
		}

		if (impl._context) {
			impl._context->Release();
			impl._context = nullptr;
		}

		if (impl._factory) {
			impl._factory->Release();
			impl._factory = nullptr;
		}

		impl._blendState = nullptr;
		impl._swapChain = nullptr;
		impl._renderTarget2D = nullptr;
	}

	static bool createDevice(GraphicsDevice::PlatformImplementation& impl) {
		auto createDeviceFlags = 0;
#if _DEBUG
		createDeviceFlags = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
#endif        
		

		auto hr = D3D11CreateDevice(
			impl._adapter->impl->dxadapter,
			D3D_DRIVER_TYPE_UNKNOWN,
			NULL,
			createDeviceFlags,
			NULL,
			0,
			D3D11_SDK_VERSION,
			&impl._device,
			&impl._featureLevel,
			&impl._context);

		if (FAILED(hr)) {
			OutputDebugString("---> Usando Adaptador WARP: não há suporte ao D3D11\n");

			hr = D3D11CreateDevice(
				NULL,
				D3D_DRIVER_TYPE_WARP,
				NULL,
				createDeviceFlags,
				NULL,
				0,
				D3D11_SDK_VERSION,
				&impl._device,
				&impl._featureLevel,
				&impl._context);						
		}
		
		return SUCCEEDED(hr);
	}

	GraphicsDevice::GraphicsDevice() {		
		impl = unew<PlatformImplementation>();
		impl->_adapter = GraphicsAdapter::DefaultAdapter();
		impl->_adapter->CurrentDisplayMode(
			SurfaceFormat::Color, 
			GraphicsDeviceManager::DefaultBackBufferWidth, 
			GraphicsDeviceManager::DefaultBackBufferHeight);
	}

	GraphicsDevice::GraphicsDevice(GraphicsDeviceInformation const& info) {
		impl = unew<PlatformImplementation>();
		
		impl->_adapter = info.Adapter;
		impl->_gameWindow = info.Window;
		impl->_presentationParameters = info.Parameters;
		impl->_adapter->CurrentDisplayMode(
			impl->_presentationParameters->BackBufferFormat, 
			impl->_presentationParameters->BackBufferWidth,
			impl->_presentationParameters->BackBufferHeight);
	}

	GraphicsDevice::~GraphicsDevice() {
		impl = nullptr;
	}

	bool GraphicsDevice::Initialize() {
		if (!impl)
			impl = uptr<PlatformImplementation>();

		reset(*impl);

		auto _this = shared_from_this();
		
		if (!createDevice(*impl))
			return false;

		auto hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&impl->_factory);
		if (FAILED(hr)) 
			return false;		

		const auto bounds = impl->_gameWindow->ClientBounds();

		impl->_viewport = xna::Viewport(0.0F, 0.0F,
			static_cast<float>(bounds.Width),
			static_cast<float>(bounds.Height),
			0.0F, 1.F);

		COLORREF color = impl->_gameWindow->impl->Color();
		impl->_backgroundColor[0] = GetRValue(color) / 255.0f;
		impl->_backgroundColor[1] = GetGValue(color) / 255.0f;
		impl->_backgroundColor[2] = GetBValue(color) / 255.0f;
		impl->_backgroundColor[3] = 1.0f;

		impl->_swapChain = New<xna::SwapChain>(_this);
		impl->_swapChain->Initialize();

		hr = impl->_factory->MakeWindowAssociation(impl->_gameWindow->impl->WindowHandle(), DXGI_MWA_NO_ALT_ENTER);
		if (FAILED(hr)) return false;

		impl->_renderTarget2D = New<RenderTarget2D>(_this);
		if (!impl->_renderTarget2D->Initialize())
			return false;

		impl->_renderTarget2D->Apply();

		D3D11_VIEWPORT view{};
		view.TopLeftX = impl->_viewport.X;
		view.TopLeftY = impl->_viewport.Y;
		view.Width = impl->_viewport.Width;
		view.Height = impl->_viewport.Height;
		view.MinDepth = impl->_viewport.MinDetph;
		view.MaxDepth = impl->_viewport.MaxDepth;

		impl->_context->RSSetViewports(1, &view);

		impl->_blendState = BlendState::NonPremultiplied();
		impl->_blendState->Bind(_this);
		impl->_blendState->Apply();

		return true;
	}

	bool GraphicsDevice::Present() {
		if (!impl) return false;

		const auto result = impl->_swapChain->Present(impl->_usevsync);
		impl->_context->OMSetRenderTargets(1, &impl->_renderTarget2D->render_impl->_renderTargetView, nullptr);

		return result;
	}	

	void GraphicsDevice::Clear() {
		if (!impl) return;

		impl->_context->ClearRenderTargetView(impl->_renderTarget2D->render_impl->_renderTargetView, impl->_backgroundColor);
	}

	void GraphicsDevice::Clear(Color const& color) {
		if (!impl) return;

		const auto v4 = color.ToVector4();

		impl->_backgroundColor[0] = v4.X;
		impl->_backgroundColor[1] = v4.Y;
		impl->_backgroundColor[2] = v4.Z;
		impl->_backgroundColor[3] = v4.W;

		impl->_context->ClearRenderTargetView(
			impl->_renderTarget2D->render_impl->_renderTargetView, 
			impl->_backgroundColor);
	}

	sptr<GraphicsAdapter> GraphicsDevice::Adapter() const {
		if (!impl) return nullptr;

		return impl->_adapter;
	}

	void GraphicsDevice::Adapter(sptr<GraphicsAdapter> const& adapter) {
		if (!impl) return;

		impl->_adapter = adapter;
	}

	xna::Viewport GraphicsDevice::Viewport() const {
		if (!impl) return {};

		return impl->_viewport;
	}

	void GraphicsDevice::Viewport(xna::Viewport const& viewport) {
		if (!impl) return;

		impl->_viewport = viewport;
	}

	void GraphicsDevice::UseVSync(bool use) {
		if (!impl) return;

		impl->_usevsync = use;
	}	
}