#include "xna/xna-dx.hpp"

namespace xna {
	static void reset(GraphicsDevice::PlatformImplementation& impl);
	static void createDevice(GraphicsDevice::PlatformImplementation& impl, GraphicsAdapter& currentAdapter);
	static void initAndApplyState(P_BlendState& blendState, P_RasterizerState& rasterizerState,
		P_DepthStencilState& depthStencilState, P_SamplerStateCollection& samplerStates, P_GraphicsDevice const& device);

	GraphicsDevice::GraphicsDevice() {		
		impl = unew<PlatformImplementation>();
		adapter = GraphicsAdapter::DefaultAdapter();		
	}
	
	GraphicsDevice::GraphicsDevice(sptr<GraphicsAdapter> const& adapter, GraphicsProfile const& graphicsProfile, sptr<PresentationParameters> const& presentationParameters) 
		: adapter(adapter), graphicsProfile(graphicsProfile), presentationParameters(presentationParameters) {
		impl = unew<PlatformImplementation>();
		
		blendState = xna::BlendState::Opaque();
		depthStencilState = xna::DepthStencilState::Default();
		rasterizerState = xna::RasterizerState::CullCounterClockwise();
		samplerStateCollection = snew<SamplerStateCollection>();
	}

	void GraphicsDevice::Initialize() {
		reset(*impl);
		
		auto _this = shared_from_this();				
			
		createDevice(*impl, *adapter);	
		
		//
		// Background
		// 

		const auto backColor = Colors::CornflowerBlue;
		const auto backColorV3 = backColor.ToVector3();

		impl->_backgroundColor[0] = backColorV3.X;
		impl->_backgroundColor[1] = backColorV3.Y;
		impl->_backgroundColor[2] = backColorV3.Z;
		impl->_backgroundColor[3] = 1.0f;		

		//
		// Window Association
		//

		auto hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&impl->_factory);

		if FAILED(hr)
			Exception::Throw(Exception::FAILED_TO_CREATE);

		auto hwnd = reinterpret_cast<HWND>(presentationParameters->DeviceWindowHandle);
		hr = impl->_factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
		
		if (FAILED(hr)) 
			Exception::Throw(Exception::FAILED_TO_MAKE_WINDOW_ASSOCIATION);		

		//
		// Viewport
		//

		viewport = xna::Viewport(0.0F, 0.0F,
			presentationParameters->BackBufferWidth,
			presentationParameters->BackBufferHeight,
			0.0F, 1.F);

		D3D11_VIEWPORT view = DxHelpers::ViewportToDx(viewport);
		impl->_context->RSSetViewports(1, &view);

		//
		// States
		//

		initAndApplyState(blendState, rasterizerState, depthStencilState, samplerStateCollection, _this);

		//
		// Presentation
		//

		const auto currentPresenInterval = presentationParameters->PresentationInterval;		

		switch (currentPresenInterval)
		{
		case PresentInterval::Default:
		case PresentInterval::One:
		case PresentInterval::Two:
			impl->vSyncValue = 1;
			break;
		case PresentInterval::Immediate:
			impl->vSyncValue = 0;
			break;
		default:
			impl->vSyncValue = 1;
			break;
		}		

		impl->_swapChain = snew<xna::SwapChain>(_this);
		impl->_swapChain->Initialize();

		//
		//Render Target
		//

