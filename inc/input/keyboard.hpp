#ifndef XNA_INPUT_KEYBOARD_HPP
#define XNA_INPUT_KEYBOARD_HPP

#include "../default.hpp"

namespace xna {
	struct IKeyboardState {
	public:
		virtual bool IsKeyDown(Keys key) const = 0;
		virtual bool IsKeyUp(Keys key) const = 0;
	};

	class IKeyboard {
	public:
		virtual ~IKeyboard(){}		
		static KeyboardState GetState();
		static bool IsConnected();
	};
}

#endif