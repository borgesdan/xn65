#include "xna/input/mouse.hpp"
#include "xna/platform-dx/dx.hpp"

namespace xna {
	MouseState Mouse::GetState() {
		if (!impl || !impl->_dxMouse)
			return MouseState();

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
		if (!impl || !impl->_dxMouse)
			return false;

		return impl->_dxMouse->IsConnected();
	}

	bool Mouse::IsVisible() {
		if (!impl || !impl->_dxMouse)
			return false;

		return impl->_dxMouse->IsVisible();
	}

	void Mouse::IsVisible(bool value) {
		if (!impl || !impl->_dxMouse)
			return;

		impl->_dxMouse->SetVisible(value);
	}

	void Mouse::ResetScrollWheel() {
		if (!impl || !impl->_dxMouse)
			return;
		
		impl->_dxMouse->ResetScrollWheelValue();
	}

	void Mouse::Initialize() {
		impl = unew<PlatformImplementation>();
	}
}