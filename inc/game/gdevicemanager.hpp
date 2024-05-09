#ifndef XNA_GAME_GRAPHICSDEVICEMANAGER_HPP
#define XNA_GAME_GRAPHICSDEVICEMANAGER_HPP

#include "../default.hpp"

namespace xna {
	class IGraphicsDeviceManager {
	public:
		virtual ~IGraphicsDeviceManager(){}
		virtual void ApplyChanges() = 0;
		virtual bool Initialize() = 0;
		virtual bool ToggleFullScreen() = 0;
		virtual Int PreferredBackBufferWidth() const = 0;
		virtual Int PreferredBackBufferHeight() const = 0;
		virtual void PreferredBackBufferWidth(Int value) = 0;
		virtual void PreferredBackBufferHeight(Int value) = 0;
	protected:
		virtual bool CreateDevice() = 0;
		virtual void ChangeDevice() = 0;			
	};
}

#endif