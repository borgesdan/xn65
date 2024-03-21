#ifndef XNA_PLATFORM_GAME_DX_HPP
#define XNA_PLATFORM_GAME_DX_HPP

#include "../game/game.hpp"
#include "dxgi.h"
#include "d3d11.h"

namespace xna {
	class Game : public IGame {
	public:
		Game();

		virtual ~Game() override {
		}

		virtual void Exit() override{}
		virtual int Run() override;

	protected:
		virtual void Draw(GameTime const& gameTime) override{}
		virtual void Initialize() override{}
		virtual void Update(GameTime const& gameTime) override{}		

	private:		
		PGameWindow _gameWindow{ nullptr };
		PGraphicsDevice _graphicsDevice{ nullptr };
		
		int startLoop();		
	};
}

#endif