#ifndef XNA_INPUT_GAMEPAD_HPP
#define XNA_INPUT_GAMEPAD_HPP

#include "../default.hpp"
#include "../common/vectors.hpp"

namespace xna {
	struct IGamePadTriggers {
	public:
		virtual float Left() const = 0;
		virtual float Right() const = 0;
	};

	struct IGamePadThumbSticks {
	public:
		virtual Vector2 Left() const = 0;
		virtual Vector2 Right() const = 0;
	};

	struct IGamePadDPad {
		virtual ButtonState Up() const = 0;
		virtual ButtonState Down() const = 0;
		virtual ButtonState Right() const = 0;
		virtual ButtonState Left() const = 0;
	};	

	struct IGamePadButtons {
		virtual ButtonState A() const = 0;
		virtual ButtonState B() const = 0;
		virtual ButtonState Back() const = 0;
		virtual ButtonState X() const = 0;
		virtual ButtonState Y() const = 0;
		virtual ButtonState Start() const = 0;
		virtual ButtonState LeftShoulder() const = 0;
		virtual ButtonState LeftStick() const = 0;
		virtual ButtonState RightShoulder() const = 0;
		virtual ButtonState RightStick() const = 0;
		virtual ButtonState BigButton() const = 0;
	};

	struct IGamePadCapabilities {
		virtual GamePadCapabilitiesType GamePadType() const = 0;
		virtual bool IsConnected() const = 0;
		virtual String Id() const = 0;
		virtual Ushort Vid() const = 0;
		virtual Ushort Pid() const = 0;
	};

	struct IGamePadState {
		virtual GamePadButtons Buttons() const = 0;
		virtual GamePadDPad DPad() const = 0;
		virtual bool IsConnected() const = 0;
		virtual Ulong PacketNumber() const = 0;
		virtual GamePadThumbSticks ThumbSticks() = 0;
		virtual GamePadTriggers Triggers() = 0;
		virtual bool IsButtonDown(xna::Buttons button) = 0;
		virtual bool IsButtonUp(xna::Buttons button) = 0;
	};

	class IGamePad {
	public:
		virtual ~IGamePad(){}

		static GamePadState GetState(PlayerIndex index);
		static GamePadState GetState(PlayerIndex index, GamePadDeadZone deadZone);
		static GamePadCapabilities GetCapabilities(PlayerIndex index);
		static bool SetVibration(PlayerIndex index, float leftMotor, float rightMotor, float leftTrigger = 0, float rightTrigger = 0);		
	};
}

#endif