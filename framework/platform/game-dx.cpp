#include "game-dx.hpp"
#include "window-dx.hpp"
#include "device-dx.hpp"
#include "Windows.h"
#include "../game/time.hpp"
#include "gdevicemanager-dx.hpp"

namespace xna {
	Game::Game() {
		_gameWindow = New<GameWindow>();
		_gameWindow->Color(255, 155, 55);
		_gameWindow->Title("Teste de título");
		_gameWindow->Size(
			GraphicsDeviceManager::DefaultBackBufferWidth,
			GraphicsDeviceManager::DefaultBackBufferHeight, false);
	}

	int Game::Run() {		
		Initialize();

		if (_graphicsDevice == nullptr) {
			MessageBox(nullptr, "O dispositivo gráfico não foi inicializado corretamente", "Xna Game Engine", MB_OK);
			return EXIT_FAILURE;
		}

		return startLoop();
	}
	
	int Game::startLoop() {
		MSG msg{};								
		_clock.Start();

		do {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{	
				TranslateMessage(&msg);
				DispatchMessage(&msg);	
			}
			else {
				tick();
			}

		} while (msg.message != WM_QUIT);

		return static_cast<int>(msg.wParam);
	}	

	void Game::tick() {
		_clock.Reset();

		this->Update(_currentGameTime);

		_currentGameTime.ElapsedGameTime = _clock.ElapsedTime();
		_currentGameTime.TotalGameTime = _clock.TotalTime();

		this->Draw(_currentGameTime);

		_graphicsDevice->Present();

		_currentGameTime.ElapsedGameTime = _clock.ElapsedTime();
		_currentGameTime.TotalGameTime = _clock.TotalTime();
	}
}
