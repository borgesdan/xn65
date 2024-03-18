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
	class GraphicsDevice : public std::enable_shared_from_this<GraphicsDevice> {
	public:
		PLATFORM_DEVELOPMENT
			GraphicsDevice();

		PLATFORM_DEVELOPMENT
			void Clear();

		PLATFORM_DEVELOPMENT
			void Clear(Color const& color);

		PLATFORM_DEVELOPMENT
			bool Initialize(GameWindow& gameWindow);

		PLATFORM_DEVELOPMENT
			bool Present();

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

	private:
		PGraphicsAdapter _adapter{ nullptr };
		PSwapChain _swapChain{ nullptr };
		PRenderTarget2D _renderTarget2D{ nullptr };
		xna::Viewport _viewport{};
		PBlendState _blendState{ nullptr };

	public:
		class InternalProperty;
		friend class InternalProperty;
		sptr<InternalProperty> ip_GraphicsDevice{ nullptr };
	};
}

#endif