#include "xna/xna-dx.hpp"

namespace xna {
	Game::Game() {
		impl = unew<PlatformImplementation>();
		services = snew<GameServiceContainer>();
		auto iservice = reinterpret_pointer_cast<IServiceProvider>(services);
		contentManager = snew<ContentManager>(services, "");
		contentManager->mainGameService = iservice;

		_gameWindow = snew<GameWindow>();
		_gameWindow->impl->Color(146, 150, 154);
		_gameWindow->Title("XN65");
		_gameWindow->impl->Size(
			GraphicsDeviceManager::DefaultBackBufferWidth,
			GraphicsDeviceManager::DefaultBackBufferHeight, false);

		_gameComponents = snew<GameComponentCollection>();

		IsFixedTimeStep(isFixedTimeStep);
		TargetElapsedTime(targetElapsedTime);
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
				Tick();
			}

		} while (msg.message != WM_QUIT);

		EndRun();
		return static_cast<int>(msg.wParam);
	}

	void Game::Tick()
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

		BeginDraw();
		Draw(_currentGameTime);
		EndDraw();
	}

	int Game::Run() {
		if (isRunning)
			return EXIT_FAILURE;

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

			isRunning = true;
			BeginRun();
			return StartGameLoop();
		}
		catch (std::exception& e) {
			MessageBox(nullptr, e.what(), "XN65", MB_OK);
			return EXIT_FAILURE;
		}		
	}	

	void Game::Initialize() {	
		Keyboard::Initialize();		
		Mouse::Initialize(_gameWindow->Handle());
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
	sptr<GraphicsDevice> Game::Device() const { return graphicsDevice; }
	sptr<GameComponentCollection> Game::Components() const { return _gameComponents; }
	sptr<GameServiceContainer> Game::Services() { return services; }
	sptr<ContentManager> Game::Content() const { return contentManager; }
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

	void Game::Content(sptr<ContentManager> const& value) {
		contentManager = value;
		auto iservice = reinterpret_pointer_cast<IServiceProvider>(services);
		contentManager->mainGameService = iservice;
	}

	void Game::IsFixedTimeStep(bool value) {
		isFixedTimeStep = value;
		impl->_stepTimer.SetFixedTimeStep(value);		
	}

	bool Game::IsMouseVisible() const {
		if (!Mouse::impl)
			return false;

		return Mouse::impl->_dxMouse->IsVisible();
	}

	void Game::IsMouseVisible(bool value) {
		if (!Mouse::impl)
			return;

		Mouse::impl->_dxMouse->SetVisible(value);
	}
	void Game::TargetElapsedTime(TimeSpan const& value) {
		if (!isFixedTimeStep)
			return;

		const auto ticks = targetElapsedTime.Ticks();
		impl->_stepTimer.SetTargetElapsedTicks(ticks);
	}

	void Game::ResetElapsedTime() const {
		impl->_stepTimer.ResetElapsedTime();
	}
	
	void Game::RunOneFrame() {
		Tick();
	}
}
