#include "graphics/sprite.hpp"
#include "graphics/device.hpp"
#include "platform-dx/adapter-dx.hpp"
#include "platform-dx/presentparameters-dx.hpp"
#include "dxheaders.hpp"
#include "platform-dx/swapchain-dx.hpp"
#include "platform-dx/rendertarget-dx.hpp"
#include "platform-dx/blendstate-dx.hpp"

namespace xna {
	struct SpriteFont::PlatformImplementation {
		sptr<DirectX::SpriteFont> _dxSpriteFont = nullptr;
	};

	struct SpriteBatch::PlatformImplementation {
		sptr<DirectX::SpriteBatch> _dxspriteBatch = nullptr;
	};		
}