#include "xna-dx/framework.hpp"

namespace xna {
	GameWindow::GameWindow() {
		impl = unew<PlatformImplementation>(this);

		impl->_hInstance = GetModuleHandle(NULL);
		impl->_windowIcon = LoadIcon(NULL, IDI_APPLICATION);
		impl->_windowCursor = LoadCursor(NULL, IDC_ARROW);
		impl->_windowStyle = static_cast<int>(GameWindowMode::Windowed);		
		impl->_windowCenterX = impl->_windowWidth / 2.0F;
		impl->_windowCenterY = impl->_windowHeight / 2.0F;
		impl->_windowWidth = GameWindow::DefaultClientWidth;
		impl->_windowHeight = GameWindow::DefaultClientHeight;
	}	

	void GameWindow::PlatformImplementation::Position(int width, int height, bool update) {
		_windowPosX = width;
		_windowPosY = height;
		setCenter();

		if(update) Update();
	}

	void GameWindow::PlatformImplementation::Size(int width, int height, bool update) {
		_windowWidth = width;
		_windowHeight = height;
		setPosition();
		setCenter();
		
		if(update) Update();
	}

	void GameWindow::Title(String const& value) {
		title = value;
		impl->_windowTitle = title;
	}	

	bool GameWindow::PlatformImplementation::Create() {
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

			if (!_windowHandle)
				return false;
		}	

		
		//
		// GameWindow
		//

		const auto handle = reinterpret_cast<intptr_t>(_windowHandle);

		gameWindow->handle = handle;
		gameWindow->title = _windowTitle;
		gameWindow->clientBounds = { _windowPosX, _windowPosY, _windowWidth, _windowHeight };
		gameWindow->currentOrientation = DisplayOrientation::Default;
		
		auto screens = csharp::Screen::AllScreens();
		
		if (screens.size() == 1)
			gameWindow->screenDeviceName = screens[0]->DeviceName();
		else {
			for (size_t i = 0; i < screens.size(); ++i) {
				const auto& screen = screens[i];

				if (screen->Primary())
					gameWindow->screenDeviceName = screen->DeviceName();
			}
		}

		return true;
	}

	bool GameWindow::PlatformImplementation::Update() {
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

		gameWindow->clientBounds = { _windowPosX, _windowPosY, _windowWidth, _windowHeight };

		return true;
	}	

	bool GameWindow::IsWindowMinimized() const {
		return IsIconic(impl->_windowHandle);
	}

	LRESULT GameWindow::PlatformImplementation::WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_ACTIVATE:
		case WM_ACTIVATEAPP:
			if(Keyboard::impl) Keyboard::impl->ProcessMessage(msg, wParam, lParam);
			if(Mouse::impl) Mouse::impl->ProcessMessage(msg, wParam, lParam);
			break;
		case WM_SYSKEYDOWN:
			if (!(wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)) {				
				if (Keyboard::impl) Keyboard::impl->ProcessMessage(msg, wParam, lParam);
			}			
			break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
			if (Keyboard::impl) Keyboard::impl->ProcessMessage(msg, wParam, lParam);
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
			if (Mouse::impl) Mouse::impl->ProcessMessage(msg, wParam, lParam);
			break;
		case WM_KILLFOCUS:
			if (GamePad::impl) GamePad::impl->Suspend();
			break;
		case WM_SETFOCUS:
			if (GamePad::impl) GamePad::impl->Resume();
			break;
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	uptr<csharp::Screen> GameWindow::ScreenFromAdapter(GraphicsAdapter const& adapter) {
		auto screens = csharp::Screen::AllScreens();

		for (size_t i = 0; i < screens.size(); ++i) {
			auto& screen = screens[i];
			
			const auto screeDeviceName = screen->DeviceName();
			const auto adapterDeviceName = adapter.DeviceName();

			if (screeDeviceName == adapterDeviceName)
				return std::move(screen);
		}		

		return nullptr;
	}

	uptr<csharp::Screen> GameWindow::ScreenFromHandle(intptr_t windowHandle) {
		const auto handle = reinterpret_cast<HWND>(windowHandle);
		auto hMonitor = MonitorFromWindow(handle, MONITOR_DEFAULTTOPRIMARY);

		if (!hMonitor)
			return nullptr;		

		const auto hmonitor = reinterpret_cast<intptr_t>(hMonitor);
		auto screen = unew<csharp::Screen>(hmonitor);

		return screen;
	}

	String GameWindow::ScreenDeviceName() const {
		const auto screen = ScreenFromHandle(handle);
		return screen->DeviceName();
	}
}