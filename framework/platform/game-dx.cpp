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
		if (GraphicsDevice == nullptr) {
			MessageBox(nullptr, "O dispositivo gráfico não foi inicializar corretamente", "Xna Game Engine", MB_OK);
			return EXIT_FAILURE;
		}

		return startLoop();
	}
	
	int Game::startLoop() {
		MSG msg{};		
		_clock.Start();

		GameTime gameTime{};
		TimeSpan endElapsedTime{};

		do {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				auto elapsed = _clock.ElapsedTime();
				gameTime.ElapsedGameTime = elapsed - endElapsedTime;
				gameTime.TotalGameTime = _clock.TotalTime();

				this->Update(gameTime);				

				elapsed = _clock.ElapsedTime();
				gameTime.ElapsedGameTime = elapsed - endElapsedTime;
				gameTime.TotalGameTime = _clock.TotalTime();

				this->Draw(gameTime);

				GraphicsDevice->Present();

				endElapsedTime = _clock.ElapsedTime();
			}

		} while (msg.message != WM_QUIT);

		return static_cast<int>(msg.wParam);
	}
}
