#ifndef XNA_GAME_WINDOW_HPP
#define XNA_GAME_WINDOW_HPP

#include "../default.hpp"
#include "../common/numerics.hpp"

namespace xna {
	class GameWindow {
	public:
		GameWindow();
		~GameWindow();
		String Title() const;
		void Title(String const& title);
		Rectangle ClientBounds() const;
		intptr_t Handle() const;

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
}

#endif