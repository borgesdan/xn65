#ifndef XNA_GAME_WINDOW_HPP
#define XNA_GAME_WINDOW_HPP

#include "../enums.hpp"
#include "../common/rectangle.hpp"

namespace xna {
	class IGameWindow {
	public:
		virtual ~IGameWindow(){}

		virtual String Title() const = 0;
		virtual void Title(String const& title) = 0;
		virtual Rectangle ClientBounds() const = 0;
		virtual intptr_t Handle() const = 0;
	};
}

#endif