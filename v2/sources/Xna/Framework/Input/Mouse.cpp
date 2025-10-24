#include "Xna/Framework/Input/Mouse.hpp"
#include "CSharp/Exception.hpp"
#include "InputImplementations.hpp"

using std::unique_ptr;
using std::make_unique;
using CSharp::InvalidOperationException;

namespace Xna {
	const std::string SR_NULL_IMPLEMENTATION = "The internal mouse handler is null.";	

	unique_ptr<Mouse::Implementation> Mouse::implementation = make_unique<Mouse::Implementation>();

	MouseState Mouse::GetState() {
		if(!implementation)
			throw InvalidOperationException(SR_NULL_IMPLEMENTATION);

		const auto state = implementation->mouse.GetState();
		MouseState mstate;
		mstate.LeftButton = static_cast<ButtonState>(state.leftButton);
		mstate.RightButton = static_cast<ButtonState>(state.rightButton);
		mstate.MiddleButton = static_cast<ButtonState>(state.middleButton);
		mstate.XButton1 = static_cast<ButtonState>(state.xButton1);
		mstate.XButton2 = static_cast<ButtonState>(state.xButton2);
		mstate.X = state.x;
		mstate.Y = state.y;
		mstate.ScroolWheelValue = state.scrollWheelValue;

		return mstate;
	}	

	void Mouse::WindowHandle(intptr_t value) {
		if (!implementation)
			throw InvalidOperationException(SR_NULL_IMPLEMENTATION);

		auto hwnd = reinterpret_cast<HWND>(value);

		if (!hwnd) return;

		implementation->mouse.SetWindow(hwnd);
	}

	void Mouse::SetPosition(int32_t x, int32_t y) {
		SetCursorPos(x, y);
	}
}