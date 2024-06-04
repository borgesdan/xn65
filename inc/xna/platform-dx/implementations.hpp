#ifndef XNA_PLATFORM_DX_IMPLEMENTATIONS_HPP
#define XNA_PLATFORM_DX_IMPLEMENTATIONS_HPP

#include "headers.hpp"
#include "stepTimer.hpp"
#include "../graphics/device.hpp"
#include "../graphics/adapter.hpp"
#include "../graphics/blendstate.hpp"
#include "../graphics/buffer.hpp"
#include "../graphics/depthstencilstate.hpp"
#include "../graphics/displaymode.hpp"
#include "../graphics/sprite.hpp"
#include "../graphics/samplerstate.hpp"
#include "../input/gamepad.hpp"
#include "../input/keyboard.hpp"
#include "../input/mouse.hpp"
#include "../graphics/rasterizerstate.hpp"
#include "../graphics/presentparams.hpp"
#include "../graphics/shader.hpp"
#include "../graphics/swapchain.hpp"
#include "../graphics/texture.hpp"
#include "../graphics/rendertarget.hpp"
#include "../game/window.hpp"
#include "../audio/audioengine.hpp"
#include "../audio/soundeffect.hpp"
#include "../graphics/viewport.hpp"
#include "../common/color.hpp"
#include "../game/game.hpp"

namespace xna {
	struct SpriteFont::PlatformImplementation {
		sptr<DirectX::SpriteFont> _dxSpriteFont = nullptr;
	};

	struct SpriteBatch::PlatformImplementation {
		sptr<DirectX::SpriteBatch> _dxspriteBatch = nullptr;
	};

	struct GraphicsAdapter::PlatformImplementation {
		~PlatformImplementation() {
			if (dxadapter) {
				dxadapter->Release();
				dxadapter = nullptr;
			}
		}

		IDXGIAdapter1* dxadapter = nullptr;
	private:
		friend class GraphicsAdapter;
		Uint _index{ 0 };
		sptr<DisplayMode> _currentDisplayMode = nullptr;

	public:
		bool GetOutput(UINT slot, IDXGIOutput*& output);
	};

	struct BlendRenderTarget {
		bool Enabled{ true };
		Blend Source{ Blend::SourceAlpha };
		Blend Destination{ Blend::InverseSourceAlpha };
		BlendOperation Operation{ BlendOperation::Add };
		Blend SourceAlpha{ Blend::One };
		Blend DestinationAlpha{ Blend::Zero };
		BlendOperation OperationAlpha{ BlendOperation::Add };
		ColorWriteChannels WriteMask{ ColorWriteChannels::All };

		constexpr BlendRenderTarget() = default;
	};

	struct BlendState::PlatformImplementation {
		~PlatformImplementation() {
			if (dxBlendState) {
				dxBlendState->Release();
				dxBlendState = nullptr;
			}
		}

		ID3D11BlendState* dxBlendState = nullptr;
		D3D11_BLEND_DESC dxDescription{};
		float blendFactor[4]{ 1.0F, 1.0F, 1.0F, 1.0F };
		UINT sampleMask{ 0xffffffff };
	};

	struct ConstantBuffer::PlatformImplementation {
		~PlatformImplementation() {
			if (_buffer) {
				_buffer->Release();
				_buffer = nullptr;
			}
		}

		D3D11_BUFFER_DESC _description{};
		D3D11_SUBRESOURCE_DATA _subResource{};
		ID3D11Buffer* _buffer = nullptr;
		DirectX::XMMATRIX _worldViewProjection{};
	};

	struct DataBuffer::PlatformImplementation {
		~PlatformImplementation() {
			if (_blob) {
				_blob->Release();
				_blob = nullptr;
			}
		}

		ID3DBlob* _blob = nullptr;

		void Set(ID3DBlob*& blob) {
			_blob = blob;
		}
	};

