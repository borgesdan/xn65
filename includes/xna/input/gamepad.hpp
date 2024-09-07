#ifndef XNA_INPUT_GAMEPAD_HPP
#define XNA_INPUT_GAMEPAD_HPP

#include "../default.hpp"
#include "input-enums.hpp"
#include "../common/numerics.hpp"

namespace xna {
	//Structure that defines the position of the left and right triggers on an Xbox Controller.
	struct GamePadTriggers {
		constexpr GamePadTriggers() = default;

		constexpr GamePadTriggers(float left, float right) 
			: _left(left), _right(right) {
			clamp();
		}		

		//Identifies the position of the left trigger on the Xbox Controller.
		constexpr float Left() const {
			return _left;
		}

		//Identifies the position of the right trigger on the Xbox Controller.
		constexpr float Right() const {
			return _right;
		}

		constexpr bool operator==(const GamePadTriggers& other) const {
			return _left == other._left && _right == other._right;
		}

	private:
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

	//Structure that represents the position of left and right sticks (thumbsticks) on an Xbox Controller.
	struct GamePadThumbSticks {
		constexpr GamePadThumbSticks() = default;

		constexpr GamePadThumbSticks(Vector2 left, Vector2 right) : _left(left), _right(right) {
			clamp();
		}		

		//Returns the position of the left Xbox Controller stick (thumbstick) as a 2D vector.
		constexpr Vector2 Left() const {
			return _left;
		}

		//Returns the position of the right Xbox Controller stick (thumbstick) as a 2D vector.
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

	//Identifies which directions on the directional pad of an Xbox Controller are being pressed.
	struct GamePadDPad {
		constexpr GamePadDPad() = default;

		constexpr GamePadDPad(const ButtonState& up, const ButtonState& down,
			const ButtonState& left, const ButtonState& right) : up(up), down(down), left(left), right(right) {}

		//Identifies whether the Up direction on the Xbox Controller directional pad is pressed.
		constexpr ButtonState Up() const { return up; }
		//Identifies whether the Right direction on the Xbox Controller directional pad is pressed.
		constexpr ButtonState Right() const { return right; }
		//Identifies whether the Down direction on the Xbox Controller directional pad is pressed.
		constexpr ButtonState Down() const { return down; }
		//Identifies whether the Left direction on the Xbox Controller directional pad is pressed.
		constexpr ButtonState Left() const { return left; }

	private:
		ButtonState up{};
		ButtonState right{};
		ButtonState down{};
		ButtonState left{};
	};	

	//Identifies whether buttons on an Xbox Controller are pressed or released. 
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
			this->a = a;
			this->b = b;
			this->x = x;
			this->y = y;
			this->leftStick = leftStick;
			this->rightStick = rightStick;
			this->leftShoulder = leftShoulder;
			this->rightShoulder = rightShoulder;
			this->back = back;
			this->start = start;
			this->bigButton = bigButton;
		}

