#ifndef XNA_GAME_GRAPHICSDEVICEMANAGER_HPP
#define XNA_GAME_GRAPHICSDEVICEMANAGER_HPP

#include "../default.hpp"
#include "gdeviceinfo.hpp"
#include "../csharp/eventhandler.hpp"

namespace xna {
	struct IGraphicsDeviceService {
		virtual sptr<GraphicsDevice> GetGraphicsDevice() = 0;

		EventHandler<EventArgs> DeviceDisposing;
		EventHandler<EventArgs> DeviceReset;
		EventHandler<EventArgs> DeviceResetting;
		EventHandler<EventArgs> DeviceCreated;
	};

	class IGraphicsDeviceManager {
		virtual void CreateDevice() = 0;
		virtual bool BeginDraw() = 0;
		virtual void EndDraw() = 0;
	};

	class GraphicsDeviceManager : public IGraphicsDeviceService, public IGraphicsDeviceManager {
	public:
		//Creates a new GraphicsDeviceManager and registers it to handle the configuration and management of the graphics device for the specified Game.
		GraphicsDeviceManager(sptr<Game> const& game);

	public:
		//Specifies the default minimum back-buffer width.
		static constexpr int DefaultBackBufferWidth = 800;
		//Specifies the default minimum back-buffer height.
		static constexpr int DefaultBackBufferHeight = 480;

	public:
		//Gets the GraphicsDevice associated with the GraphicsDeviceManager.
		sptr<GraphicsDevice> GetGraphicsDevice() override {
			return device;
		}

		//Gets or sets the graphics profile, which determines the graphics feature set.
		constexpr GraphicsProfile PreferredGraphicsProfile() const {
			return graphicsProfile;
		}

		//Gets or sets the graphics profile, which determines the graphics feature set.
		constexpr void PreferredGraphicsProfile(xna::GraphicsProfile value) {
			graphicsProfile = value;
			isDeviceDirty = true;
		}

		//Gets or sets a value that indicates whether the device should start in full-screen mode. 
		constexpr  bool IsFullScreen() const {
			return isFullScreen;
		}

		//Gets or sets a value that indicates whether the device should start in full-screen mode. 
		constexpr void IsFullScreen(bool value) {
			isFullScreen = value;
			isDeviceDirty = true;
		}

		//Gets or sets the format of the back buffer.
		constexpr SurfaceFormat PreferredBackBufferFormat() const {
			return backBufferFormat;
		}

		//Gets or sets the format of the back buffer.
		constexpr void PreferredBackBufferFormat(SurfaceFormat value) {
			backBufferFormat = value;
			isDeviceDirty = true;
		}

		//Gets or sets the preferred back-buffer height.
		constexpr Int PreferredBackBufferHeight() const {
			return backBufferHeight;
		}

		//Gets or sets the preferred back-buffer height.
		constexpr void PreferredBackBufferHeight(Int value) {
			backBufferHeight = value;
			isDeviceDirty = true;
		}

		//Gets or sets the preferred back-buffer width.
		constexpr Int PreferredBackBufferWidth() const {
			return backBufferWidth;
		}

		//Gets or sets the preferred back-buffer width.
		constexpr void PreferredBackBufferWidth(Int value) {
			backBufferWidth = value;
			isDeviceDirty = true;
		}

		//Gets or sets the format of the depth stencil.
		constexpr DepthFormat PreferredDepthStencilFormat() const {
			return depthStencilFormat;
		}

		//Gets or sets the format of the depth stencil.
		constexpr void PreferredDepthStencilFormat(DepthFormat value) {
			depthStencilFormat = value;
			isDeviceDirty = true;
		}

		//Gets or sets the display orientations that are available if automatic rotation and scaling is enabled.
		constexpr DisplayOrientation SupportedOrientations() const {
			return supportedOrientations;
		}

		//Gets or sets the display orientations that are available if automatic rotation and scaling is enabled.
		constexpr void SupportedOrientations(DisplayOrientation value) {
			supportedOrientations = value;
			isDeviceDirty = true;
		}

		//Gets or sets a value that indicates whether to sync to the vertical trace (vsync) when presenting the back buffer. 
		constexpr bool SynchronizeWithVerticalRetrace() const {
			return synchronizeWithVerticalRetrace;
		}

		// Gets or sets a value that indicates whether to sync to the vertical trace(vsync) when presenting the back buffer.
		constexpr void SynchronizeWithVerticalRetrace(bool value) {
			synchronizeWithVerticalRetrace = value;
			isDeviceDirty = true;
		}

	public:
		//Applies any changes to device-related properties, changing the graphics device as necessary. 
		void ApplyChanges();
		bool Initialize();
		bool ToggleFullScreen();

	private:
		void ChangeDevice(bool forceCreate){}
		void AddDevice(bool anySuitableDevice, std::vector<sptr<GraphicsDeviceInformation>>& foundDevices);

	protected:
		void CreateDevice();
		void ChangeDevice();

	private:
		bool BeginDraw() override { return false; }
		void EndDraw() override{ }

	private:
		sptr<Game> game = nullptr;
		bool isDeviceDirty{ false };
		sptr<GraphicsDevice> device = nullptr;
		GraphicsDeviceInformation _information{};

		bool isFullScreen{ false };
		Int backBufferWidth{ DefaultBackBufferWidth };
		Int backBufferHeight{ DefaultBackBufferHeight };
		GraphicsProfile graphicsProfile;
		DepthFormat depthStencilFormat{ DepthFormat::Depth24 };
		SurfaceFormat backBufferFormat;
		DisplayOrientation supportedOrientations;
		bool synchronizeWithVerticalRetrace{ true };
	};
}

#endif