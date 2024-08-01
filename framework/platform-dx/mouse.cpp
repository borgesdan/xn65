#include "xna/xna-dx.hpp"

namespace xna {
	MouseState Mouse::GetState() {
		const auto state = impl->_dxMouse->GetState();
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

	void Mouse::Initialize(intptr_t handle) {
		impl = unew<PlatformImplementation>();
		windowHandle = handle;
	}

	void Mouse::WindowHandle(intptr_t value) {
		auto hwnd = reinterpret_cast<HWND>(value);

		if (!hwnd) return;
		
		impl->_dxMouse->SetWindow(hwnd);
	}
	
	void Mouse::SetPosition(Int x, Int y) {
		SetCursorPos(x, y);
	}
}