		constexpr GamePadButtons(Buttons buttons) {
			a = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::A))) == Buttons::A ? ButtonState::Pressed : ButtonState::Released;
			b = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::B))) == Buttons::B ? ButtonState::Pressed : ButtonState::Released;
			x = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::X))) == Buttons::X ? ButtonState::Pressed : ButtonState::Released;
			y = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::Y))) == Buttons::Y ? ButtonState::Pressed : ButtonState::Released;
			start = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::Start))) == Buttons::Start ? ButtonState::Pressed : ButtonState::Released;
			back = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::Back))) == Buttons::Back ? ButtonState::Pressed : ButtonState::Released;
			leftStick = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::LeftStick))) == Buttons::LeftStick ? ButtonState::Pressed : ButtonState::Released;
			rightStick = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::RightStick))) == Buttons::RightStick ? ButtonState::Pressed : ButtonState::Released;
			leftShoulder = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::LeftShoulder))) == Buttons::LeftShoulder ? ButtonState::Pressed : ButtonState::Released;
			rightShoulder = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::RightShoulder))) == Buttons::RightShoulder ? ButtonState::Pressed : ButtonState::Released;
			bigButton = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::BigButton))) == Buttons::BigButton ? ButtonState::Pressed : ButtonState::Released;
		}			

		//Identifies if the A button on the Xbox Controller is pressed. 
		constexpr ButtonState A() const { return a; }
		//Identifies if the B button on the Xbox Controller is pressed.
		constexpr ButtonState B() const { return b; }
		//Identifies if the X button on the Xbox Controller is pressed. 
		constexpr ButtonState X() const { return x; }
		//Identifies if the Y button on the Xbox Controller is pressed. 
		constexpr ButtonState Y() const { return y; }
		//Identifies if the left stick button on the Xbox Controller is pressed (the stick is "clicked" in
		constexpr ButtonState LeftStick() const { return leftStick; }
		//Identifies if the right stick button on the Xbox Controller is pressed (the stick is "clicked" in).
		constexpr ButtonState RightStick() const { return rightStick; }
		//Identifies if the left shoulder (bumper) button on the Xbox Controller is pressed.
		constexpr ButtonState LeftShoulder() const { return leftShoulder; }
		//Identifies if the right shoulder (bumper) button on the Xbox Controller is pressed.
		constexpr ButtonState RightShoulder() const { return rightShoulder; }
		//Identifies if the BACK button on the Xbox Controller is pressed. 
		constexpr ButtonState Back() const { return back; }
		//Identifies if the START button on the Xbox Controller is pressed. 
		constexpr ButtonState Start() const { return start; }
		//Identifies if the BigButton button is pressed. 
		constexpr ButtonState BigButton() const { return bigButton; }

	private:
		ButtonState a{};
		ButtonState b{};
		ButtonState x{};
		ButtonState y{};
		ButtonState leftStick{};
		ButtonState rightStick{};
		ButtonState leftShoulder{};
		ButtonState rightShoulder{};
		ButtonState back{};
		ButtonState start{};
		ButtonState bigButton{};
	};

	//Describes the capabilities of an Xbox Controller, including controller type, and identifies if the controller supports voice. 
	struct GamePadCapabilities {
		constexpr GamePadCapabilities() = default;	

		constexpr GamePadCapabilities(bool isConnected, GamePadCapabilitiesType type) : isConnected(isConnected), type(type){}
	
		//Indicates whether the Xbox Controller is connected.
		constexpr bool IsConnected() const { return isConnected; }
		//Gets the type of controller.
		constexpr GamePadCapabilitiesType GamePadType() const { return type; }		

	private:
		GamePadCapabilitiesType type{};
		bool isConnected{ false };
	};

	//Represents specific information about the state of an Xbox Controller, including the current state of buttons and sticks.
	struct GamePadState {
		constexpr GamePadState() = default;	

		constexpr GamePadState(
			GamePadThumbSticks sticks,
			GamePadTriggers triggers,
			GamePadButtons buttons,
			GamePadDPad dpad, bool isConnected
		) : buttons(buttons), dpad(dpad), triggers(triggers), thumbSticks(sticks), isConnected(isConnected){}

		//Determines whether specified input device buttons are pressed in this GamePadState.
		constexpr bool IsButtonDown(xna::Buttons button) const {
			switch (button)
			{
			case xna::Buttons::A:
				return this->Buttons().A() == ButtonState::Pressed;
			case xna::Buttons::B:
				return this->Buttons().B() == ButtonState::Pressed;
			case xna::Buttons::X:
				return this->Buttons().X() == ButtonState::Pressed;
			case xna::Buttons::Y:
				return this->Buttons().Y() == ButtonState::Pressed;
			case xna::Buttons::Back:
				return this->Buttons().Back() == ButtonState::Pressed;
			case xna::Buttons::Start:
				return this->Buttons().Start() == ButtonState::Pressed;
			case xna::Buttons::DPadUp:
				return this->Dpad().Up() == ButtonState::Pressed;
			case xna::Buttons::DPadDown:
				return this->Dpad().Down() == ButtonState::Pressed;
			case xna::Buttons::DPadLeft:
				return this->Dpad().Left() == ButtonState::Pressed;
			case xna::Buttons::DPadRight:
				return this->Dpad().Right() == ButtonState::Pressed;
			case xna::Buttons::LeftShoulder:
				return this->Buttons().LeftShoulder() == ButtonState::Pressed;
			case xna::Buttons::RightShoulder:
				return this->Buttons().RightShoulder() == ButtonState::Pressed;
			case xna::Buttons::LeftStick:
				return this->Buttons().LeftStick() == ButtonState::Pressed;
			case xna::Buttons::RightStick:
				return this->Buttons().RightStick() == ButtonState::Pressed;
			case xna::Buttons::BigButton:
				return this->Buttons().BigButton() == ButtonState::Pressed;
			case xna::Buttons::LeftThumbstickLeft:
				return this->ThumbSticks().Left().X < 0.5F;
			case xna::Buttons::LeftThumbstickRight:
				return this->ThumbSticks().Left().X > 0.5F;
			case xna::Buttons::LeftThumbstickDown:
				return this->ThumbSticks().Left().Y > 0.5F;
			case xna::Buttons::LeftThumbstickUp:
				return this->ThumbSticks().Left().Y < 0.5F;
			case xna::Buttons::RightThumbstickLeft:
				return this->ThumbSticks().Right().X < 0.5F;
			case xna::Buttons::RightThumbstickRight:
				return this->ThumbSticks().Right().X > 0.5F;
			case xna::Buttons::RightThumbstickDown:
				return this->ThumbSticks().Right().Y > 0.5F;
			case xna::Buttons::RightThumbstickUp:
				return this->ThumbSticks().Right().Y < 0.5F;
			case xna::Buttons::LeftTrigger:
				return this->Triggers().Left() > 0.5F;
			case xna::Buttons::RightTrigger:
				return this->Triggers().Right() > 0.5F;
			default:
				return false;
			}
		}
		
		//Determines whether specified input device buttons are up (not pressed) in this GamePadState.
		constexpr bool IsButtonUp(xna::Buttons button) const {
			return !IsButtonDown(button);
		}

		//Returns a structure that identifies what buttons on the Xbox controller are pressed.
		constexpr GamePadButtons Buttons() const { return buttons; }
		//Returns a structure that identifies what directions of the directional pad on the Xbox Controller are pressed.
		constexpr GamePadDPad Dpad() const { return dpad; }
		//Indicates whether the Xbox Controller is connected.
		constexpr bool IsConnected() const { return isConnected; }
		//Returns a structure that indicates the position of the Xbox Controller sticks (thumbsticks).
		constexpr GamePadThumbSticks ThumbSticks() const { return thumbSticks; }
		//Returns a structure that identifies the position of triggers on the Xbox controller.
		constexpr GamePadTriggers Triggers() const { return triggers; }

	private:
		GamePadButtons buttons{};
		GamePadDPad dpad{};
		bool isConnected{ false };
		GamePadThumbSticks thumbSticks{};
		GamePadTriggers triggers{};
	};

	//Allows retrieval of user interaction with an Xbox Controller and setting of controller vibration motors.
	class GamePad {
	public:		
		//Gets the current state of a game pad controller. As an option, it specifies a dead zone processing method for the analog sticks.
		static GamePadState GetState(PlayerIndex index);
		//Gets the current state of a game pad controller. As an option, it specifies a dead zone processing method for the analog sticks.
		static GamePadState GetState(PlayerIndex index, GamePadDeadZone deadZone);

		//Retrieves the capabilities of an Xbox Controller.
		static GamePadCapabilities GetCapabilities(PlayerIndex index);
		//Sets the vibration motor speeds on an Xbox Controller.
		static bool SetVibration(PlayerIndex index, float leftMotor, float rightMotor, float leftTrigger = 0, float rightTrigger = 0);

	private:
		friend class Game;
		static void Initialize();

		GamePad() = default;
		GamePad(GamePad&) = default;
		GamePad(GamePad&&) = default;

	public:
		struct PlatformImplementation;
		inline static uptr<PlatformImplementation> impl = nullptr;		
	};
}

#endif