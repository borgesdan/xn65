#ifndef XNA_PLATFORM_DEVICE_DX_HPP
#define XNA_PLATFORM_DEVICE_DX_HPP

#include "../graphics/device.hpp"
#include "../graphics/presentparams.hpp"
#include "dxgi.h"
#include "d3d11.h"

namespace xna {
	class GraphicsDevice : public IGraphicsDevice {
	public:
		GraphicsDevice();
		GraphicsDevice(GraphicsDeviceInformation const& info);

		virtual ~GraphicsDevice() override {
			if (_device) {
				_device->Release();
				_device = nullptr;
			}

			if (_context) {
				_context->Release();
				_context = nullptr;
			}
		}

		virtual void Clear() override;
		virtual bool Initialize(GameWindow& gameWindow) override;
		virtual bool Present() override;

		virtual PGraphicsAdapter Adapter() const override {
			return _adapter;
		}

		virtual void Adapter(PGraphicsAdapter const& adapter) override {
			_adapter = adapter;
		}

		virtual constexpr xna::Viewport Viewport() const override {
			return _viewport;
		}

		virtual constexpr void Viewport(xna::Viewport const& viewport) override {
			_viewport = viewport;
		}

		virtual void UseVSync(bool use) override {
			_usevsync = use;
		}

		constexpr void SetCreateFlags(D3D11_CREATE_DEVICE_FLAG flags) {
			_createDeviceFlags |= flags;
		}

		constexpr void ClearCreateFlags() {
			_createDeviceFlags = 0;
		}

		bool GetSwapChainBackBuffer(ID3D11Texture2D*& texture2D);

	public:
		ID3D11Device* _device{ nullptr };
		ID3D11DeviceContext* _context{ nullptr };

	private:
		unsigned int _createDeviceFlags{ 0 };
		D3D_FEATURE_LEVEL _featureLevel{ D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0 };
		float _backgroundColor[4] = { 0, 0, 0, 0 };
		xna::PresentationParameters _presentParameters;
		PGraphicsAdapter _adapter{ nullptr };
		PSwapChain _swapChain{ nullptr };
		PRenderTarget2D _renderTarget2D{ nullptr };
		xna::Viewport _viewport{};
		PBlendState _blendState{ nullptr };
		bool _usevsync{ false };

		bool createDevice();
	};	
}

#endif