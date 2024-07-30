#ifndef XNA_GAME_WINDOW_HPP
#define XNA_GAME_WINDOW_HPP

#include "../default.hpp"
#include "../common/numerics.hpp"
#include "../csharp/screen.hpp"

namespace xna {
	class GameWindow {
	public:
		GameWindow();

		//Gets the current display orientation, which reflects the physical orientation of the phone in the user's hand. 
		constexpr DisplayOrientation CurrentOrientation() const {
			return currentOrientation;
		}

		//Gets and sets the title of the system window.
		constexpr String Title() const {
			return title;
		}

		//Gets and sets the title of the system window.
		void Title(String const& value);

		//Gets the handle to the system window.
		constexpr intptr_t Handle() const {
			return handle;
		}					

		//The screen dimensions of the game window's client rectangle.
		constexpr Rectangle ClientBounds() const {
			return clientBounds;
		}
		
		//Gets the device name of the screen the window is currently in.
		String ScreenDeviceName() const;

		static uptr<Screen> ScreenFromAdapter(GraphicsAdapter const& adapter);
		static uptr<Screen> ScreenFromHandle(intptr_t windowHandle);
		bool IsWindowMinimized() const;

		inline static constexpr Int DefaultClientWidth = 800;
		inline static constexpr Int DefaultClientHeight = 600;

	private:
		String title;
		intptr_t handle{ 0 };
		Rectangle clientBounds{};
		String screenDeviceName;
		DisplayOrientation currentOrientation{ DisplayOrientation::Default };


	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
}

#endif