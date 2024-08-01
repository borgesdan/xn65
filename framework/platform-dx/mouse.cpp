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

	bool Mouse::IsConnected() {		
		return impl->_dxMouse->IsConnected();
	}

	bool Mouse::IsVisible() {
		return impl->_dxMouse->IsVisible();
	}

	void Mouse::IsVisible(bool value) {
		impl->_dxMouse->SetVisible(value);
	}

	void Mouse::ResetScrollWheel() {
		impl->_dxMouse->ResetScrollWheelValue();
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
}