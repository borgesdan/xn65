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
#include "Windows.h"

namespace xna {
	Game::Game() {
		Keyboard::_dxKeyboard = uNew<DirectX::Keyboard>();
		Mouse::_dxMouse = uNew<DirectX::Mouse>();
		GamePad::_dxGamePad = uNew<DirectX::GamePad>();

		_gameWindow = New<GameWindow>();
		_gameWindow->Color(255, 155, 55);
		_gameWindow->Title("XN65");
		_gameWindow->Size(
			GraphicsDeviceManager::DefaultBackBufferWidth,
			GraphicsDeviceManager::DefaultBackBufferHeight, false);

		//CoInitializeEx é requisito para inicialização correta de AudioEngine
		const auto hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

		if (FAILED(hr)) {
			MessageBox(nullptr, "Ocorreu um erro ao executar a função CoInitializeEx", "XN65", MB_OK);
		}

		_audioEngine = New<AudioEngine>();		
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

	void Game::Draw(GameTime const& gameTime) {
		_graphicsDevice->Present();
	}

	void Game::Initialize() {
		LoadContent();
	}

	void Game::Update(GameTime const& gameTime) {
		_audioEngine->Update();
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
				const auto total =_stepTimer.GetTotalSeconds();
				const auto elapsedTimeSpan = TimeSpan::FromSeconds(elapsed);
				const auto totalTimeSpan = TimeSpan::FromSeconds(total);
				_currentGameTime.ElapsedGameTime = elapsedTimeSpan;
				_currentGameTime.TotalGameTime = totalTimeSpan;
				Update(_currentGameTime);				
			});

		Draw(_currentGameTime);
	}
}
