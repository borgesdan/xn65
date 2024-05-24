#ifndef XNA_GAME_GRAPHICSDEVICEMANAGER_HPP
#define XNA_GAME_GRAPHICSDEVICEMANAGER_HPP

#include "../default.hpp"
#include "gdeviceinfo.hpp"

namespace xna {
	class GraphicsDeviceManager {
	public:
		GraphicsDeviceManager(sptr<Game> const& game);
		~GraphicsDeviceManager() {}
		void ApplyChanges();
		bool Initialize();
		bool ToggleFullScreen();
		Int PreferredBackBufferWidth() const;
		Int PreferredBackBufferHeight() const;
		void PreferredBackBufferWidth(Int value);
		void PreferredBackBufferHeight(Int value);

	public:
		static constexpr int DefaultBackBufferWidth = 800;
		static constexpr int DefaultBackBufferHeight = 600;

	protected:
		bool CreateDevice();
		void ChangeDevice();

	private:
		sptr<Game> _game = nullptr;
		Int _backBufferWidth{ DefaultBackBufferWidth };
		Int _backBufferHeight{ DefaultBackBufferHeight };
		bool _isDeviceDirty{ false };		
		sptr<GraphicsDevice> _device = nullptr;
		bool _isFullScreen{ false };
		GraphicsDeviceInformation _information{};
	};
}

#endif