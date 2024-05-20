#include "graphics/sprite.hpp"
#include "graphics/device.hpp"
#include "graphics/adapter.hpp"
#include "graphics/buffer.hpp"
#include "platform-dx/presentparameters-dx.hpp"
#include "dxheaders.hpp"
#include "platform-dx/swapchain-dx.hpp"
#include "platform-dx/rendertarget-dx.hpp"
#include "graphics/blendstate.hpp"
#include "graphics/depthstencilstate.hpp"

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
}