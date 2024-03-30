#ifndef XNA_GAME_GRAPHICSDEVICEMANAGER_HPP
#define XNA_GAME_GRAPHICSDEVICEMANAGER_HPP

#include "../types.hpp"
#include "../csharp/timespan.hpp"
#include "../forward.hpp"

namespace xna {
	class IGraphicsDeviceManager {
	public:
		virtual void ApplyChanges() = 0;
		virtual void ToggleFullScreen() = 0;
		virtual Int PreferredBackBufferWidth() const = 0;
		virtual Int PreferredBackBufferHeight() const = 0;
		virtual void PreferredBackBufferWidth(Int value) = 0;
		virtual void PreferredBackBufferHeight(Int value) = 0;
	protected:
		virtual void CreateDevice(GraphicsDeviceInformation const& info) = 0;
		virtual void ChangeDevice() = 0;			
	};
}

#endif