#include "window-dx.hpp"

namespace xna {
	GameWindow::GameWindow() {
		ip_GameWindow = New<InternalProperty>(this);
		auto& p = ip_GameWindow;

		p->_hInstance = GetModuleHandle(NULL);
		p->_windowIcon = LoadIcon(NULL, IDI_APPLICATION);
		p->_windowCursor = LoadCursor(NULL, IDC_ARROW);
		p->_windowStyle = static_cast<int>(GameWindowMode::Windowed);		
		p->_windowCenterX = p->_windowWidth / 2.0F;
		p->_windowCenterY = p->_windowHeight / 2.0F;
	}

	void GameWindow::Title(String const& title) {
		ip_GameWindow->_windowTitle = title;
	}

	void GameWindow::InternalProperty::Size(int width, int height) {
		_windowWidth = width;
		_windowHeight = height;

		setCenter();
		setPosition();
	}

	bool GameWindow::InternalProperty::Create() {
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
	}

	String GameWindow::Title() const {
		return ip_GameWindow->_windowTitle;
	}

	Rectangle GameWindow::ClientBounds() const {
		return Rectangle(
			ip_GameWindow->_windowPosX,
			ip_GameWindow->_windowPosY,
			ip_GameWindow->_windowWidth,
			ip_GameWindow->_windowHeight
		);
	}

	intptr_t GameWindow::Handle() const {
		return reinterpret_cast<intptr_t>(ip_GameWindow->_windowHandle);
	}

	LRESULT GameWindow::InternalProperty::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}