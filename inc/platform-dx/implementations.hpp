#include "dxheaders.hpp"
#include "graphics/adapter.hpp"
#include "graphics/blendstate.hpp"
#include "graphics/buffer.hpp"
#include "graphics/depthstencilstate.hpp"
#include "graphics/device.hpp"
#include "graphics/displaymode.hpp"
#include "graphics/sprite.hpp"
#include "graphics/samplerstate.hpp"
#include "input/gamepad.hpp"
#include "input/keyboard.hpp"
#include "input/mouse.hpp"
#include "graphics/rasterizerstate.hpp"
#include "graphics/presentparams.hpp"
#include "platform-dx/rendertarget-dx.hpp"
#include "graphics/shader.hpp"
#include "graphics/swapchain.hpp"

namespace xna {
	struct SpriteFont::PlatformImplementation {
		sptr<DirectX::SpriteFont> _dxSpriteFont = nullptr;
	};

	struct SpriteBatch::PlatformImplementation {
		sptr<DirectX::SpriteBatch> _dxspriteBatch = nullptr;
	};

	struct GraphicsAdapter::PlatformImplementation {
		~PlatformImplementation(){
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
		DirectX::XMMATRIX _worldViewProjection;
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

	template <typename T>
	inline bool IndexBuffer::Initialize(std::vector<T> const& data, xna_error_ptr_arg) {
		if (!impl || !m_device || !m_device->_device || data.empty()) {
			xna_error_apply(err, XnaErrorCode::INVALID_OPERATION);
			return false;
		}

		const auto hr = DirectX::CreateStaticBuffer(m_device->_device, data.data(), data.size(), sizeof(T), D3D11_BIND_INDEX_BUFFER, &impl->dxBuffer);

		if (FAILED(hr)) {
			xna_error_apply(err, XnaErrorCode::FAILED_OPERATION);
			return false;
		}

		return true;
	}

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
}