		impl->_renderTarget2D = RenderTarget2D::FromBackBuffer(_this);
		const auto& renderView = impl->_renderTarget2D->impl2->_renderTargetView;
		impl->_context->OMSetRenderTargets(1, renderView.GetAddressOf(), nullptr);
	}

	bool GraphicsDevice::Present() const {
		const auto currentPresenInterval = presentationParameters->PresentationInterval;
		bool result = impl->_swapChain->Present(impl->vSyncValue != 0);		
		
		impl->_context->OMSetRenderTargets(
			1, 
			impl->_renderTarget2D->impl2->_renderTargetView.GetAddressOf(), 
			nullptr);

		return result;
	}		

	void GraphicsDevice::Clear(Color const& color) const {
		if (!impl) return;

		const auto v4 = color.ToVector4();

		impl->_backgroundColor[0] = v4.X;
		impl->_backgroundColor[1] = v4.Y;
		impl->_backgroundColor[2] = v4.Z;
		impl->_backgroundColor[3] = v4.W;
		
		impl->_context->ClearRenderTargetView(
			impl->_renderTarget2D->impl2->_renderTargetView.Get(),
			impl->_backgroundColor);
	}

	void GraphicsDevice::Clear(ClearOptions options, Color const& color, float depth, Int stencil) const {
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

	void GraphicsDevice::Viewport(xna::Viewport const& value) {
		viewport = value;
		const auto view = DxHelpers::ViewportToDx(viewport);

		impl->_context->RSSetViewports(1, &view);
	}	
	
	void GraphicsDevice::BlendState(sptr<xna::BlendState> const& value) {
		blendState = value;
		blendState->Apply();
	}
	
	void GraphicsDevice::DepthStencilState(sptr<xna::DepthStencilState> const& value) {
		depthStencilState = value;
		depthStencilState->Apply();
	}
	
	void GraphicsDevice::RasterizerState(sptr<xna::RasterizerState> const& value) {
		rasterizerState = value;
		rasterizerState->Apply();
	}		

	void GraphicsDevice::Reset(sptr<PresentationParameters> const& parameters, sptr<GraphicsAdapter> const& graphicsAdapter){
		impl = unew<PlatformImplementation>();
		adapter = graphicsAdapter;
		presentationParameters = parameters;
		
		Initialize();
	}		

	//
	// INTERNAL
	//

	void reset(GraphicsDevice::PlatformImplementation& impl)
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

	void createDevice(GraphicsDevice::PlatformImplementation& impl, GraphicsAdapter& currentAdapter) {
		//
		// See ref
		//
		// D3D_DRIVER_TYPE
		// https://learn.microsoft.com/en-us/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_driver_type
		//
		// D3D11CreateDevice function 
		// https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-d3d11createdevice
		//		

		auto createDeviceFlags = 0;
#if _DEBUG
		createDeviceFlags = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
#endif  

		const auto& pAdapter = GraphicsAdapter::UseNullDevice() ? NULL : currentAdapter.impl->dxAdapter.Get();

		//
		// if pAdapter is not NULL driverType must be D3D_DRIVER_TYPE_UNKNOWN
		//
		auto driverType = D3D_DRIVER_TYPE_UNKNOWN;

		if (GraphicsAdapter::UseReferenceDevice())
			driverType = D3D_DRIVER_TYPE_WARP;
		else if (GraphicsAdapter::UseNullDevice())
			driverType = D3D_DRIVER_TYPE_HARDWARE;

		auto hr = D3D11CreateDevice(
			//_In_opt_ IDXGIAdapter* pAdapter,
			pAdapter,
			//D3D_DRIVER_TYPE DriverType,
			driverType,
			//HMODULE Software,
			NULL,
			//UINT Flags,
			createDeviceFlags,
			//_In_reads_opt_( FeatureLevels ) CONST D3D_FEATURE_LEVEL* pFeatureLevels,
			impl.featureLevels,
			//UINT FeatureLevels,
			7,
			//UINT SDKVersion,
			D3D11_SDK_VERSION,
			//_COM_Outptr_opt_ ID3D11Device** ppDevice
			impl._device.GetAddressOf(),
			//_Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,
			&impl.currentFeatureLevel,
			//_COM_Outptr_opt_ ID3D11DeviceContext** ppImmediateContext
			impl._context.GetAddressOf());

		if FAILED(hr)
			Exception::Throw(Exception::FAILED_TO_CREATE);
	}

	static void initAndApplyState(P_BlendState& blendState, P_RasterizerState& rasterizerState, P_DepthStencilState& depthStencilState, P_SamplerStateCollection& samplerStates, P_GraphicsDevice const& device) {
		blendState->Bind(device);
		blendState->Initialize();
		blendState->Apply();

		rasterizerState->Bind(device);
		rasterizerState->Initialize();
		rasterizerState->Apply();

		depthStencilState->Bind(device);
		depthStencilState->Initialize();
		depthStencilState->Apply();

		samplerStates->Apply(*device);
	}
}