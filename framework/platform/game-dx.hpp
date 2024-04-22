#ifndef XNA_PLATFORM_GAME_DX_HPP
#define XNA_PLATFORM_GAME_DX_HPP

#include "../default.hpp"
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

		virtual void Exit() override;

		virtual int Run() override;
		
		virtual PGameWindow Window() override {
			return _gameWindow;
		}

		virtual PGraphicsDevice GetGraphicsDevice() override {
			return _graphicsDevice;
		}

	protected:
		virtual void Draw(GameTime const& gameTime) override{}
		
		virtual void Initialize() override;

		virtual void LoadContent() override{}
		
		virtual void Update(GameTime const& gameTime) override;

	public:
		PGraphicsDevice _graphicsDevice{ nullptr };		

	protected:		
		PGameWindow _gameWindow{ nullptr };
		PAudioEngine _audioEngine = nullptr;
		
		GameClock _clock{};
		GameTime _currentGameTime{};
		
	private:
		int startLoop();
		void tick();
	};
}

#endif