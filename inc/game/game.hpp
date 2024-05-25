#ifndef XNA_GAME_GAME_HPP
#define XNA_GAME_GAME_HPP

#include "../default.hpp"
#include "game/time.hpp"

namespace xna {
	class Game : public std::enable_shared_from_this<Game> {
	public:
		Game();
		~Game();
		void Exit();
		int Run();
		sptr<GameWindow> Window();
		sptr<GraphicsDevice> GetGraphicsDevice();
		sptr<GameComponentCollection> Components();
		sptr<GameServiceContainer> Services();
		sptr<ContentManager> Content();
		void EnableGameComponents(bool value);

	protected:
		virtual void Draw(GameTime const& gameTime);
		virtual void Initialize();
		virtual void LoadContent(){}
		virtual void Update(GameTime const& gameTime);
		int StartGameLoop();
		void Step();

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

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
}

#endif