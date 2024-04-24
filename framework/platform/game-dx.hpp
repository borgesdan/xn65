#ifndef XNA_PLATFORM_GAME_DX_HPP
#define XNA_PLATFORM_GAME_DX_HPP

#include "../default.hpp"
#include "../game/game.hpp"
#include "dxheaders.hpp"
#include "dx/StepTimer.hpp"

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
		virtual void Draw(GameTime const& gameTime) override;
		
		virtual void Initialize() override;

		virtual void LoadContent() override{}
		
		virtual void Update(GameTime const& gameTime) override;

	public:
		PGraphicsDevice _graphicsDevice{ nullptr };		

	protected:		
		PGameWindow _gameWindow{ nullptr };
		PAudioEngine _audioEngine = nullptr;		
		
		GameTime _currentGameTime{};
		DX::StepTimer _stepTimer;
		
	private:
		int startLoop();
		void step();
	};
}

#endif