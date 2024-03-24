#ifndef XNA_PLATFORM_GAME_DX_HPP
#define XNA_PLATFORM_GAME_DX_HPP

#include "../game/game.hpp"
#include "clock-dx.hpp"
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

	protected:		
		PGraphicsDevice _graphicsDevice{ nullptr };		
		PGameWindow _gameWindow{ nullptr };
		
		GameClock _clock{};
		GameTime _currentGameTime{};
		
		int startLoop();		
	};
}

#endif