#include "window-dx.hpp"
#include "keyboard-dx.hpp"
#include "mouse-dx.hpp"

namespace xna {
	GameWindow::GameWindow() {
		_hInstance = GetModuleHandle(NULL);
		_windowIcon = LoadIcon(NULL, IDI_APPLICATION);
		_windowCursor = LoadCursor(NULL, IDC_ARROW);
		_windowStyle = static_cast<int>(GameWindowMode::Windowed);		
		_windowCenterX = _windowWidth / 2.0F;
		_windowCenterY = _windowHeight / 2.0F;	

	}

	void GameWindow::Position(int width, int height, bool update) {
		_windowPosX = width;
		_windowPosY = height;
		setCenter();

		if(update) Update();
	}

	void GameWindow::Size(int width, int height, bool update) {
		_windowWidth = width;
		_windowHeight = height;
		setPosition();
		setCenter();
		
		if(update) Update();
	}

	void GameWindow::Title(String const& title) {
		_windowTitle = title;
	}	

	bool GameWindow::Create() {
		WNDCLASSEX wndClass{};
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_DBLCLKS | CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = WinProc;
		wndClass.cbClsExtra = 0;
		wndClass.cbWndExtra = 0;
		wndClass.hInstance = _hInstance;
		wndClass.hIcon = _windowIcon;
		wndClass.hCursor = _windowCursor;
		wndClass.hbrBackground = (HBRUSH)CreateSolidBrush(_windowColor);
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = "XnaGameWindow";
		wndClass.hIconSm = _windowIcon;

		if (!RegisterClassEx(&wndClass))
			return false;

		_windowHandle = CreateWindowEx(
			NULL,
			"XnaGameWindow",
			_windowTitle.c_str(),
			_windowStyle,
			_windowPosX, _windowPosY,
			_windowWidth, _windowHeight,
			NULL,
			NULL,
			_hInstance,
			NULL);

		if (_windowStyle == static_cast<int>(GameWindowMode::Windowed)) {
			RECT winRect = { 0, 0, _windowWidth, _windowHeight };

			AdjustWindowRectEx(&winRect,
				GetWindowStyle(_windowHandle),
				GetMenu(_windowHandle) != NULL,
				GetWindowExStyle(_windowHandle));

			_windowPosX = GetSystemMetrics(SM_CXSCREEN) / 2 - (winRect.right - winRect.left) / 2;
			_windowPosY = GetSystemMetrics(SM_CYSCREEN) / 2 - (winRect.bottom - winRect.top) / 2;

			MoveWindow(
				_windowHandle,
				_windowPosX,
				_windowPosY,
				winRect.right - winRect.left,
				winRect.bottom - winRect.top,
				TRUE);

			return _windowHandle ? true : false;
		}	

		return true;
	}

	bool GameWindow::Update() {
		if (_windowStyle == static_cast<int>(GameWindowMode::Windowed)) {
			RECT winRect = { 0, 0, _windowWidth, _windowHeight };

			AdjustWindowRectEx(&winRect,
				GetWindowStyle(_windowHandle),
				GetMenu(_windowHandle) != NULL,
				GetWindowExStyle(_windowHandle));

			_windowPosX = GetSystemMetrics(SM_CXSCREEN) / 2 - (winRect.right - winRect.left) / 2;
			_windowPosY = GetSystemMetrics(SM_CYSCREEN) / 2 - (winRect.bottom - winRect.top) / 2;

			MoveWindow(
				_windowHandle,
				_windowPosX,
				_windowPosY,
				winRect.right - winRect.left,
				winRect.bottom - winRect.top,
				TRUE);

			return _windowHandle ? true : false;
		}

		return true;
	}

	String GameWindow::Title() const {
		return _windowTitle;
	}

	Rectangle GameWindow::ClientBounds() const {
		return Rectangle(
			_windowPosX,
			_windowPosY,
			_windowWidth,
			_windowHeight
		);
	}

	intptr_t GameWindow::Handle() const {
		return reinterpret_cast<intptr_t>(_windowHandle);
	}

	LRESULT GameWindow::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_ACTIVATE:
		case WM_ACTIVATEAPP:
			Keyboard::_dxKeyboard->ProcessMessage(msg, wParam, lParam);
			Mouse::_dxMouse->ProcessMessage(msg, wParam, lParam);
			break;
		case WM_SYSKEYDOWN:
			if (!(wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)) {
				Keyboard::_dxKeyboard->ProcessMessage(msg, wParam, lParam);
			}			
			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			Keyboard::_dxKeyboard->ProcessMessage(msg, wParam, lParam);
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
			Mouse::_dxMouse->ProcessMessage(msg, wParam, lParam);
			break;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}