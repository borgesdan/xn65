#ifndef XNA_PLATFORM_KEYBOARD_DX_HPP
#define XNA_PLATFORM_KEYBOARD_DX_HPP

#include "../input/keyboard.hpp"
#include <Keyboard.h>

namespace xna {
	struct KeyboardState : public IKeyboardState {
	public:
		KeyboardState() = default;
		KeyboardState(DirectX::Keyboard::State const& state) :_state(state) {}

		virtual bool IsKeyDown(Keys key) const override {
			const auto k = static_cast<DirectX::Keyboard::Keys>(key);
			return _state.IsKeyDown(k);
		}

		virtual bool IsKeyUp(Keys key) const override {
			const auto k = static_cast<DirectX::Keyboard::Keys>(key);
			return _state.IsKeyUp(k);
		}		

	public:
		DirectX::Keyboard::State _state{};
	};

	struct Keyboard : public IKeyboard {
		inline static void Initialize() {
			_dxKeyboard = uNew<DirectX::Keyboard>();
		}

	private:
		constexpr Keyboard() = default;
		constexpr Keyboard(Keyboard&&) = default;
		constexpr Keyboard(const Keyboard&) = default;

	public:
		inline static uptr<DirectX::Keyboard> _dxKeyboard = nullptr;
	};

	inline KeyboardState IKeyboard::GetState() {
		if (!Keyboard::_dxKeyboard)
			return KeyboardState();

		const auto state = Keyboard::_dxKeyboard->GetState();
		return KeyboardState(state);
	}

	inline bool IKeyboard::IsConnected() {
		if (!Keyboard::_dxKeyboard)
			return false;

		return Keyboard::_dxKeyboard->IsConnected();
	}
}

#endif