	struct DepthStencilState::PlatformImplementation {
		~PlatformImplementation() {
			if (dxDepthStencil) {
				dxDepthStencil->Release();
				dxDepthStencil = nullptr;
			}
		}

		ID3D11DepthStencilState* dxDepthStencil = nullptr;
		D3D11_DEPTH_STENCIL_DESC dxDescription{};
	};

	struct DisplayModeRefreshRate {
		constexpr DisplayModeRefreshRate() = default;

		constexpr DisplayModeRefreshRate(DXGI_RATIONAL const& dxrational) {
			Numerator = dxrational.Numerator;
			Denominator = dxrational.Denominator;
		}
		constexpr DisplayModeRefreshRate(Uint numerator, Uint denominator)
			: Numerator(numerator), Denominator(denominator) {}

		Uint Numerator{ 0 };
		Uint Denominator{ 0 };

		constexpr bool operator==(const DisplayModeRefreshRate& other) const
		{
			return Numerator == other.Numerator && Denominator == other.Denominator;
		}
	};

	struct DisplayModeDescription {
		DisplayModeScanlineOrder _scanlineOrdering{ DisplayModeScanlineOrder::Unspecified };
		DisplayModeScaling _scaling{ DisplayModeScaling::Unspecified };
		DisplayModeRefreshRate _refreshRate{};

		constexpr bool operator==(const DisplayModeDescription& other) const
		{
			return _scanlineOrdering == other._scanlineOrdering && _scaling == other._scaling && _refreshRate == other._refreshRate;
		}
	};

	struct DisplayMode::PlatformImplementation {
		std::vector<DisplayModeDescription> Descriptions;
	};

	struct GamePad::PlatformImplementation {
		inline static uptr<DirectX::GamePad> _dxGamePad = nullptr;

		void Suspend() {
			if (_dxGamePad)
				_dxGamePad->Suspend();
		}

		void Resume() {
			if (_dxGamePad)
				_dxGamePad->Resume();
		}
	};

	struct IndexBuffer::PlatformImplementation {
		~PlatformImplementation() {
			if (dxBuffer) {
				dxBuffer->Release();
				dxBuffer = nullptr;
			}
		}

		ID3D11Buffer* dxBuffer = nullptr;
	};

	struct Keyboard::PlatformImplementation {
		inline static uptr<DirectX::Keyboard> _dxKeyboard = nullptr;

