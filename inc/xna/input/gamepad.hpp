#ifndef XNA_INPUT_GAMEPAD_HPP
#define XNA_INPUT_GAMEPAD_HPP

#include "../default.hpp"
#include "../common/numerics.hpp"

namespace xna {
	struct GamePadTriggers {
		constexpr GamePadTriggers() = default;

		constexpr GamePadTriggers(float left, float right) : _left(left), _right(right) {
			clamp();
		}		

		constexpr float Left() const {
			return _left;
		}

		constexpr float Right() const {
			return _right;
		}

		constexpr bool operator==(const GamePadTriggers& other) const {
			return _left == other._left && _right == other._right;
		}

	public:
		float _left{ 0 };
		float _right{ 0 };

	private:
		void constexpr clamp() {
			_left = _left < 1.0F ? _left : 1.0F;
			_left = _left > 0.0F ? _left : 0.0F;
			_right = _right < 1.0F ? _right : 1.0F;
			_right = _right > 0.0F ? _right : 0.0F;
		}
	};

	struct GamePadThumbSticks {
		constexpr GamePadThumbSticks() = default;

		constexpr GamePadThumbSticks(Vector2 left, Vector2 right) : _left(left), _right(right) {
			clamp();
		}		

		constexpr Vector2 Left() const {
			return _left;
		}

		constexpr Vector2 Right() const {
			return _right;
		}

		constexpr bool operator==(const GamePadThumbSticks& other) const {
			return _left == other._left && _right == other._right;
		}

	private:
		Vector2 _left{};
		Vector2 _right{};

	private:
		constexpr void clamp() {
			_left = Vector2::Min(_left, Vector2::One());
			_left = Vector2::Max(_left, -Vector2::One());
			_right = Vector2::Min(_right, Vector2::One());
			_right = Vector2::Max(_right, -Vector2::One());
		}
	};

	struct GamePadDPad {
		constexpr GamePadDPad() = default;

		constexpr GamePadDPad(const ButtonState& up, const ButtonState& down,
			const ButtonState& left, const ButtonState& right)
			: Up(up), Right(right), Down(down), Left(left) {
		}				
	
		ButtonState Up{};
		ButtonState Right{};
		ButtonState Down{};
		ButtonState Left{};
	};	

	struct GamePadButtons {
		constexpr GamePadButtons() = default;

		constexpr GamePadButtons(
			ButtonState a,
			ButtonState b,
			ButtonState x,
			ButtonState y,
			ButtonState leftStick,
			ButtonState rightStick,
			ButtonState leftShoulder,
			ButtonState rightShoulder,
			ButtonState back,
			ButtonState start,
			ButtonState bigButton) {
			A = a;
			B = b;
			X = x;
			Y = y;
			LeftStick = leftStick;
			RightStick = rightStick;
			LeftShoulder = leftShoulder;
			RightShoulder = rightShoulder;
			Back = back;
			Start = start;
			BigButton = bigButton;
		}

		constexpr GamePadButtons(Buttons buttons) {
			A = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::A))) == Buttons::A ? ButtonState::Pressed : ButtonState::Released;
			B = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::B))) == Buttons::B ? ButtonState::Pressed : ButtonState::Released;
			X = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::X))) == Buttons::X ? ButtonState::Pressed : ButtonState::Released;
			Y = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::Y))) == Buttons::Y ? ButtonState::Pressed : ButtonState::Released;
			Start = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::Start))) == Buttons::Start ? ButtonState::Pressed : ButtonState::Released;
			Back = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::Back))) == Buttons::Back ? ButtonState::Pressed : ButtonState::Released;
			LeftStick = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::LeftStick))) == Buttons::LeftStick ? ButtonState::Pressed : ButtonState::Released;
			RightStick = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::RightStick))) == Buttons::RightStick ? ButtonState::Pressed : ButtonState::Released;
			LeftShoulder = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::LeftShoulder))) == Buttons::LeftShoulder ? ButtonState::Pressed : ButtonState::Released;
			RightShoulder = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::RightShoulder))) == Buttons::RightShoulder ? ButtonState::Pressed : ButtonState::Released;
			BigButton = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::BigButton))) == Buttons::BigButton ? ButtonState::Pressed : ButtonState::Released;
		}			

		ButtonState A{};
		ButtonState B{};
		ButtonState X{};
		ButtonState Y{};
		ButtonState LeftStick{};
		ButtonState RightStick{};
		ButtonState LeftShoulder{};
		ButtonState RightShoulder{};
		ButtonState Back{};
		ButtonState Start{};
		ButtonState BigButton{};
	};

