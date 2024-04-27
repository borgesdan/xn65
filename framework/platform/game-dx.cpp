#define NOMINMAX
#include "../game/time.hpp"
#include "audioengine-dx.hpp"
#include "device-dx.hpp"
#include "game-dx.hpp"
#include "gamepad-dx.hpp"
#include "gdevicemanager-dx.hpp"
#include "keyboard-dx.hpp"
#include "mouse-dx.hpp"
#include "window-dx.hpp"
#include <Windows.h>

namespace xna {
	Game::Game() {
		_gameWindow = New<GameWindow>();
		_gameWindow->Color(255, 155, 55);
		_gameWindow->Title("XN65");
		_gameWindow->Size(
			GraphicsDeviceManager::DefaultBackBufferWidth,
			GraphicsDeviceManager::DefaultBackBufferHeight, false);

		_gameComponents = New<GameComponentCollection>();
	}

	void Game::Exit()
	{
		_gameWindow->Close();
	}

	int Game::Run() {
		Initialize();

		if (_graphicsDevice == nullptr) {
			MessageBox(nullptr, "O dispositivo gráfico não foi inicializado corretamente", "XN65", MB_OK);
			return EXIT_FAILURE;
		}

		return startLoop();
	}	

	void Game::Initialize() {
		Keyboard::Initialize();
		Mouse::Initialize();

		//initialize é requisito para GamePad
		Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);

		if (FAILED(initialize))
			MessageBox(nullptr, "Ocorreu um erro ao executar Microsoft::WRL::Wrappers::RoInitializeWrapper. O GamePad não foi inicializado corretamente.", "XN65", MB_OK);

		GamePad.Initialize();

		//CoInitializeEx é requisito para AudioEngine
		const auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

		if (FAILED(hr))
			MessageBox(nullptr, "Ocorreu um erro ao executar CoInitializeEx. O AudioEngine não foi inicializado corretamente.", "XN65", MB_OK);

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

		_graphicsDevice->Present();
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

	int Game::startLoop() {
		MSG msg{};
		_stepTimer = DX::StepTimer();

		do {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				step();
			}

		} while (msg.message != WM_QUIT);

		return static_cast<int>(msg.wParam);
	}

	void Game::step()
	{
		_stepTimer.Tick([&]()
			{
				const auto elapsed = _stepTimer.GetElapsedSeconds();
				const auto total = _stepTimer.GetTotalSeconds();
				const auto elapsedTimeSpan = TimeSpan::FromSeconds(elapsed);
				const auto totalTimeSpan = TimeSpan::FromSeconds(total);
				_currentGameTime.ElapsedGameTime = elapsedTimeSpan;
				_currentGameTime.TotalGameTime = totalTimeSpan;
				Update(_currentGameTime);
			});

		Draw(_currentGameTime);
	}
}
