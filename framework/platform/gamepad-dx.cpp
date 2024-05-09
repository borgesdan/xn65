#include "platform-dx/gamepad-dx.hpp"

namespace xna {
	GamePadState _GamePad::GetState(PlayerIndex index) {
		if (!_dxGamePad)
			return GamePadState();

		const auto state = _dxGamePad->GetState(
			static_cast<int>(index)
		);
		return GamePadState(state);
	}

	GamePadState _GamePad::GetState(PlayerIndex index, GamePadDeadZone deadZone) {
		if (!_dxGamePad)
			return GamePadState();

		const auto state = _dxGamePad->GetState(
			static_cast<int>(index),
			static_cast<DirectX::GamePad::DeadZone>(deadZone)
		);
		return GamePadState(state);
	}

	GamePadCapabilities _GamePad::GetCapabilities(PlayerIndex index) {
		if (!_dxGamePad)
			return GamePadCapabilities();

		const auto capabilities = _dxGamePad->GetCapabilities(static_cast<int>(index));
		return GamePadCapabilities(capabilities);
	}

	bool _GamePad::SetVibration(PlayerIndex index, float leftMotor, float rightMotor, float leftTrigger, float rightTrigger) {
		if (!_dxGamePad)
			return false;

		return _dxGamePad->SetVibration(static_cast<int>(index), leftMotor, rightMotor, leftTrigger, rightTrigger);
	}
}