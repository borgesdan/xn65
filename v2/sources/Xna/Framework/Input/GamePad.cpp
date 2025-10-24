#include "CSharp/Exception.hpp"
#include "Xna/Framework/Input/GamePad.hpp"
#include "InputImplementations.hpp"

using CSharp::InvalidOperationException;
using std::make_shared;
using std::make_unique;
using std::unique_ptr;

namespace Xna {
	const std::string SR_NULL_IMPLEMENTATION = "The internal gamepad handler is null.";	

	std::unique_ptr<GamePad::Implementation> GamePad::implementation = make_unique<GamePad::Implementation>();		

	GamePadState GamePad::GetState(PlayerIndex playerIndex, GamePadDeadZone deadZone) {
		if (!implementation)
			throw InvalidOperationException(SR_NULL_IMPLEMENTATION);

		const auto stateDeadZone = static_cast<DirectX::GamePad::DeadZone>(deadZone);
		
		const auto state = implementation->gamePad.GetState(static_cast<int>(playerIndex), stateDeadZone);

		const auto buttons = GamePadButtons(
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

		const auto dpad = GamePadDPad(
			static_cast<ButtonState>(state.dpad.up),
			static_cast<ButtonState>(state.dpad.right),
			static_cast<ButtonState>(state.dpad.down),
			static_cast<ButtonState>(state.dpad.left));

		const auto isConnected = state.connected;

		const auto thumbSticks = GamePadThumbSticks(
			Vector2(state.thumbSticks.leftX, state.thumbSticks.leftY),
			Vector2(state.thumbSticks.rightX, state.thumbSticks.rightY));
		const auto triggers = GamePadTriggers(state.triggers.left, state.triggers.right);

		const auto pad = GamePadState(thumbSticks, triggers, buttons, dpad, isConnected);
		return pad;

	}

	GamePadCapabilities GamePad::GetCapabilities(PlayerIndex playerIndex) {
		if (!implementation)
			throw InvalidOperationException(SR_NULL_IMPLEMENTATION);		

		const auto index = static_cast<int>(playerIndex);
		auto capabilities = implementation->gamePad.GetCapabilities(index);

		const auto gamePadType = static_cast<GamePadCapabilitiesType>(capabilities.gamepadType);
		const auto cap = GamePadCapabilities(capabilities.connected, gamePadType);

		return cap;
	}

	bool GamePad::SetVibration(PlayerIndex index, float leftMotor, float rightMotor, float leftTrigger, float rightTrigger) {
		if (!implementation)
			throw InvalidOperationException(SR_NULL_IMPLEMENTATION);

		return implementation->gamePad.SetVibration(static_cast<int>(index), leftMotor, rightMotor, leftTrigger, rightTrigger);
	}
}