		void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam) {
			if (_dxKeyboard)
				_dxKeyboard->ProcessMessage(message, wParam, lParam);
		}
	};

	struct Mouse::PlatformImplementation {
		inline static uptr<DirectX::Mouse> _dxMouse = nullptr;

		void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam) {
			if (_dxMouse)
				_dxMouse->ProcessMessage(message, wParam, lParam);
		}
	};

	struct RasterizerState::PlatformImplementation {
		~PlatformImplementation() {
			if (dxRasterizerState) {
				dxRasterizerState->Release();
				dxRasterizerState = nullptr;
			}
		}

		ID3D11RasterizerState* dxRasterizerState = nullptr;
		D3D11_RASTERIZER_DESC dxDescription{};
	};

	struct SamplerState::PlatformImplementation {
		~PlatformImplementation() {
			if (_samplerState) {
				_samplerState->Release();
				_samplerState = nullptr;
			}
		}

		ID3D11SamplerState* _samplerState = nullptr;
		D3D11_SAMPLER_DESC _description{};
	};

	struct VertexShader::PlatformImplementation {
		~PlatformImplementation() {
			if (_vertexShader) {
				_vertexShader->Release();
				_vertexShader = nullptr;
			}
		}

		ID3D11VertexShader* _vertexShader = nullptr;
	};

	struct PixelShader::PlatformImplementation {
		~PlatformImplementation() {
			if (_pixelShader) {
				_pixelShader->Release();
				_pixelShader = nullptr;
			}
		}

		ID3D11PixelShader* _pixelShader = nullptr;
	};

	struct SwapChain::PlatformImplementation {
		IDXGISwapChain1* dxSwapChain{ nullptr };
		DXGI_SWAP_CHAIN_DESC1 dxDescription{};
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC dxFullScreenDescription{};

		bool GetBackBuffer(ID3D11Texture2D*& texture2D) {
			if (!dxSwapChain)
				return false;

			const auto hr = dxSwapChain->GetBuffer(0, __uuidof(texture2D), (void**)(&texture2D));

			return !FAILED(hr);
		}
	};

	struct Texture2D::PlatformImplementation {
		~PlatformImplementation() {
			if (dxTexture2D) {
				dxTexture2D->Release();
				dxTexture2D = nullptr;
			}

			if (dxShaderResource) {
				dxShaderResource->Release();
				dxShaderResource = nullptr;
			}
		}

		ID3D11Texture2D* dxTexture2D{ nullptr };
		ID3D11ShaderResourceView* dxShaderResource{ nullptr };
		D3D11_SUBRESOURCE_DATA dxSubResource{};
		D3D11_TEXTURE2D_DESC dxDescription{};
		D3D11_SHADER_RESOURCE_VIEW_DESC dxShaderDescription{};
	};

	struct RenderTarget2D::PlatformImplementation {
		~PlatformImplementation() {
			if (_renderTargetView) {
				_renderTargetView->Release();
				_renderTargetView = nullptr;
			}
		}

		ID3D11RenderTargetView* _renderTargetView = nullptr;
		D3D11_RENDER_TARGET_VIEW_DESC _renderTargetDesc{};
	};

	struct VertexBuffer::PlatformImplementation {
		~PlatformImplementation() {
			if (dxBuffer) {
				dxBuffer->Release();
				dxBuffer = nullptr;
			}
		}

		ID3D11Buffer* dxBuffer = nullptr;
		UINT size{ 0 };
	};

	struct VertexInputLayout::PlatformImplementation {
		~PlatformImplementation() {
			if (_inputLayout) {
				_inputLayout->Release();
				_inputLayout = nullptr;
			}
		}

		ID3D11InputLayout* _inputLayout{ nullptr };
		std::vector<D3D11_INPUT_ELEMENT_DESC> _description{};
	};

	enum class GameWindowMode : UINT {
		Fullscreen = WS_POPUP | WS_VISIBLE,
		Windowed = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE,
		Borderless = WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE,
	};

	struct GameWindow::PlatformImplementation {
	public:
		constexpr void Mode(GameWindowMode mode) {
			_windowStyle = static_cast<int>(mode);
		}

		constexpr GameWindowMode Mode() const {
			return static_cast<GameWindowMode>(_windowStyle);
		}

		void Position(int width, int height, bool update = true);
		void Size(int width, int height, bool update = true);

		inline HINSTANCE HInstance() const {
			return _hInstance;
		}

		inline HWND WindowHandle() const {
			return _windowHandle;
		}

		constexpr int Width() const {
			return _windowWidth;
		}

		constexpr int Height() const {
			return _windowHeight;
		}

		inline void Icon(unsigned int icon) {
			_windowIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(icon));
		}

		inline void Icon(HICON icon) {
			_windowIcon = icon;
		}

		inline void Cursor(unsigned int cursor) {
			_windowCursor = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(cursor));
		}

		inline void Cursor(HCURSOR cursor) {
			_windowCursor = cursor;
		}

		constexpr float CenterX() const {
			return _windowCenterX;
		}

		constexpr float CenterY() const {
			return _windowCenterY;
		}

		inline void CursorVisibility(bool visible) const {
			ShowCursor(visible);
		}

		inline void Close() {
			PostMessage(_windowHandle, WM_DESTROY, 0, 0);
		}

		constexpr COLORREF Color()	const {
			return _windowColor;
		}

		constexpr void Color(COLORREF color) {
			_windowColor = color;
		}

		constexpr void Color(BYTE r, BYTE g, BYTE b) {
			_windowColor = RGB(r, g, b);
		}

		bool Create();
		bool Update();

		static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		friend class GameWindow;

		HINSTANCE		_hInstance{ nullptr };
		HWND			_windowHandle{ nullptr };
		int				_windowWidth{ 800 };
		int				_windowHeight{ 480 };
		HICON			_windowIcon{ nullptr };
		HCURSOR			_windowCursor{ nullptr };
		COLORREF		_windowColor{ RGB(0,0,0) };
		String			_windowTitle{ "Xna++ Game Development" };
		DWORD			_windowStyle{ 0 };
		int				_windowPosX{ 0 };
		int				_windowPosY{ 0 };
		float			_windowCenterX{ 0 };
		float			_windowCenterY{ 0 };

		inline void setPosition() {
			_windowPosX = GetSystemMetrics(SM_CXSCREEN) / 2 - _windowWidth / 2;
			_windowPosY = GetSystemMetrics(SM_CYSCREEN) / 2 - _windowHeight / 2;
		}

		inline void setCenter() {
			_windowCenterX = _windowWidth / 2.0f;
			_windowCenterY = _windowHeight / 2.0f;
		}
	};

	struct AudioEngine::PlatformImplementation {
		PlatformImplementation() {
			_dxAudioEngine = unew<DirectX::AudioEngine>(
#ifdef _DEBUG
				DirectX::AudioEngine_Debug
#endif
			);
		}

		~PlatformImplementation() {
			if (_dxAudioEngine) {
				_dxAudioEngine->Suspend();
			}
		}

		uptr<DirectX::AudioEngine> _dxAudioEngine = nullptr;
	};

	struct GraphicsDevice::PlatformImplementation {
		ID3D11Device* _device{ nullptr };
		ID3D11DeviceContext* _context{ nullptr };
		IDXGIFactory1* _factory = nullptr;
		sptr<SwapChain> _swapChain{ nullptr };
		sptr<GraphicsAdapter> _adapter{ nullptr };
		sptr<RenderTarget2D> _renderTarget2D{ nullptr };
		sptr<BlendState> _blendState{ nullptr };
		xna::Viewport _viewport{};
		sptr<xna::PresentationParameters> _presentationParameters;
		D3D_FEATURE_LEVEL _featureLevel{ D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0 };

	private:
		friend class GraphicsDevice;
		float _backgroundColor[4] = { 0, 0, 0, 0 };
		bool _usevsync{ true };
	};

	struct Game::PlatformImplementation {
	private:
		friend class Game;
		
		xna::StepTimer _stepTimer{};
	};

	struct SoundEffectInstance::PlatformImplementation {
		uptr<DirectX::SoundEffectInstance> _dxInstance = nullptr;
	};

	struct SoundEffect::PlatformImplementation {
		~PlatformImplementation() {			
		}

		uptr<DirectX::SoundEffect> _dxSoundEffect = nullptr;
	};

	template <typename T>
	inline bool IndexBuffer::Initialize(std::vector<T> const& data, xna_error_ptr_arg) {
		if (!impl || !m_device || !m_device->impl->_device || data.empty()) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		const auto hr = DirectX::CreateStaticBuffer(m_device->impl->_device, data.data(), data.size(), sizeof(T), D3D11_BIND_INDEX_BUFFER, &impl->dxBuffer);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		return true;
	}

	template <typename T>
	inline bool VertexBuffer::Initialize(std::vector<T> const& data, xna_error_ptr_arg) {
		if (!impl || !m_device || !m_device->impl->_device || data.empty()) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		const auto hr = DirectX::CreateStaticBuffer(m_device->impl->_device, data.data(), data.size(), sizeof(T), D3D11_BIND_VERTEX_BUFFER, &impl->dxBuffer);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		impl->size = sizeof(T);

		return true;
	}
}

#endif