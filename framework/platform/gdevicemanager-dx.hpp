#ifndef XNA_PLATFORM_GDEVICEMANAGER_DX_HPP
#define XNA_PLATFORM_GDEVICEMANAGER_DX_HPP

#include "../game/gdevicemanager.hpp"

namespace xna {
	class GraphicsDeviceManager : public IGraphicsDeviceManager {
	public:
		GraphicsDeviceManager(Game* game) : _game(game){}

		virtual void ApplyChanges() override;
		virtual void ToggleFullScreen() override;
		
		virtual constexpr Int PreferredBackBufferWidth() const {
			return _backBufferWidth;
		}

		virtual constexpr Int PreferredBackBufferHeight() const {
			return _backBufferHeight;
		}

		virtual void PreferredBackBufferWidth(Int value) {
			_backBufferWidth = value;
			_isDeviceDirty = true;
		}

		virtual void PreferredBackBufferHeight(Int value) {
			_backBufferHeight = value;
			_isDeviceDirty = true;
		}

	protected:
		virtual void CreateDevice(GraphicsDeviceInformation const& info) override;
		virtual void ChangeDevice() override;

	public:
		static constexpr int DefaultBackBufferWidth = 800;
		static constexpr int DefaultBackBufferHeight = 480;

	private:
		Game*& _game;
		Int _backBufferWidth{ DefaultBackBufferWidth };
		Int _backBufferHeight{ DefaultBackBufferHeight };
		bool _isDeviceDirty{ false };
		PGraphicsDevice _device;

	};
}

#endif