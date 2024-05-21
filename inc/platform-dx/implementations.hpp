#include "dxheaders.hpp"
#include "graphics/adapter.hpp"
#include "graphics/blendstate.hpp"
#include "graphics/buffer.hpp"
#include "graphics/depthstencilstate.hpp"
#include "graphics/device.hpp"
#include "graphics/displaymode.hpp"
#include "graphics/sprite.hpp"
#include "input/gamepad.hpp"
#include "platform-dx/presentparameters-dx.hpp"
#include "platform-dx/rendertarget-dx.hpp"
#include "platform-dx/swapchain-dx.hpp"

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
		~PlatformImplementation() {
			if (_dxGamePad) {
				_dxGamePad->Suspend();				
				_dxGamePad = nullptr;
			}
		}

		inline static uptr<DirectX::GamePad> _dxGamePad = nullptr;
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
}