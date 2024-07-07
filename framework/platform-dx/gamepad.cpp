#include "xna/platform/dx.hpp"
#include "xna/input/gamepad.hpp"

namespace xna {
	void GamePad::Initialize() {
		impl = unew<PlatformImplementation>();	
	}

	GamePadState GamePad::GetState(PlayerIndex index) {
		if (!impl || !impl->_dxGamePad)
			return GamePadState();

		const auto state = impl->_dxGamePad->GetState(
			static_cast<int>(index)
		);

		GamePadState pad;		
		pad.Dpad = GamePadDPad(
			static_cast<ButtonState>(state.dpad.up),
			static_cast<ButtonState>(state.dpad.right),
			static_cast<ButtonState>(state.dpad.down),
			static_cast<ButtonState>(state.dpad.left));
		
		pad.IsConnected = state.connected;
		pad.PackedNumber = state.packet;
		pad.ThumbSticks = GamePadThumbSticks(
			Vector2(state.thumbSticks.leftX, state.thumbSticks.leftY),
			Vector2(state.thumbSticks.rightX, state.thumbSticks.rightY));
		pad.Triggers = GamePadTriggers(state.triggers.left, state.triggers.right);
		pad.Buttons = GamePadButtons(
			static_cast<ButtonState>(state.buttons.a),
			static_cast<ButtonState>(state.buttons.b),
			static_cast<ButtonState>(state.buttons.x),
			static_cast<ButtonState>(state.buttons.y),
			static_cast<ButtonState>(state.buttons.leftStick),
			static_cast<ButtonState>(state.buttons.rightStick),
			static_cast<ButtonState>(state.buttons.leftShoulder),
			static_cast<ButtonState>(state.buttons.rightShoulder),
			static_cast<ButtonState>(state.buttons.back),
			static_cast<ButtonState>(state.buttons.start),
			static_cast<ButtonState>(state.buttons.view));
		
		return pad;
	}

	GamePadState GamePad::GetState(PlayerIndex index, GamePadDeadZone deadZone) {
		if (!impl || !impl->_dxGamePad)
			return GamePadState();

		const auto state = impl->_dxGamePad->GetState(
			static_cast<int>(index),
			static_cast<DirectX::GamePad::DeadZone>(deadZone)
		);

		GamePadState pad;
		pad.Dpad = GamePadDPad(
			static_cast<ButtonState>(state.dpad.up),
			static_cast<ButtonState>(state.dpad.right),
			static_cast<ButtonState>(state.dpad.down),
			static_cast<ButtonState>(state.dpad.left));

		pad.IsConnected = state.connected;
		pad.PackedNumber = state.packet;
		pad.ThumbSticks = GamePadThumbSticks(
			Vector2(state.thumbSticks.leftX, state.thumbSticks.leftY),
			Vector2(state.thumbSticks.rightX, state.thumbSticks.rightY));
		pad.Triggers = GamePadTriggers(state.triggers.left, state.triggers.right);

		return pad;
	}

	GamePadCapabilities GamePad::GetCapabilities(PlayerIndex index) {
		if (!impl || !impl->_dxGamePad)
			return GamePadCapabilities();

		const auto capabilities = impl->_dxGamePad->GetCapabilities(static_cast<int>(index));
		GamePadCapabilities cap;
		cap.Connected = capabilities.connected;
		cap.Id = capabilities.id;
		cap.Pid = capabilities.pid;
		cap.Type = static_cast<GamePadCapabilitiesType>(capabilities.gamepadType);
		cap.Vid = capabilities.vid;
		return cap;
	}

	bool GamePad::SetVibration(PlayerIndex index, float leftMotor, float rightMotor, float leftTrigger, float rightTrigger) {
		if (!impl || !impl->_dxGamePad)
			return false;

		return impl->_dxGamePad->SetVibration(static_cast<int>(index), leftMotor, rightMotor, leftTrigger, rightTrigger);
	}
}