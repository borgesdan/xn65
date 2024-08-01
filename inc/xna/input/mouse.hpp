#ifndef XNA_INPUT_MOUSE_HPP
#define XNA_INPUT_MOUSE_HPP

#include "../default.hpp"

namespace xna {
	//Represents the state of a mouse input device, including mouse cursor position and buttons pressed.
	struct MouseState {
		//Returns the state of the left mouse button.
		ButtonState LeftButton{ ButtonState::Released };
		//Returns the state of the middle mouse button.
		ButtonState MiddleButton{ ButtonState::Released };
		//Returns the state of the right mouse button.
		ButtonState RightButton{ ButtonState::Released };
		//Gets the cumulative mouse scroll wheel value since the game was started.
		int ScroolWheelValue{ 0 };
		//Specifies the horizontal position of the mouse cursor.
		int X{ 0 };
		//Returns the state of XBUTTON1.
		ButtonState XButton1{ ButtonState::Released };
		//Returns the state of XBUTTON2.
		ButtonState XButton2{ ButtonState::Released };
		//Specifies the vertical position of the mouse cursor.
		int Y{ 0 };
	};

	//Allows retrieval of position and button clicks from a mouse input device. 
	class Mouse {
	public:
		//Gets the current state of the mouse, including mouse position and buttons pressed. 
		static MouseState GetState();

		//Gets or sets the window used for mouse processing. 
		//Mouse coordinates returned by GetState are relative to the upper-left corner of this window. 
		constexpr static intptr_t WindowHandle() {
			return windowHandle;
		}

		//Gets or sets the window used for mouse processing. 
		//Mouse coordinates returned by GetState are relative to the upper-left corner of this window. 
		static void WindowHandle(intptr_t value);	

		//Sets the position of the mouse cursor relative to the upper-left corner of the window.
		static void SetPosition(Int x, Int y);

	private:
		friend class Game;
		static void Initialize(intptr_t windowHandle);

		Mouse() = default;
		Mouse(Mouse&) = default;
		Mouse(Mouse&&) = default;

		inline static intptr_t windowHandle = 0;

	public:
		struct PlatformImplementation;
		inline static uptr<PlatformImplementation> impl = nullptr;		
	};
}

#endif