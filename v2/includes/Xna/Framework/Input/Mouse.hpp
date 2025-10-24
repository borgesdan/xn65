#ifndef XNA_INPUT_MOUSE_HPP
#define XNA_INPUT_MOUSE_HPP

#include "MouseState.hpp"
#include <cstdint>
#include <memory>

namespace Xna {
	//Allows retrieval of position and button clicks from a mouse input device. 
	class Mouse {
	public:
		//Gets the current state of the mouse, including mouse position and buttons pressed. 
		static MouseState GetState();

		//Gets or sets the window used for mouse processing. 
		//Mouse coordinates returned by GetState are relative to the upper-left corner of this window. 
		constexpr static intptr_t WindowHandle() {
			return windowHandle;
		}

		//Gets or sets the window used for mouse processing. 
		//Mouse coordinates returned by GetState are relative to the upper-left corner of this window. 
		static void WindowHandle(intptr_t value);

		//Sets the position of the mouse cursor relative to the upper-left corner of the window.
		static void SetPosition(int32_t x, int32_t y);

	private:
		friend class Game;
		static void Initialize(intptr_t windowHandle);

		Mouse() = default;
		Mouse(Mouse&) = default;
		Mouse(Mouse&&) = default;

		inline static intptr_t windowHandle = 0;

	public:
		friend class GameWindow;
		struct Implementation;
		static std::unique_ptr<Implementation> implementation;
	};
}

#endif