#include "game-dx.hpp"
#include "window-dx.hpp"
#include "device-dx.hpp"
#include "Windows.h"

namespace xna {
	Game::Game() {
		_gameWindow = New<GameWindow>();
		_gameWindow->Color(255, 155, 55);
		_gameWindow->Title("Teste de t�tulo");

		_graphicsDevice = New<GraphicsDevice>();
	}

	int Game::Run() {
		if (!_gameWindow->Create()) {
			MessageBox(nullptr, "Falha na cria��o da janela", "Xna Game Engine", MB_OK);			
			return EXIT_FAILURE;
		}

		if (!_graphicsDevice->Initialize(*_gameWindow)) {
			MessageBox(nullptr, "Falha na inicializa��o do dispositivo gr�fico", "Xna Game Engine", MB_OK);			
			return EXIT_FAILURE;
		}

		return startLoop();
	}
	
	int Game::startLoop() {
		MSG msg{};

		do {
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else {
				GameTime gt;
				this->Update(gt);
				_graphicsDevice->Clear();
				this->Draw(gt);
				_graphicsDevice->Present();
			}

		} while (msg.message != WM_QUIT);

		return msg.wParam;
	}
}
