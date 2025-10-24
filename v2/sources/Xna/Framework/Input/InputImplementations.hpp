#ifndef XNA_FRAMEWORK_INPUT_INPUTIMPLEMENTATIONS_HPP
#define XNA_FRAMEWORK_INPUT_INPUTIMPLEMENTATIONS_HPP

#include "Xna/Framework/Input/Mouse.hpp"
#include "Xna/Framework/Input/Keyboard.hpp"
#include "Xna/Framework/Input/GamePad.hpp"
#include "../../../WindowsPlatform.hpp"

namespace Xna {
	struct Mouse::Implementation {
		inline void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam) const {
			mouse.ProcessMessage(message, wParam, lParam);
		}

		DirectX::Mouse mouse{};
	};

	struct Keyboard::Implementation {
		inline void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam) const {
			keyboard.ProcessMessage(message, wParam, lParam);
		}

		DirectX::Keyboard keyboard{};
	};

	struct GamePad::Implementation {
		inline void Suspend() { gamePad.Suspend(); }
		inline void Resume() { gamePad.Resume(); }

		DirectX::GamePad gamePad{};
	};
}

#endif