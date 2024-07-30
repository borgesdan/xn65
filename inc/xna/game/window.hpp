#ifndef XNA_GAME_WINDOW_HPP
#define XNA_GAME_WINDOW_HPP

#include "../default.hpp"
#include "../common/numerics.hpp"
#include "../csharp/screen.hpp"

namespace xna {
	class GameWindow {
	public:
		GameWindow();
		~GameWindow();
		String Title() const;
		void Title(String const& title);
		Rectangle ClientBounds() const;
		intptr_t Handle() const;

		static uptr<Screen> ScreenFromAdapter(GraphicsAdapter const& adapter);
		static uptr<Screen> ScreenFromHandle(intptr_t windowHandle);

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
}

#endif