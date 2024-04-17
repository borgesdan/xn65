#ifndef XNA_PLATFORM_GAMEPAD_DX_HPP
#define XNA_PLATFORM_GAMEPAD_DX_HPP

#include "../input/gamepad.hpp"
#include <GamePad.h>

namespace xna {
	struct GamePadTriggers: public IGamePadTriggers {
		constexpr GamePadTriggers() = default;

		constexpr GamePadTriggers(float left, float right) : _left(left), _right(right) {
			clamp();
		}
		
		constexpr GamePadTriggers(DirectX::GamePad::Triggers const& trigger) {
			_left = trigger.left;
			_right = trigger.right;
			clamp();
		}

		virtual constexpr float Left() const override {
			return _left;
		}

		virtual constexpr float Right() const override {
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

	struct GamePadThumbSticks : public IGamePadThumbSticks {
		constexpr GamePadThumbSticks() = default;

		constexpr GamePadThumbSticks(Vector2 left, Vector2 right) : _left(left), _right(right) {
			clamp();
		}

		constexpr GamePadThumbSticks(DirectX::GamePad::ThumbSticks const& sticks) {
			_left = Vector2(sticks.leftX, sticks.leftY);
			_right = Vector2(sticks.rightX, sticks.rightY);
			clamp();
		}

		virtual constexpr Vector2 Left() const override {
			return _left;
		}

		virtual constexpr Vector2 Right() const override {
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

	struct GamePadDPad : public IGamePadDPad {
		constexpr GamePadDPad() = default;

		constexpr GamePadDPad(const ButtonState& up,	const ButtonState& down,
			const ButtonState& left, const ButtonState& right)
			: _up(up), _right(right), _down(down), _left(left) {
		}

		constexpr GamePadDPad(DirectX::GamePad::DPad dpad) {
			_up = static_cast<ButtonState>(dpad.up);
			_right = static_cast<ButtonState>(dpad.right);
			_down = static_cast<ButtonState>(dpad.down);
			_left = static_cast<ButtonState>(dpad.left);
		}

		virtual constexpr ButtonState Up() const override {
			return _up;
		}

		virtual constexpr ButtonState Down() const override {
			return _down;
		}

		virtual constexpr ButtonState Right() const override {
			return _right;
		}

		virtual constexpr ButtonState Left() const override {
			return _left;
		}

	private:
		ButtonState _up{};
		ButtonState _right{};
		ButtonState _down{};
		ButtonState _left{};
	};

	struct GamePadButtons : public IGamePadButtons {
		constexpr GamePadButtons() = default;

		constexpr GamePadButtons(Buttons buttons) {
			_a = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::A))) == Buttons::A ? ButtonState::Pressed : ButtonState::Released;
			_b = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::B))) == Buttons::B ? ButtonState::Pressed : ButtonState::Released;
			_x = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::X))) == Buttons::X ? ButtonState::Pressed : ButtonState::Released;
			_y = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::Y))) == Buttons::Y ? ButtonState::Pressed : ButtonState::Released;
			_start = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::Start))) == Buttons::Start ? ButtonState::Pressed : ButtonState::Released;
			_back = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::Back))) == Buttons::Back ? ButtonState::Pressed : ButtonState::Released;
			_leftStick = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::LeftStick))) == Buttons::LeftStick ? ButtonState::Pressed : ButtonState::Released;
			_rightStick = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::RightStick))) == Buttons::RightStick ? ButtonState::Pressed : ButtonState::Released;
			_leftShoulder = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::LeftShoulder))) == Buttons::LeftShoulder ? ButtonState::Pressed : ButtonState::Released;
			_rightShoulder = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::RightShoulder))) == Buttons::RightShoulder ? ButtonState::Pressed : ButtonState::Released;
			_bigButton = static_cast<Buttons>((static_cast<int>(buttons) & static_cast<int>(Buttons::BigButton))) == Buttons::BigButton ? ButtonState::Pressed : ButtonState::Released;
		}

		constexpr GamePadButtons(DirectX::GamePad::Buttons const& buttons) {
			_a = static_cast<ButtonState>(buttons.a);
			_b = static_cast<ButtonState>(buttons.b);
			_x = static_cast<ButtonState>(buttons.x);
			_y = static_cast<ButtonState>(buttons.y);
			_leftStick = static_cast<ButtonState>(buttons.leftStick);
			_rightStick = static_cast<ButtonState>(buttons.rightStick);
			_leftShoulder = static_cast<ButtonState>(buttons.leftShoulder);
			_rightShoulder = static_cast<ButtonState>(buttons.rightShoulder);
			_back = static_cast<ButtonState>(buttons.back);
			_start = static_cast<ButtonState>(buttons.start);
			_bigButton = static_cast<ButtonState>(buttons.view);
		}

		virtual constexpr ButtonState A() const override {
			return _a;
		}

		virtual constexpr ButtonState B() const override {
			return _b;
		}

		virtual constexpr ButtonState Back() const override{
			return _back;
		}

		virtual constexpr ButtonState X() const override {
			return _x;
		}

		virtual constexpr ButtonState Y() const override {
			return _y;
		}

		virtual constexpr ButtonState Start() const override {
			return _start;
		}

		virtual constexpr ButtonState LeftShoulder() const override {
			return _leftShoulder;
		}

		virtual constexpr ButtonState LeftStick() const override {
			return _leftStick;
		}

		virtual constexpr ButtonState RightShoulder() const override {
			return _rightShoulder;
		}

		virtual constexpr ButtonState RightStick() const override {
			return _rightStick;
		}

		virtual constexpr ButtonState BigButton() const override {
			return _bigButton;
		}

	private:
		ButtonState _a{};
		ButtonState _b{};
		ButtonState _x{};
		ButtonState _y{};
		ButtonState _leftStick{};
		ButtonState _rightStick{};
		ButtonState _leftShoulder{};
		ButtonState _rightShoulder{};
		ButtonState _back{};
		ButtonState _start{};
		ButtonState _bigButton{};
	};

	struct GamePadCapabilities : public IGamePadCapabilities {
		constexpr GamePadCapabilities() = default;		

		constexpr GamePadCapabilities(DirectX::GamePad::Capabilities const& capabilities) {
			_type = static_cast<GamePadCapabilitiesType>(capabilities.gamepadType);
			_connected = capabilities.connected;
			_id = capabilities.id;
			_vid = capabilities.vid;
			_pid = capabilities.pid;
		}

		virtual constexpr GamePadCapabilitiesType GamePadType() const override {
			return _type;
		}

		virtual constexpr bool IsConnected() const override {
			return _connected;
		}

		virtual String Id() const override {
			return XnaHToString(_id);
		}

		virtual constexpr Ushort Vid() const override {
			return _vid;
		}

		virtual constexpr Ushort Pid() const override {
			return _pid;
		}

	private:
		GamePadCapabilitiesType _type{};
		bool _connected{ false };
		std::wstring _id{ 0 };
		Ushort _vid{ 0 };
		Ushort _pid{ 0 };
	};

	struct GamePadState : public IGamePadState {
		constexpr GamePadState() = default;

		constexpr GamePadState(DirectX::GamePad::State const& state) : _dxState(state){
		}

		virtual constexpr GamePadButtons Buttons() const override {
			const auto buttons = _dxState.buttons;
			return GamePadButtons(buttons);
		};

		virtual constexpr GamePadDPad DPad() const override {
			const auto dpad = _dxState.dpad;
			return GamePadDPad(dpad);
		};
		
		inline virtual bool IsConnected() const override{
			return _dxState.IsConnected();
		};

		virtual constexpr Ulong PacketNumber() const override {
			return _dxState.packet;
		};

		virtual constexpr GamePadThumbSticks ThumbSticks() override{
			const auto thumbs = _dxState.thumbSticks;
			return GamePadThumbSticks(thumbs);
		};

		virtual GamePadTriggers Triggers() override {
			const auto triggers = _dxState.triggers;
			return GamePadTriggers(triggers);
		};

		virtual bool IsButtonDown(xna::Buttons button) override {
			switch (button)
			{
			case xna::Buttons::A:
				return _dxState.IsAPressed();				
			case xna::Buttons::B:
				return _dxState.IsBPressed();
			case xna::Buttons::X:
				return _dxState.IsXPressed();
			case xna::Buttons::Y:
				return _dxState.IsYPressed();
			case xna::Buttons::Back:
				return _dxState.IsBackPressed();
			case xna::Buttons::Start:
				return _dxState.IsStartPressed();
			case xna::Buttons::DPadUp:
				return _dxState.IsDPadUpPressed();
			case xna::Buttons::DPadDown:
				return _dxState.IsDPadDownPressed();
			case xna::Buttons::DPadLeft:
				return _dxState.IsDPadLeftPressed();
			case xna::Buttons::DPadRight:
				return _dxState.IsDPadRightPressed();
			case xna::Buttons::LeftShoulder:
				return _dxState.IsLeftShoulderPressed();
			case xna::Buttons::RightShoulder:
				return _dxState.IsRightShoulderPressed();
			case xna::Buttons::LeftStick:
				return _dxState.IsLeftStickPressed();
			case xna::Buttons::RightStick:
				return _dxState.IsRightStickPressed();
			case xna::Buttons::BigButton:
				return _dxState.IsViewPressed();
			case xna::Buttons::LeftThumbstickLeft:
				return _dxState.IsLeftThumbStickLeft();
			case xna::Buttons::LeftThumbstickRight:
				return _dxState.IsLeftThumbStickRight();
			case xna::Buttons::LeftThumbstickDown:
				return _dxState.IsLeftThumbStickDown();
			case xna::Buttons::LeftThumbstickUp:
				return _dxState.IsLeftThumbStickUp();
			case xna::Buttons::RightThumbstickLeft:
				return _dxState.IsRightThumbStickLeft();
			case xna::Buttons::RightThumbstickRight:
				return _dxState.IsLeftThumbStickRight();
			case xna::Buttons::RightThumbstickDown:
				return _dxState.IsRightThumbStickDown();
			case xna::Buttons::RightThumbstickUp:
				return _dxState.IsRightThumbStickUp();
			case xna::Buttons::LeftTrigger:
				return _dxState.IsLeftTriggerPressed();
			case xna::Buttons::RightTrigger:
				return _dxState.IsRightTriggerPressed();
			default:
				return false;
			}
		};

		virtual bool IsButtonUp(xna::Buttons button) override {
			return !IsButtonDown(button);
		};

	public:
		DirectX::GamePad::State _dxState{};
	};

	class GamePad : public IGamePad {
	public:
		inline static sptr<DirectX::GamePad> _dxGamePad = New<DirectX::GamePad>();
	};

	inline GamePadState IGamePad::GetState(PlayerIndex index) {
		const auto state = GamePad::_dxGamePad->GetState(
			static_cast<int>(index)
		);
		return GamePadState(state);
	}

	inline GamePadState IGamePad::GetState(PlayerIndex index, GamePadDeadZone deadZone) {
		const auto state = GamePad::_dxGamePad->GetState(
			static_cast<int>(index),
			static_cast<DirectX::GamePad::DeadZone>(deadZone)
		);
		return GamePadState(state);
	}

	inline GamePadCapabilities IGamePad::GetCapabilities(PlayerIndex index) {
		const auto capabilities = GamePad::_dxGamePad->GetCapabilities(static_cast<int>(index));
		return GamePadCapabilities(capabilities);
	}

	inline bool IGamePad::SetVibration(PlayerIndex index, float leftMotor, float rightMotor, float leftTrigger, float rightTrigger) {
		return GamePad::_dxGamePad->SetVibration(static_cast<int>(index), leftMotor, rightMotor, leftTrigger, rightTrigger);
	}	
}

#endif