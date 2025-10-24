#ifndef XNA_FRAMEWORK_GAME_GAMEWINDOW_HPP
#define XNA_FRAMEWORK_GAME_GAMEWINDOW_HPP

#include <string>
#include <memory>
#include "CSharp/Windows/Forms/Screen.hpp"
#include "../Rectangle.hpp"

namespace Xna {
	inline static const int GAMEWINDOW_DEFAULT_CLIENT_WIDTH = 800;
	inline static const int GAMEWINDOW_DEFAULT_CLIENT_HEIGHT = 480;

	class GameWindow {
	public:
		GameWindow();		

		//Gets and sets the title of the system window.
		std::string Title() const;
		//Gets and sets the title of the system window.
		void Title(std::string const& value);
		//Gets the handle to the system window.
		intptr_t Handle() const;
		//The screen dimensions of the game window's client rectangle.
		Rectangle ClientBounds() const;
		//Gets the device name of the screen the window is currently in.
		std::string ScreenDeviceName() const;

		//TODO
		//static CSharp::Screen ScreenFromAdapter(GraphicsAdapter const& adapter);
		static CSharp::Screen ScreenFromHandle(intptr_t windowHandle);
		bool IsWindowMinimized() const;

		inline static constexpr int DefaultClientWidth = GAMEWINDOW_DEFAULT_CLIENT_WIDTH;
		inline static constexpr int DefaultClientHeight = GAMEWINDOW_DEFAULT_CLIENT_HEIGHT;

	public:
		struct Implementation;
		std::shared_ptr<Implementation> implementation = nullptr;
	};
}

#endif