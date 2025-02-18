#ifndef XNA_DX_IMPLEMENTATIONS_HPP
#define XNA_DX_IMPLEMENTATIONS_HPP

#include "headers.hpp"

namespace xna {
	struct BlendStateImplementation {
		D3D11_BLEND_DESC Description{};
		float BlendFactor[4]{ 1.0F, 1.0F, 1.0F, 1.0F };
		UINT SampleMask{ 0xffffffff };
		comptr<ID3D11BlendState> BlendState;

		static constexpr int MAX_RENDER_TARGETS = 8;
	};

	struct DepthStencilStateImplementation {
		comptr<ID3D11DepthStencilState> DepthStencil;
		D3D11_DEPTH_STENCIL_DESC Description{};
	};

	struct GraphicsAdapterImplementation {
		comptr<IDXGIAdapter1> Adapter;
		comptr<IDXGIFactory1> Factory;
	};	

	struct GraphicsDeviceImplementation {
		comptr<ID3D11Device> Device;
		comptr<ID3D11DeviceContext> Context;
		comptr<IDXGIFactory1> Factory;
		std::shared_ptr<SwapChain> SwapChain;
		std::shared_ptr<RenderTarget2D> RenderTarget2D;
		intptr_t WindowHandle{ 0 };

		static constexpr int FeatureLevelCount = 7;
		inline static constexpr D3D_FEATURE_LEVEL FeatureLevels[FeatureLevelCount] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};

		D3D_FEATURE_LEVEL CurrentFeatureLevel{ D3D_FEATURE_LEVEL_11_1 };

		void Create(GraphicsAdapter& currentAdapter);
		void Reset();		

	private:
		friend class GraphicsDevice;
		float backgroundColor[4] = { 0, 0, 0, 0 };
		UINT vSyncValue = 1;
	};

	struct RasterizerStateImplementation {
		comptr<ID3D11RasterizerState> RasterizerState;
		D3D11_RASTERIZER_DESC Description{};
	};

	struct RenderTarget2DImplementation {
		comptr<ID3D11RenderTargetView> RenderTargetView;
		D3D11_RENDER_TARGET_VIEW_DESC Description{};
	};

	struct SamplerStateImplementation {
		comptr<ID3D11SamplerState> SamplerState;
		D3D11_SAMPLER_DESC Description{};
	};

	struct SpriteBatchImplementation {
		std::shared_ptr<DirectX::SpriteBatch> SpriteBatch;
		std::shared_ptr<DirectX::DX11::IEffect> EffectBuffer;
		comptr<ID3D11InputLayout> InputLayout;
	};

	struct SpriteFontImplementation {
		std::unique_ptr<DirectX::SpriteFont> SpriteFont;
	};	

	struct Texture2DImplementation {
		Texture2DImplementation() {
			Description.MipLevels = 1;
			Description.ArraySize = 1;
			Description.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			Description.SampleDesc.Count = 1;
			Description.Usage = D3D11_USAGE_DEFAULT;
			Description.BindFlags = D3D11_BIND_SHADER_RESOURCE;

			ShaderDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			ShaderDescription.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			ShaderDescription.Texture2D.MipLevels = Description.MipLevels;
			ShaderDescription.Texture2D.MostDetailedMip = 0;
		}

		comptr<ID3D11Texture2D> Texture2D;
		comptr<ID3D11ShaderResourceView> ShaderResource;
		D3D11_SUBRESOURCE_DATA SubResource{};
		D3D11_TEXTURE2D_DESC Description{};
		D3D11_SHADER_RESOURCE_VIEW_DESC ShaderDescription{};

		HRESULT SetData(GraphicsDevice& device, UINT const* data);
	};

	struct GamePad::PlatformImplementation {
		uptr<DirectX::GamePad> _dxGamePad = unew<DirectX::GamePad>();

		void Suspend() const {
			if (_dxGamePad)
				_dxGamePad->Suspend();
		}

		void Resume() const {
			if (_dxGamePad)
				_dxGamePad->Resume();
		}
	};

	struct Keyboard::PlatformImplementation {
		PlatformImplementation() {
			_dxKeyboard = unew<DirectX::Keyboard>();
		}

		uptr<DirectX::Keyboard> _dxKeyboard = nullptr;

		inline void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam) const {
			if (_dxKeyboard)
				_dxKeyboard->ProcessMessage(message, wParam, lParam);
		}
	};

	struct Mouse::PlatformImplementation {
		PlatformImplementation() {
			_dxMouse = unew<DirectX::Mouse>();
		}

		inline void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam) const {
			if (_dxMouse)
				_dxMouse->ProcessMessage(message, wParam, lParam);
		}

		uptr<DirectX::Mouse> _dxMouse = nullptr;
	};	

	struct SwapChain::PlatformImplementation {
		comptr<IDXGISwapChain1> dxSwapChain{ nullptr };
		DXGI_SWAP_CHAIN_DESC1 dxDescription{};
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC dxFullScreenDescription{};

		bool GetBackBuffer(comptr<ID3D11Texture2D>& texture2D) const {
			if (!dxSwapChain)
				return false;

			const auto hr = dxSwapChain->GetBuffer(0, __uuidof(texture2D), (void**)texture2D.GetAddressOf());

			return !FAILED(hr);
		}
	};		

	enum class GameWindowMode : UINT {
		Fullscreen = WS_POPUP | WS_VISIBLE,
		Windowed = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE,
		Borderless = WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE,
	};

	struct GameWindow::PlatformImplementation {
	public:
		PlatformImplementation(GameWindow* gameWindow) : gameWindow(gameWindow) {}

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

		inline void Close() const {
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
		GameWindow* gameWindow = nullptr;

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

	struct Game::PlatformImplementation {
	private:
		friend class Game;

		xna::StepTimer _stepTimer{};
	};

	struct SoundEffectInstance::PlatformImplementation {
		uptr<DirectX::SoundEffectInstance> _dxInstance = nullptr;
	};

	struct SoundEffect::PlatformImplementation {
		uptr<DirectX::SoundEffect> _dxSoundEffect = nullptr;
	};

	struct Effect::PlatformImplementation {
		sptr<DirectX::IEffect> dxEffect = nullptr;
	};

	struct BasicEffect::PlatformImplementation {
		sptr<DirectX::BasicEffect> dxBasicEffect = nullptr;
	};
}

#endif