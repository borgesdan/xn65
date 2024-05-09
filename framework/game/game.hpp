#ifndef XNA_GAME_GAME_HPP
#define XNA_GAME_GAME_HPP

#include "../default.hpp"
#include "time.hpp"
#include "component.hpp"
#include "servicecontainer.hpp"

namespace xna {
	class IGame {
	public:
		virtual void Exit() = 0;
		virtual int Run() = 0;
		virtual sptr<GameWindow> Window() = 0;
		virtual sptr<GraphicsDevice> GetGraphicsDevice() = 0;
		virtual sptr<GameComponentCollection> Components() = 0;
		virtual sptr<GameServiceContainer> Services() = 0;
		virtual sptr<ContentManager> Content() = 0;

	protected:
		virtual void Draw(GameTime const& gameTime) = 0;
		virtual void Initialize() = 0;
		virtual void LoadContent() = 0;
		virtual void Update(GameTime const& gameTime) = 0;
	};
}

#endif