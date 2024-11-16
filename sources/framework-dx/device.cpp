#include "xna-dx/framework.hpp"

namespace xna {
	void GraphicsDeviceImplementation::Create(GraphicsAdapter& currentAdapter) {
		// See ref
		//
		// D3D_DRIVER_TYPE
		// https://learn.microsoft.com/en-us/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_driver_type
		// D3D11CreateDevice function 
		// https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-d3d11createdevice
		//		

		auto createDeviceFlags = 0;
#if _DEBUG
		createDeviceFlags = D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG;
#endif  
		const auto& pAdapter = GraphicsAdapter::UseNullDevice() ? NULL : currentAdapter.Implementation->Adapter.Get();

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
			FeatureLevels,
			//UINT FeatureLevels,
			static_cast<UINT>(FeatureLevelCount),
			//UINT SDKVersion,
			D3D11_SDK_VERSION,
			//_COM_Outptr_opt_ ID3D11Device** ppDevice
			Device.GetAddressOf(),
			//_Out_opt_ D3D_FEATURE_LEVEL* pFeatureLevel,
			&CurrentFeatureLevel,
			//_COM_Outptr_opt_ ID3D11DeviceContext** ppImmediateContext
			Context.GetAddressOf());

		if FAILED(hr)
			Exception::Throw(Exception::FAILED_TO_CREATE);
	}

	void GraphicsDeviceImplementation::Reset() {
		if (Device) {
			Device->Release();
			Device = nullptr;
		}

		if (Context) {
			Context->Release();
			Context = nullptr;
		}

		if (Factory) {
			Factory->Release();
			Factory = nullptr;
		}
	}
	
	static void initAndApplyState(P_BlendState& blendState, P_RasterizerState& rasterizerState,
		P_DepthStencilState& depthStencilState, P_SamplerStateCollection& samplerStates, P_GraphicsDevice const& device);

	GraphicsDevice::GraphicsDevice() {
		Implementation = unew<GraphicsDeviceImplementation>();
		adapter = GraphicsAdapter::DefaultAdapter();
	}

	GraphicsDevice::GraphicsDevice(sptr<GraphicsAdapter> const& adapter, GraphicsProfile const& graphicsProfile, sptr<PresentationParameters> const& presentationParameters)
		: adapter(adapter), graphicsProfile(graphicsProfile), presentationParameters(presentationParameters) {
		Implementation = unew<GraphicsDeviceImplementation>();

		blendState = xna::BlendState::Opaque();
		depthStencilState = xna::DepthStencilState::Default();
		rasterizerState = xna::RasterizerState::CullCounterClockwise();
		samplerStateCollection = snew<SamplerStateCollection>();
	}

	void GraphicsDevice::Initialize() {
		Implementation->Reset();

		auto _this = shared_from_this();

		Implementation->Create(*adapter);

		//
		// Background
		// 

		const auto backColor = Colors::CornflowerBlue;
		const auto backColorV3 = backColor.ToVector3();

		Implementation->backgroundColor[0] = backColorV3.X;
		Implementation->backgroundColor[1] = backColorV3.Y;
		Implementation->backgroundColor[2] = backColorV3.Z;
		Implementation->backgroundColor[3] = 1.0f;

		//
		// Window Association
		//

		auto hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&Implementation->Factory);

		if FAILED(hr)
			Exception::Throw(Exception::FAILED_TO_CREATE);

		auto hwnd = reinterpret_cast<HWND>(presentationParameters->DeviceWindowHandle);
		hr = Implementation->Factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);

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
		Implementation->Context->RSSetViewports(1, &view);

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
			Implementation->vSyncValue = 1;
			break;
		case PresentInterval::Immediate:
			Implementation->vSyncValue = 0;
			break;
		default:
			Implementation->vSyncValue = 1;
			break;
		}

		Implementation->SwapChain = snew<xna::SwapChain>(_this);
		Implementation->SwapChain->Initialize();

		//
		//Render Target
		//

		if (renderTarget) {
			renderTarget->Initialize();
			Implementation->RenderTarget2D = renderTarget;
		}
		else {
			Implementation->RenderTarget2D = RenderTarget2D::FromBackBuffer(_this);
		}

		const auto& renderView = Implementation->RenderTarget2D->Implementation2->RenderTargetView;
		Implementation->Context->OMSetRenderTargets(1, renderView.GetAddressOf(), nullptr);
	}

	bool GraphicsDevice::Present() const {
		bool result = Implementation->SwapChain->Present(Implementation->vSyncValue != 0);

		Implementation->Context->OMSetRenderTargets(
			1,
			Implementation->RenderTarget2D->Implementation2->RenderTargetView.GetAddressOf(),
			nullptr);

		return result;
	}

	void GraphicsDevice::Clear(Color const& color) const {
		if (!Implementation) return;

		const auto v4 = color.ToVector4();

		Implementation->backgroundColor[0] = v4.X;
		Implementation->backgroundColor[1] = v4.Y;
		Implementation->backgroundColor[2] = v4.Z;
		Implementation->backgroundColor[3] = v4.W;

		Implementation->Context->ClearRenderTargetView(
			Implementation->RenderTarget2D->Implementation2->RenderTargetView.Get(),
			Implementation->backgroundColor);
	}

	void GraphicsDevice::Clear(ClearOptions options, Color const& color, float depth, Int stencil) const {
		if (!Implementation) return;

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

		Implementation->Context->RSSetViewports(1, &view);
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

	void GraphicsDevice::Reset(sptr<PresentationParameters> const& parameters, sptr<GraphicsAdapter> const& graphicsAdapter) {
		Implementation = unew<GraphicsDeviceImplementation>();
		adapter = graphicsAdapter;
		presentationParameters = parameters;

		Initialize();
	}

	//
	// INTERNAL
	//	

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