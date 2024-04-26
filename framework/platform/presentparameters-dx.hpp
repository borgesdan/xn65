#ifndef XNA_PLATFORM_PRESENTPARAMETERS_DX_HPP
#define XNA_PLATFORM_PRESENTPARAMETERS_DX_HPP

#include "../graphics/presentparams.hpp"
#include "gdevicemanager-dx.hpp"
#include "dxheaders.hpp"

namespace xna {
	class PresentationParameters : public IPresentationParameters {
	public:
		virtual constexpr Uint BackBufferWidth() const override {
			return backBufferWidth;
		}

		virtual constexpr Uint BackBufferHeight() const override {
			return backBufferHeight;
		}

		virtual constexpr SurfaceFormat BackBufferFormat() const override {
			return backBufferFormat;
		}

		virtual constexpr SwapEffect PresentationSwapEffect() const override {
			return swapEffect;
		}

		virtual intptr_t DeviceWindowHandle() const override {
			return reinterpret_cast<intptr_t>(windowHandle);
		}

		virtual constexpr bool IsFullScreen() const override {
			return fullscreen;
		}

		HWND DeviceWindowHWND() const {
			return windowHandle;
		}

	public:
		Uint backBufferWidth { GraphicsDeviceManager::DefaultBackBufferWidth };
		Uint backBufferHeight{ GraphicsDeviceManager::DefaultBackBufferHeight };
		SurfaceFormat backBufferFormat{ SurfaceFormat::Color };
		SwapEffect swapEffect{ SwapEffect::FlipDiscard };
		HWND windowHandle = nullptr;
		bool fullscreen{ false };
	};
}

#endif