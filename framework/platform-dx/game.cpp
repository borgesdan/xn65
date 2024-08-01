#include "xna/xna-dx.hpp"

namespace xna {
	Game::Game() {
		impl = unew<PlatformImplementation>();
		services = snew<GameServiceContainer>();
		auto iservice = reinterpret_pointer_cast<IServiceProvider>(services);
		_contentManager = snew<ContentManager>(services, "");
		_contentManager->mainGameService = iservice;

		_gameWindow = snew<GameWindow>();
		_gameWindow->impl->Color(146, 150, 154);
		_gameWindow->Title("XN65");
		_gameWindow->impl->Size(
			GraphicsDeviceManager::DefaultBackBufferWidth,
			GraphicsDeviceManager::DefaultBackBufferHeight, false);

		_gameComponents = snew<GameComponentCollection>();
	}

	void Game::Exit() {
		_gameWindow->impl->Close();
	}

	int Game::StartGameLoop() {
		MSG msg{};		

		impl->_stepTimer = xna::StepTimer();

		do {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				Step();
			}

		} while (msg.message != WM_QUIT);

		return static_cast<int>(msg.wParam);
	}

	void Game::Step()
	{
		impl->_stepTimer.Tick([&]()
			{
				const auto elapsed = impl->_stepTimer.GetElapsedSeconds();
				const auto total = impl->_stepTimer.GetTotalSeconds();
				const auto elapsedTimeSpan = TimeSpan::FromSeconds(elapsed);
				const auto totalTimeSpan = TimeSpan::FromSeconds(total);
				_currentGameTime.ElapsedGameTime = elapsedTimeSpan;
				_currentGameTime.TotalGameTime = totalTimeSpan;
				Update(_currentGameTime);
			});

		Draw(_currentGameTime);
	}

	int Game::Run() {
		try {
			if (!_gameWindow->impl->Create()) {
				Exception::Throw(Exception::FAILED_TO_CREATE);				
				return false;
			}

			Initialize();			

			if (graphicsDevice == nullptr) {
				MessageBox(nullptr, "O dispositivo gráfico não foi inicializado corretamente", "XN65", MB_OK);
				return EXIT_FAILURE;
			}

			return StartGameLoop();
		}
		catch (std::exception& e) {
			MessageBox(nullptr, e.what(), "XN65", MB_OK);
			return EXIT_FAILURE;
		}		
	}	

	void Game::Initialize() {	
		Keyboard::Initialize();
		Mouse::Initialize();
		GamePad::Initialize();
		AudioEngine::Initialize();		

		LoadContent();
	}

	void Game::Draw(GameTime const& gameTime) {
		if (_enabledGameComponents && !_drawableGameComponents.empty()) {
			const auto count = _drawableGameComponents.size();
			
			if (count != _drawableGameComponentsCount && _gameComponents->AutoSort) {
				GameComponentCollection::DrawSort(_drawableGameComponents);
				_drawableGameComponentsCount = count;
			}			

			for (size_t i = 0; i < count; ++i) {
				auto& component = _drawableGameComponents[i];

				if (!component) continue;

				auto drawable = reinterpret_pointer_cast<IDrawable>(component);

				if (drawable && drawable->Visible())
					drawable->Draw(gameTime);
			}

			_drawableGameComponents.clear();
		}

		graphicsDevice->Present();
	}

	void Game::Update(GameTime const& gameTime) {
		_audioEngine->Update();

		if (_enabledGameComponents && _gameComponents->Count() > 0) {
			const auto count = _gameComponents->Count();
			for (size_t i = 0; i < count; ++i) {
				auto component = _gameComponents->At(i);

				if (!component) continue;

				if (component->Type() == GameComponentType::Drawable) {
					_drawableGameComponents.push_back(component);					
				}

				auto updatable = reinterpret_pointer_cast<IUpdateable>(component);

				if(updatable && updatable->Enabled())
					updatable->Update(gameTime);
			}			
		}
	}

	sptr<GameWindow> Game::Window() { return _gameWindow; }
	sptr<GraphicsDevice> Game::GetGraphicsDevice() { return graphicsDevice; }
	sptr<GameComponentCollection> Game::Components() { return _gameComponents; }
	sptr<GameServiceContainer> Game::Services() { return services; }
	sptr<ContentManager> Game::Content() { return _contentManager; }
	void Game::EnableGameComponents(bool value) { _enabledGameComponents = value; }

	void Game::AttachGraphicsDevice(sptr<GraphicsDevice> const& device) {
		graphicsDevice = device;
	}

	void Game::ResizeWindow(int width, int heigth) {
		const auto windowBounds = _gameWindow->ClientBounds();

		if (windowBounds.Width != width || windowBounds.Height != heigth) {
			_gameWindow->impl->Size(
				width,
				heigth);
			_gameWindow->impl->Update();
		}
	}
}
