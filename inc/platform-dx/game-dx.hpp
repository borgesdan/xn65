#ifndef XNA_PLATFORM_GAME_DX_HPP
#define XNA_PLATFORM_GAME_DX_HPP

#include "../content/manager.hpp"
#include "../default.hpp"
#include "../game/game.hpp"
#include "dxheaders.hpp"
#include "platform-dx/dx/StepTimer.hpp"

namespace xna {
	class Game : public IGame, public std::enable_shared_from_this<Game> {
	public:
		Game();		

		void Exit() override;
		int Run() override;
		
		inline sptr<GameWindow> Window() override {	return _gameWindow; }
		inline sptr<GraphicsDevice> GetGraphicsDevice() override { return graphicsDevice; }
		inline sptr<GameComponentCollection> Components() override { return _gameComponents; }
		inline sptr<GameServiceContainer> Services() override { return services; }
		inline sptr<ContentManager> Content() override { return _contentManager; }
		constexpr void EnableGameComponents(bool value) { _enabledGameComponents = value; }

	protected:
		virtual void Draw(GameTime const& gameTime) override;		
		virtual void Initialize() override;
		virtual void LoadContent() override{}		
		virtual void Update(GameTime const& gameTime) override;

	public:
		sptr<GraphicsDevice> graphicsDevice = nullptr;

	protected:
		sptr<GameServiceContainer> services = nullptr;

	private:		
		sptr<GameComponentCollection> _gameComponents = nullptr;
		sptr<GameWindow> _gameWindow{ nullptr };
		sptr<AudioEngine> _audioEngine = nullptr;
		sptr<ContentManager> _contentManager;
		std::vector<sptr<IGameComponent>> _drawableGameComponents;
		size_t _drawableGameComponentsCount{ 0 };
		bool _enabledGameComponents{ false };		
		GameTime _currentGameTime{};
		DX::StepTimer _stepTimer{};

		int startLoop();
		void step();
	};
}

#endif