#ifndef XNA_FRAMEWORK_GAME_GAMEIMPLEMENTATIONS_HPP
#define XNA_FRAMEWORK_GAME_GAMEIMPLEMENTATIONS_HPP
#include "Xna/Framework/Game/GameWindow.hpp"
#include "../../../WindowsPlatform.hpp"

#include <memory>

namespace Xna {
	enum class GameWindowMode : UINT {
		Fullscreen = WS_POPUP | WS_VISIBLE,
		Windowed = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE,
		Borderless = WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE,
	};

	struct GameWindow::Implementation {
		HINSTANCE instance{ nullptr };
		HWND handle{ nullptr };
		int width{ GAMEWINDOW_DEFAULT_CLIENT_WIDTH };
		int	height{ GAMEWINDOW_DEFAULT_CLIENT_HEIGHT };
		HICON icon{ LoadIcon(NULL, IDI_APPLICATION) };
		HCURSOR cursor{ LoadCursor(NULL, IDC_ARROW) };
		COLORREF color{ RGB(0,0,0) };
		std::string title{ "XN65 Game" };
		DWORD style{ static_cast<DWORD>(GameWindowMode::Windowed) };
		int positionX{ 0 };
		int	positionY{ 0 };
		float centerX{ 0 };
		float centerY{ 0 };

		void setPosition() {
			positionX = GetSystemMetrics(SM_CXSCREEN) / 2 - width / 2;
			positionY = GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2;
		}

		void setCenter() {
			centerX = width / 2.0f;
			centerY = height / 2.0f;
		}

		bool Create();
		static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};	
}

#endif