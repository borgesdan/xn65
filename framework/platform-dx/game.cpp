#include "xna/xna-dx.hpp"

namespace xna {
	Game::Game() {
		impl = unew<PlatformImplementation>();
		services = snew<GameServiceContainer>();
		auto iservice = reinterpret_pointer_cast<IServiceProvider>(services);
		contentManager = snew<ContentManager>(services, "");
		contentManager->mainGameService = iservice;

		gameWindow = snew<GameWindow>();
		gameWindow->impl->Color(146, 150, 154);
		gameWindow->Title("XN65");
		gameWindow->impl->Size(
			GraphicsDeviceManager::DefaultBackBufferWidth,
			GraphicsDeviceManager::DefaultBackBufferHeight, false);

		gameComponents = snew<GameComponentCollection>();

		IsFixedTimeStep(isFixedTimeStep);
		TargetElapsedTime(targetElapsedTime);
	}

	void Game::Exit() {
		gameWindow->impl->Close();
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
				currentGameTime.ElapsedGameTime = elapsedTimeSpan;
				currentGameTime.TotalGameTime = totalTimeSpan;
				Update(currentGameTime);
			});

		BeginDraw();
		Draw(currentGameTime);
		EndDraw();
	}

	int Game::Run() {
		if (isRunning)
			return EXIT_FAILURE;

		try {
			if (!gameWindow->impl->Create()) {
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
		Mouse::Initialize(gameWindow->Handle());
		GamePad::Initialize();
		AudioEngine::Initialize();		

		LoadContent();
	}

	void Game::Draw(GameTime const& gameTime) {
		if (enabledGameComponents && !drawableGameComponents.empty()) {
			const auto count = drawableGameComponents.size();
			
			if (count != drawableGameComponentsCount && gameComponents->AutoSort) {
				GameComponentCollection::DrawSort(drawableGameComponents);
				drawableGameComponentsCount = count;
			}			

			for (size_t i = 0; i < count; ++i) {
				auto& component = drawableGameComponents[i];

				if (!component) continue;

				auto drawable = reinterpret_pointer_cast<IDrawable>(component);

				if (drawable && drawable->Visible())
					drawable->Draw(gameTime);
			}

			drawableGameComponents.clear();
		}

		graphicsDevice->Present();
	}

	void Game::Update(GameTime const& gameTime) {
		audioEngine->Update();

		if (enabledGameComponents && gameComponents->Count() > 0) {
			const auto count = gameComponents->Count();
			for (size_t i = 0; i < count; ++i) {
				auto component = gameComponents->At(i);

				if (!component) continue;

				if (component->Type() == GameComponentType::Drawable) {
					drawableGameComponents.push_back(component);					
				}

				auto updatable = reinterpret_pointer_cast<IUpdateable>(component);

				if(updatable && updatable->Enabled())
					updatable->Update(gameTime);
			}			
		}
	}

	sptr<GameWindow> Game::Window() { return gameWindow; }
	sptr<GraphicsDevice> Game::Device() const { return graphicsDevice; }
	sptr<GameComponentCollection> Game::Components() const { return gameComponents; }
	sptr<GameServiceContainer> Game::Services() { return services; }
	sptr<ContentManager> Game::Content() const { return contentManager; }
	void Game::EnableGameComponents(bool value) { enabledGameComponents = value; }

	void Game::AttachGraphicsDevice(sptr<GraphicsDevice> const& device) {
		graphicsDevice = device;
	}

	void Game::ResizeWindow(int width, int heigth) {
		const auto windowBounds = gameWindow->ClientBounds();

		if (windowBounds.Width != width || windowBounds.Height != heigth) {
			gameWindow->impl->Size(
				width,
				heigth);
			gameWindow->impl->Update();
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
