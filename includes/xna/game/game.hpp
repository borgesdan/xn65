#ifndef XNA_GAME_GAME_HPP
#define XNA_GAME_GAME_HPP

#include "../default.hpp"
#include "time.hpp"

namespace xna {
	//Provides basic graphics device initialization, game logic, and rendering code.
	class Game : public std::enable_shared_from_this<Game> {
	public:
		Game();

		//Gets the collection of GameComponents owned by the game.
		sptr<GameComponentCollection> Components() const;
		//Gets or sets the current ContentManager.
		sptr<ContentManager> Content() const;
		//Gets or sets the current ContentManager.
		void Content(sptr<ContentManager> const& value);
		//Gets the current GraphicsDevice. 
		sptr<GraphicsDevice> Device() const;
		//Gets or sets a value indicating whether to use fixed time steps. 
		//The default value for IsFixedTimeStep is true. 
		constexpr bool IsFixedTimeStep() const { return isFixedTimeStep; }
		//Gets or sets a value indicating whether to use fixed time steps. 
		//The default value for IsFixedTimeStep is true. 
		void IsFixedTimeStep(bool value);
		//Gets or sets a value indicating whether the mouse cursor should be visible. 
		bool IsMouseVisible() const;
		//Gets or sets a value indicating whether the mouse cursor should be visible. 
		void IsMouseVisible(bool value);
		//Gets the GameServiceContainer holding all the service providers attached to the Game.
		sptr<GameServiceContainer> Services();
		//Gets or sets the target time between calls to Update when IsFixedTimeStep is true. 
		constexpr csharp::TimeSpan TargetElapsedTime() const { return targetElapsedTime; }
		//Gets or sets the target time between calls to Update when IsFixedTimeStep is true. 
		void TargetElapsedTime(csharp::TimeSpan const& value);
		//Gets the underlying operating system window.
		sptr<GameWindow> Window();

		//Exits the game.
		void Exit();
		//Resets the elapsed time counter.
		void ResetElapsedTime() const;
		//Call this method to initialize the game, begin running the game loop, and start processing events for the game. 
		int Run();
		//Run the game through what would happen in a single tick of the game clock; this method is designed for debugging only. 
		void RunOneFrame();
		//Updates the game's clock and calls Update and Draw. 
		void Tick();

		void EnableGameComponents(bool value);
		void AttachGraphicsDevice(sptr<GraphicsDevice> const& graphicsDevice);
		void ResizeWindow(int width, int heigth);

	protected:
		//Starts the drawing of a frame. This method is followed by calls to Draw and EndDraw. 
		virtual void BeginDraw(){}
		//Called after all components are initialized but before the first update in the game loop. 
		virtual void BeginRun() {};
		//Called when the game determines it is time to draw a frame.
		virtual void Draw(GameTime const& gameTime);
		//Ends the drawing of a frame. This method is preceeded by calls to Draw and BeginDraw.
		virtual void EndDraw() {}
		//Called after the game loop has stopped running before exiting.
		virtual void EndRun() {};
		//Called after the Game and GraphicsDevice are created, but before LoadContent. 
		virtual void Initialize();
		//Called when graphics resources need to be loaded.
		virtual void LoadContent(){}
		//Called when the game has determined that game logic needs to be processed.
		virtual void Update(GameTime const& gameTime);		

	private:
		int StartGameLoop();

	private:
		friend class GraphicsDeviceManager;

		sptr<GameComponentCollection> gameComponents = nullptr;
		sptr<GameWindow> gameWindow{ nullptr };
		sptr<AudioEngine> audioEngine = nullptr;
		sptr<ContentManager> contentManager;
		std::vector<sptr<IGameComponent>> drawableGameComponents;	
		size_t drawableGameComponentsCount{ 0 };
		bool enabledGameComponents{ false };
		GameTime currentGameTime{};
		bool isFixedTimeStep{ true };
		csharp::TimeSpan targetElapsedTime{ csharp::TimeSpan::FromTicks(166667L) };
		bool isRunning{ false };
		sptr<GameServiceContainer> services = nullptr;
		sptr<GraphicsDevice> graphicsDevice = nullptr;

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl = nullptr;
	};
}

#endif