#ifndef XNA_PLATFORM_MOUSE_DX_HPP
#define XNA_PLATFORM_MOUSE_DX_HPP

#include "../input/mouse.hpp"
#include <Mouse.h>

namespace xna {
	struct MouseState : public IMouseState {
		constexpr MouseState() = default;

		constexpr MouseState(DirectX::Mouse::State const& dxMouseState) {
			LeftButton = static_cast<ButtonState>(dxMouseState.leftButton);
			RightButton = static_cast<ButtonState>(dxMouseState.rightButton);
			MiddleButton = static_cast<ButtonState>(dxMouseState.middleButton);
			XButton1 = static_cast<ButtonState>(dxMouseState.xButton1);
			XButton2 = static_cast<ButtonState>(dxMouseState.xButton2);
			X = dxMouseState.x;
			Y = dxMouseState.y;
			ScroolWheelValue = dxMouseState.scrollWheelValue;
		}				
	};

	struct Mouse : public IMouse {
		inline static void Initialize() {
			_dxMouse = uNew<DirectX::Mouse>();
		}

	public:
		inline static uptr<DirectX::Mouse> _dxMouse = nullptr;

	private:
		constexpr Mouse() = default;
		constexpr Mouse(Mouse&&) = default;
		constexpr Mouse(const Mouse&) = default;
	};

	inline MouseState IMouse::GetState() {
		if (!Mouse::_dxMouse)
			return MouseState();

		const auto state = Mouse::_dxMouse->GetState();
		return MouseState(state);
	}

	inline bool IMouse::IsConnected() {
		if (!Mouse::_dxMouse)
			return false;

		return Mouse::_dxMouse->IsConnected();
	}

	inline bool IMouse::IsVisible() {
		if (!Mouse::_dxMouse)
			return false;

		return Mouse::_dxMouse->IsVisible();
	}

	inline void IMouse::IsVisible(bool value) {
		if (!Mouse::_dxMouse)
			return;

		Mouse::_dxMouse->SetVisible(value);
	}

	inline void IMouse::ResetScrollWheel() {
		if (!Mouse::_dxMouse)
			return;

		Mouse::_dxMouse->ResetScrollWheelValue();
	}
}

#endif