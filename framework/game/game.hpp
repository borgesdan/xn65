#ifndef XNA_GAME_GAME_HPP
#define XNA_GAME_GAME_HPP

#include "../enums.hpp"
#include "../forward.hpp"
#include "../types.hpp"
#include "time.hpp"
#include "window.hpp"

namespace xna {
	class IGame {
	public:
		virtual ~IGame(){}

		virtual void Exit() = 0;
		virtual int Run() = 0;
		virtual sptr<GameWindow> Window() = 0;
		virtual sptr<GraphicsDevice> GetGraphicsDevice() = 0;

	protected:
		virtual void Draw(GameTime const& gameTime) = 0;
		virtual void Initialize() = 0;
		virtual void LoadContent() = 0;
		virtual void Update(GameTime const& gameTime) = 0;
	};
}

#endif