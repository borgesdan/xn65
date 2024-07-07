#include "xna/platform/dx.hpp"
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
	}

	static void createDevice(GraphicsDevice::PlatformImplementation& impl) {
		auto createDeviceFlags = 0;
#if _DEBUG
		createDeviceFlags = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
#endif        
		auto hr = D3D11CreateDevice(
			impl._adapter->impl->dxadapter.Get(),
			D3D_DRIVER_TYPE_UNKNOWN,
			NULL,
			createDeviceFlags,
			NULL,
			0,
			D3D11_SDK_VERSION,
			impl._device.GetAddressOf(),
			&impl._featureLevel,
			impl._context.GetAddressOf());

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
				impl._device.GetAddressOf(),
				&impl._featureLevel,
				impl._context.GetAddressOf());

			if FAILED(hr)
				Exception::Throw(Exception::FAILED_TO_CREATE);
		}
	}

	void initAndApplyState(GraphicsDevice::PlatformImplementation& impl, PGraphicsDevice const& device) {
		impl._blendState->Bind(device);
		impl._blendState->Initialize();
		impl._blendState->Apply();

		impl._rasterizerState->Bind(device);
		impl._rasterizerState->Initialize();
		impl._rasterizerState->Apply();

		impl._depthStencilState->Bind(device);
		impl._depthStencilState->Initialize();
		impl._depthStencilState->Apply();

		impl._samplerStates->Apply(*device);
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

	bool GraphicsDevice::Initialize() {
		auto _this = shared_from_this();

		if (!impl)
			impl = uptr<PlatformImplementation>();

		reset(*impl);
		
		createDevice(*impl);

		auto hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&impl->_factory);
		
		if FAILED(hr)
			Exception::Throw(Exception::FAILED_TO_CREATE);

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

		impl->_swapChain = snew<xna::SwapChain>(_this);
		impl->_swapChain->Initialize();

		hr = impl->_factory->MakeWindowAssociation(impl->_gameWindow->impl->WindowHandle(), DXGI_MWA_NO_ALT_ENTER);
		
		if (FAILED(hr)) 
			Exception::Throw(Exception::FAILED_TO_MAKE_WINDOW_ASSOCIATION);

		impl->_renderTarget2D = snew<RenderTarget2D>(_this);
		
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

		initAndApplyState(*impl, _this);

		return true;
	}

	bool GraphicsDevice::Present() {
		if (!impl) return false;

		const auto result = impl->_swapChain->Present(impl->_usevsync);
		impl->_context->OMSetRenderTargets(
			1, 
			impl->_renderTarget2D->render_impl->_renderTargetView.GetAddressOf(), 
			nullptr);

		return result;
	}		

	void GraphicsDevice::Clear(Color const& color) {
		if (!impl) return;

		const auto v4 = color.ToVector4();

		impl->_backgroundColor[0] = v4.X;
		impl->_backgroundColor[1] = v4.Y;
		impl->_backgroundColor[2] = v4.Z;
		impl->_backgroundColor[3] = v4.W;
		
		impl->_context->ClearRenderTargetView(
			impl->_renderTarget2D->render_impl->_renderTargetView.Get(),
			impl->_backgroundColor);
	}

	void GraphicsDevice::Clear(ClearOptions options, Color const& color, float depth, Int stencil) {
		if (!impl) return;

		switch (options)
		{
		case xna::ClearOptions::DepthBuffer:
			Exception::Throw(Exception::NOT_IMPLEMENTED);
			break;
		case xna::ClearOptions::Stencil:
			Exception::Throw(Exception::NOT_IMPLEMENTED);
			break;
		case xna::ClearOptions::Target:
			Clear(color);
			break;
		default:
			return;
		}
	}

	void GraphicsDevice::Clear(ClearOptions options, Vector4 const& color, float depth, Int stencil) {
		if (!impl) return;


	}

	sptr<GraphicsAdapter> GraphicsDevice::Adapter() const {
		if (!impl) return nullptr;

		return impl->_adapter;
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

	
	sptr<xna::BlendState> GraphicsDevice::BlendState() const {
		return impl->_blendState;
	}
	
	void GraphicsDevice::BlendState(sptr<xna::BlendState> const& value) {
		impl->_blendState = value;
	}
	
	sptr<xna::DepthStencilState> GraphicsDevice::DepthStencilState() const {
		return impl->_depthStencilState;
	}
	
	void GraphicsDevice::DepthStencilState(sptr<xna::DepthStencilState> const& value) {
		impl->_depthStencilState = value;
	}
	
	sptr<xna::RasterizerState> GraphicsDevice::RasterizerState() const {
		return impl->_rasterizerState;
	}
	
	void GraphicsDevice::RasterizerState(sptr<xna::RasterizerState> const& value) {
		impl->_rasterizerState = value;
	}

	sptr<SamplerStateCollection> GraphicsDevice::SamplerStates() const {
		return impl->_samplerStates;
	}

	Int GraphicsDevice::MultiSampleMask() const {
		return impl->_multiSampleMask;
	}

	void GraphicsDevice::MultiSampleMask(Int value) {
		impl->_multiSampleMask = value;
	}
}