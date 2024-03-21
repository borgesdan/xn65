#ifndef XNA_GRAPHICS_DEVICE_HPP
#define XNA_GRAPHICS_DEVICE_HPP

#include "../enums.hpp"
#include "../forward.hpp"
#include "../game/window.hpp"
#include "../types.hpp"
#include "adapter.hpp"
#include "rendertarget.hpp"
#include "swapchain.hpp"
#include "viewport.hpp"
#include "blendstate.hpp"

namespace xna {
	class IGraphicsDevice {
	public:
		virtual ~IGraphicsDevice() {}
		virtual void Clear() = 0;
		virtual bool Initialize(GameWindow& gameWindow) = 0;
		virtual bool Present() = 0;

		PGraphicsAdapter Adapter() const {
			return _adapter;
		}

		void Adapter(PGraphicsAdapter const& adapter) {
			_adapter = adapter;
		}

		constexpr xna::Viewport Viewport() const {
			return _viewport;
		}

		constexpr void Viewport(xna::Viewport const& viewport) {
			_viewport = viewport;
		}

		PSwapChain GetSwapChain() const {
			return _swapChain;
		}

		constexpr void UseVSync(bool use) {
			_usevsync = use;
		}

	protected:
		PGraphicsAdapter _adapter{ nullptr };
		PSwapChain _swapChain{ nullptr };
		PRenderTarget2D _renderTarget2D{ nullptr };
		xna::Viewport _viewport{};
		PBlendState _blendState{ nullptr };
		bool _usevsync{ false };
	};
}

#endif