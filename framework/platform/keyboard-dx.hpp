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

	private:
		DirectX::Keyboard::State _state{};
	};

	class Keyboard : public IKeyboard {
	public:
		friend class IKeyboard;
		friend class GameWindow;

		Keyboard() = default;

	private:
		inline static sptr<DirectX::Keyboard> initializeKeyboard() {
			return New<DirectX::Keyboard>();
		}

		inline static sptr<DirectX::Keyboard> _dxKeyboard = initializeKeyboard();
	};

	inline KeyboardState IKeyboard::GetState() {
		
		const auto state = Keyboard::_dxKeyboard->GetState();
		return KeyboardState(state);
	}

	inline bool IKeyboard::IsConnected() {
		return Keyboard::_dxKeyboard->IsConnected();
	}
}

#endif