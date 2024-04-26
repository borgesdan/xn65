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
		
		virtual sptr<GameWindow> Window() override {
			return _gameWindow;
		}

		virtual sptr<GraphicsDevice> GetGraphicsDevice() override {
			return _graphicsDevice;
		}

	protected:
		virtual void Draw(GameTime const& gameTime) override;
		
		virtual void Initialize() override;

		virtual void LoadContent() override{}
		
		virtual void Update(GameTime const& gameTime) override;

	public:
		sptr<GraphicsDevice> _graphicsDevice{ nullptr };

	protected:		
		sptr<GameWindow> _gameWindow{ nullptr };
		sptr<AudioEngine> _audioEngine = nullptr;		
		
		GameTime _currentGameTime{};
		DX::StepTimer _stepTimer;
		
	private:
		int startLoop();
		void step();
	};
}

#endif