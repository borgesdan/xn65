#ifndef XNA_GAME_WINDOW_HPP
#define XNA_GAME_WINDOW_HPP

#include "../enums.hpp"
#include "../common/rectangle.hpp"

namespace xna {
	class GameWindow {
	public:
		PLATFORM_DEVELOPMENT
			GameWindow();
		PLATFORM_DEVELOPMENT
			String Title() const;
		PLATFORM_DEVELOPMENT
			void Title(String const& title);
		PLATFORM_DEVELOPMENT
			Rectangle ClientBounds() const;
		PLATFORM_DEVELOPMENT
			intptr_t Handle() const;

	private:
		class InternalProperty;
		friend class InternalProperty;

	public:
		sptr<InternalProperty> ip_GameWindow{ nullptr };
	};
}

#endif