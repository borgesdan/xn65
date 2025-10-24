#include "Xna/Framework/Input/GamePad.hpp"
#include "Xna/Framework/Input/Mouse.hpp"
#include "Xna/Framework/Input/Keyboard.hpp"
#include "../Input/InputImplementations.hpp"
#include "GameImplementations.hpp"
#include "CSharp/Windows/Forms/SystemInformation.hpp"
#include "CSharp/Exception.hpp"

using std::make_shared;
using std::string;
using CSharp::Screen;
using CSharp::SystemInformation;
using CSharp::InvalidOperationException;

namespace Xna {
	GameWindow::GameWindow() {
		implementation = make_shared<GameWindow::Implementation>();
	}

	void GameWindow::Title(string const& value) {
		implementation->title = value;
	}

	string GameWindow::Title() const {
		return implementation->title;
	}

	intptr_t GameWindow::Handle() const {
		return reinterpret_cast<intptr_t>(implementation->handle);
	}

	Rectangle GameWindow::ClientBounds() const {
		return {
			implementation->positionX,
			implementation->positionY,
			implementation->width,
			implementation->height
		};
	}

	std::string GameWindow::ScreenDeviceName() const {
		//TODO: implementar
		return std::string();
	}

	bool GameWindow::IsWindowMinimized() const {
		return IsIconic(implementation->handle);
	}	

	Screen GameWindow::ScreenFromHandle(intptr_t windowHandle) {
		const auto handle = reinterpret_cast<HWND>(windowHandle);
		auto hMonitor = MonitorFromWindow(handle, MONITOR_DEFAULTTOPRIMARY);

		if (!hMonitor)
			throw InvalidOperationException("HMONITOR is null.");

		const auto hmonitor = reinterpret_cast<intptr_t>(hMonitor);
		auto screen = Screen(hmonitor);

		return screen;
	}

	LRESULT GameWindow::Implementation::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_ACTIVATE:
		case WM_ACTIVATEAPP:
			Keyboard::implementation->ProcessMessage(msg, wParam, lParam);
			Mouse::implementation->ProcessMessage(msg, wParam, lParam);
			break;
		case WM_SYSKEYDOWN:
			if (!(wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)) {
				Keyboard::implementation->ProcessMessage(msg, wParam, lParam);
			}
			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Keyboard::implementation->ProcessMessage(msg, wParam, lParam);
			break;
		case WM_INPUT:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_XBUTTONDOWN:
		case WM_XBUTTONUP:
		case WM_MOUSEHOVER:
			Mouse::implementation->ProcessMessage(msg, wParam, lParam);
			break;
		case WM_KILLFOCUS:
			GamePad::implementation->Suspend();
			break;
		case WM_SETFOCUS:
			GamePad::implementation->Resume();
			break;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	bool GameWindow::Implementation::Create() {
		WNDCLASSEX wndClass{};
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = WinProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = instance;
		wndClass.hIcon = icon;
		wndClass.hCursor = cursor;
		wndClass.hbrBackground = (HBRUSH)CreateSolidBrush(color);
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = "X65GameWindow";
		wndClass.hIconSm = icon;

		if (!RegisterClassEx(&wndClass))
			return false;

		handle = CreateWindowEx(
			NULL,
			"XnaGameWindow",
			title.c_str(),
			style,
			positionX, positionY,
			width, height,
			NULL,
			NULL,
			instance,
			NULL);

		if (style == static_cast<int>(GameWindowMode::Windowed)) {
			RECT winRect = { 0, 0, width, height };

			AdjustWindowRectEx(&winRect,
				GetWindowStyle(handle),
				GetMenu(handle) != NULL,
				GetWindowExStyle(handle));

			positionX = GetSystemMetrics(SM_CXSCREEN) / 2 - (winRect.right - winRect.left) / 2;
			positionY = GetSystemMetrics(SM_CYSCREEN) / 2 - (winRect.bottom - winRect.top) / 2;

			MoveWindow(
				handle,
				positionX,
				positionY,
				winRect.right - winRect.left,
				winRect.bottom - winRect.top,
				TRUE);

			if (!handle)
				return false;
		}		

		return true;
	}

}