#ifdef USING_GAMEINPUT
	using GamePadId = APP_LOCAL_DEVICE_ID;
#elif defined(USING_WINDOWS_GAMING_INPUT)
	using GamePadId = std::wstring;
#else
	using GamePadId = uint64_t;
#endif

	struct GamePadCapabilities {
		constexpr GamePadCapabilities() = default;		
	
		GamePadCapabilitiesType Type{};
		bool Connected{ false };
		Ushort Vid{ 0 };
		Ushort Pid{ 0 };
		GamePadId Id{};
	};

	struct GamePadState {
		constexpr GamePadState() = default;			

		constexpr bool IsButtonDown(xna::Buttons button) const {
			switch (button)
			{
			case xna::Buttons::A:
				return this->Buttons.A == ButtonState::Pressed;
			case xna::Buttons::B:
				return this->Buttons.B == ButtonState::Pressed;
			case xna::Buttons::X:
				return this->Buttons.X == ButtonState::Pressed;
			case xna::Buttons::Y:
				return this->Buttons.Y == ButtonState::Pressed;
			case xna::Buttons::Back:
				return this->Buttons.Back == ButtonState::Pressed;
			case xna::Buttons::Start:
				return this->Buttons.Start == ButtonState::Pressed;
			case xna::Buttons::DPadUp:
				return this->Dpad.Up == ButtonState::Pressed;
			case xna::Buttons::DPadDown:
				return this->Dpad.Down == ButtonState::Pressed;
			case xna::Buttons::DPadLeft:
				return this->Dpad.Left == ButtonState::Pressed;
			case xna::Buttons::DPadRight:
				return this->Dpad.Right == ButtonState::Pressed;
			case xna::Buttons::LeftShoulder:
				return this->Buttons.LeftShoulder == ButtonState::Pressed;
			case xna::Buttons::RightShoulder:
				return this->Buttons.RightShoulder == ButtonState::Pressed;
			case xna::Buttons::LeftStick:
				return this->Buttons.LeftStick == ButtonState::Pressed;
			case xna::Buttons::RightStick:
				return this->Buttons.RightStick == ButtonState::Pressed;
			case xna::Buttons::BigButton:
				return this->Buttons.BigButton == ButtonState::Pressed;
			case xna::Buttons::LeftThumbstickLeft:
				return this->ThumbSticks.Left().X < 0.5F;
			case xna::Buttons::LeftThumbstickRight:
				return this->ThumbSticks.Left().X > 0.5F;
			case xna::Buttons::LeftThumbstickDown:
				return this->ThumbSticks.Left().Y > 0.5F;
			case xna::Buttons::LeftThumbstickUp:
				return this->ThumbSticks.Left().Y < 0.5F;
			case xna::Buttons::RightThumbstickLeft:
				return this->ThumbSticks.Right().X < 0.5F;
			case xna::Buttons::RightThumbstickRight:
				return this->ThumbSticks.Right().X > 0.5F;
			case xna::Buttons::RightThumbstickDown:
				return this->ThumbSticks.Right().Y > 0.5F;
			case xna::Buttons::RightThumbstickUp:
				return this->ThumbSticks.Right().Y < 0.5F;
			case xna::Buttons::LeftTrigger:
				return this->Triggers.Left() > 0.5F;
			case xna::Buttons::RightTrigger:
				return this->Triggers.Right() > 0.5F;
			default:
				return false;
			}
		}

		constexpr bool IsButtonUp(xna::Buttons button) const {
			return !IsButtonDown(button);
		}

		GamePadButtons Buttons{};
		GamePadDPad Dpad{};
		bool IsConnected{false};
		Ulong PackedNumber{0};
		GamePadThumbSticks ThumbSticks{};
		GamePadTriggers Triggers{};
	};

	class GamePad {
	public:		
		static GamePadState GetState(PlayerIndex index);
		static GamePadState GetState(PlayerIndex index, GamePadDeadZone deadZone);
		static GamePadCapabilities GetCapabilities(PlayerIndex index);
		static bool SetVibration(PlayerIndex index, float leftMotor, float rightMotor, float leftTrigger = 0, float rightTrigger = 0);
		static void Initialize();

	public:
		struct PlatformImplementation;
		inline static uptr<PlatformImplementation> impl = nullptr;	

	private:
		GamePad();
		GamePad(GamePad&&);
		GamePad(GamePad&);
	};
}

#endif