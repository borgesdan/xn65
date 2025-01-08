#ifndef XNA_GAME_GRAPHICSDEVICEMANAGER_HPP
#define XNA_GAME_GRAPHICSDEVICEMANAGER_HPP

#include "../default.hpp"
#include "../graphics/shared.hpp"
#include "gdeviceinfo.hpp"

namespace xna {
	struct IGraphicsDeviceService {
		virtual sptr<GraphicsDevice> GetGraphicsDevice() = 0;

		//EventHandler<EventArgs> DeviceDisposing;
		//EventHandler<EventArgs> DeviceReset;
		//EventHandler<EventArgs> DeviceResetting;
		//EventHandler<EventArgs> DeviceCreated;
	};

	class IGraphicsDeviceManager {
		virtual void CreateDevice() = 0;
		//virtual bool BeginDraw() = 0;
		//virtual void EndDraw() = 0;
	};

	//Handles the configuration and management of the graphics device. 
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

		//Gets or sets a value that indicates whether to enable a multisampled back buffer.
		constexpr bool PreferMultiSampling() const {
			return allowMultiSampling;
		}

		//Gets or sets a value that indicates whether to enable a multisampled back buffer.
		constexpr void PreferMultiSampling(bool value) {
			allowMultiSampling = value;
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
		//Toggles between full screen and windowed mode.
		bool ToggleFullScreen();	

	protected:	
		inline virtual void RankDevices(std::vector<sptr<GraphicsDeviceInformation>>& foundDevices) { RankDevicesPlatform(foundDevices);	}
		inline virtual sptr<GraphicsDeviceInformation> FindBestDevice(bool anySuitableDevice) { return FindBestPlatformDevice(anySuitableDevice); }
		virtual bool CanResetDevice(GraphicsDeviceInformation& newDeviceInfo);

	private:
		inline void CreateDevice() { ChangeDevice(true); }
		void ChangeDevice(bool forceCreate);
		void AddDevices(bool anySuitableDevice, std::vector<sptr<GraphicsDeviceInformation>>& foundDevices);
		void AddDevices(GraphicsAdapter const& adapter, DisplayMode const& mode, sptr<GraphicsDeviceInformation>& baseDeviceInfo, std::vector<sptr<GraphicsDeviceInformation>>& foundDevices) const;		
		sptr<GraphicsDeviceInformation> FindBestPlatformDevice(bool anySuitableDevice);
		void RankDevicesPlatform(std::vector<sptr<GraphicsDeviceInformation>>& foundDevices);
		void CreateDevice(GraphicsDeviceInformation& newInfo);		
		void MassagePresentParameters(PresentationParameters& pp);
		void ValidateGraphicsDeviceInformation(GraphicsDeviceInformation& devInfo);

	private:
		sptr<Game> game = nullptr;
		bool isDeviceDirty{ false };
		sptr<GraphicsDevice> device = nullptr;
		GraphicsDeviceInformation information{};
		bool isFullScreen{ false };
		Int backBufferWidth{ DefaultBackBufferWidth };
		Int backBufferHeight{ DefaultBackBufferHeight };
		GraphicsProfile graphicsProfile{GraphicsProfile::HiDef};
		DepthFormat depthStencilFormat{ DepthFormat::Depth24 };
		SurfaceFormat backBufferFormat{SurfaceFormat::Color};
		DisplayOrientation supportedOrientations{DisplayOrientation::Default};
		bool synchronizeWithVerticalRetrace{ true };
		bool useResizedBackBuffer{ false };
		Int resizedBackBufferWidth{ 0 };
		Int resizedBackBufferHeight{ 0 };
		bool allowMultiSampling{ false };
		bool inDeviceTransition{ false };
		bool isReallyFullScreen{ false };
		DisplayOrientation currentWindowOrientation{ DisplayOrientation::Default };
		std::vector<sptr<GraphicsDeviceInformation>> foundDevices;
	};
}

#endif