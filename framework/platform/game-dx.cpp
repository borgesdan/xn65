#include "game-dx.hpp"
#include "window-dx.hpp"
#include "device-dx.hpp"
#include "Windows.h"
#include "../game/time.hpp"
#include "gdevicemanager-dx.hpp"
#include "keyboard-dx.hpp"
#include "mouse-dx.hpp"
#include "audioengine-dx.hpp"

namespace xna {
	Game::Game() {
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
		Keyboard::_dxKeyboard = uNew<DirectX::Keyboard>();
		Mouse::_dxMouse = uNew<DirectX::Mouse>();
	}

	static void intializeAudioEngine() {

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
		LoadContent();
	}

	void Game::Update(GameTime const& gameTime) {
		_audioEngine->Update();
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
