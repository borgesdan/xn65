#ifndef XNA_PLATFORM_GDEVICEMANAGER_DX_HPP
#define XNA_PLATFORM_GDEVICEMANAGER_DX_HPP

#include "../game/gdevicemanager.hpp"
#include "game/gdeviceinfo.hpp"

namespace xna {
	class GraphicsDeviceManager : public IGraphicsDeviceManager {
	public:
		GraphicsDeviceManager(Game*& game);

		virtual ~GraphicsDeviceManager() override{}

		virtual void ApplyChanges() override;
		virtual bool Initialize() override;
		virtual bool ToggleFullScreen() override;
		
		virtual constexpr Int PreferredBackBufferWidth() const {
			return _backBufferWidth;
		}

		virtual constexpr Int PreferredBackBufferHeight() const {
			return _backBufferHeight;
		}

		virtual void PreferredBackBufferWidth(Int value);
		virtual void PreferredBackBufferHeight(Int value);

	protected:
		virtual bool CreateDevice() override;
		virtual void ChangeDevice() override;

	public:
		static constexpr int DefaultBackBufferWidth = 800;//800;
		static constexpr int DefaultBackBufferHeight = 600;// 480;

	private:
		Game* _game;
		Int _backBufferWidth{ DefaultBackBufferWidth };
		Int _backBufferHeight{ DefaultBackBufferHeight };
		bool _isDeviceDirty{ false };		
		sptr<GraphicsDevice> _device = nullptr;
		bool _isFullScreen{ false };
		GraphicsDeviceInformation _information{};

		bool initWindow();
		bool initDevice();

	};
}

#endif