#ifndef XNA_INPUT_MOUSE_HPP
#define XNA_INPUT_MOUSE_HPP

#include "../default.hpp"

namespace xna {
	struct MouseState {
		ButtonState LeftButton{ ButtonState::Released };
		ButtonState RightButton{ ButtonState::Released };
		ButtonState MiddleButton{ ButtonState::Released };
		ButtonState XButton1{ ButtonState::Released };
		ButtonState XButton2{ ButtonState::Released };
		int X{ 0 };
		int Y{ 0 };
		int ScroolWheelValue{ 0 };
	};

	//Allows retrieval of position and button clicks from a mouse input device. 
	class Mouse {
	public:
		//Gets the current state of the mouse, including mouse position and buttons pressed. 
		static MouseState GetState();

		static bool IsConnected();
		static bool IsVisible();
		static void IsVisible(bool value);
		static void ResetScrollWheel();		

	private:
		friend class Game;
		static void Initialize();

		Mouse() = default;
		Mouse(Mouse&) = default;
		Mouse(Mouse&&) = default;

	public:
		struct PlatformImplementation;
		inline static uptr<PlatformImplementation> impl = nullptr;		
	};
}

#endif