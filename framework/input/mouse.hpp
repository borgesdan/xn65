#ifndef XNA_INPUT_MOUSE_HPP
#define XNA_INPUT_MOUSE_HPP

#include "../default.hpp"

namespace xna {
	struct IMouseState {
		ButtonState LeftButton{ 0 };
		ButtonState RightButton{ 0 };
		ButtonState MiddleButton{ 0 };
		ButtonState XButton1{ 0 };
		ButtonState XButton2{ 0 };
		int X{ 0 };
		int Y{ 0 };
		int ScroolWheelValue{ 0 };
	};

	class IMouse {
	public:
		virtual ~IMouse() {}
		static MouseState GetState();
		static bool IsConnected();
		static bool IsVisible();
		static void IsVisible(bool value);
		static void ResetScrollWheel();
	};
}

#endif