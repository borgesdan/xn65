#include "csharp/type.hpp"
#include "game/time.hpp"
#include "game/component.hpp"
#include "game/servicecontainer.hpp"
#include "platform-dx/implementations.hpp"
#include "game/gdevicemanager.hpp"
#include "content/manager.hpp"

namespace xna {
	Game::Game() {
		impl = unew<PlatformImplementation>();
		services = New<GameServiceContainer>();
		_contentManager = New<ContentManager>("", services);

		_gameWindow = New<GameWindow>();
		_gameWindow->impl->Color(146, 150, 154);
		_gameWindow->Title("XN65");
		_gameWindow->impl->Size(
			GraphicsDeviceManager::DefaultBackBufferWidth,
			GraphicsDeviceManager::DefaultBackBufferHeight, false);

		_gameComponents = New<GameComponentCollection>();
	}

	Game::~Game() {
		impl = nullptr;
	}

	void Game::Exit() {
		_gameWindow->impl->Close();
	}

	int Game::StartGameLoop() {
		MSG msg{};		

		impl->_stepTimer = DX::StepTimer();

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
		Initialize();

		if (graphicsDevice == nullptr) {
			MessageBox(nullptr, "O dispositivo gr�fico n�o foi inicializado corretamente", "XN65", MB_OK);
			return EXIT_FAILURE;
		}

		return StartGameLoop();
	}	

	void Game::Initialize() {
//#if (_WIN32_WINNT >= _WIN32_WINNT_WIN10)
		Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
		if (FAILED(initialize))
		{
			MessageBox(nullptr, "Ocorreu um erro ao chamar Microsoft::WRL::Wrappers::RoInitializeWrapper.", "XN65", MB_OK);
		}
//#else
		HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (FAILED(hr))
		{
			MessageBox(nullptr, "Ocorreu um erro ao chamar CoInitializeEx.", "XN65", MB_OK);
		}
//#endif		
		Keyboard::Initialize();
		Mouse::Initialize();
		GamePad::Initialize();

		_audioEngine = New<AudioEngine>();

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
}