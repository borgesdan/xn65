#ifndef XNA_PLATFORM_WINDOW_DX_HPP
#define XNA_PLATFORM_WINDOW_DX_HPP

#include "../game/window.hpp"
#include <Windows.h>
#include <windowsx.h>

namespace xna {	

	enum class GameWindowMode : UINT {
		Fullscreen = WS_POPUP | WS_VISIBLE,
		Windowed = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE,
		Borderless = WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE,
	};

	class GameWindow::InternalProperty {
		friend class GameWindow;
	public:
		InternalProperty(GameWindow* gamewindow) :
			_gamewindow(gamewindow){}

		constexpr void Mode(GameWindowMode mode) {
			_windowStyle = static_cast<int>(mode);
		}

		constexpr GameWindowMode Mode() const {
			return static_cast<GameWindowMode>(_windowStyle);
		}

		void Size(int width, int height);

		inline HINSTANCE HInstance() const {
			return _hInstance;
		}

		inline HWND WindowHandle() const {
			return _windowHandle;
		}

		inline int Width() const {
			return _windowWidth;
		}

		inline int Height() const {
			return _windowHeight;
		}

		inline void Icon(unsigned int icon) {
			_windowIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(icon));
		}

		inline void Icon(HICON icon) {
			_windowIcon = icon;
		}
		
		inline void Cursor(unsigned int cursor) {
			_windowCursor = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(cursor));
		}

		inline void Cursor(HCURSOR cursor) {
			_windowCursor = cursor;
		}

		inline float CenterX() const {
			return _windowCenterX;
		}

		inline float CenterY() const {
			return _windowCenterY;
		}

		inline void CursorVisibility(bool visible) const {
			ShowCursor(visible);
		}

		inline void Close() {
			PostMessage(_windowHandle, WM_DESTROY, 0, 0);
		}

		inline COLORREF Color()	const {
			return _windowColor;
		}

		inline void Color(COLORREF color)	{
			_windowColor = color;
		}

		inline void Color(BYTE r, BYTE g, BYTE b) {
			_windowColor = RGB(r, g, b);
		}

		bool Create();
		static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:	
		HINSTANCE		_hInstance{ nullptr };
		HWND			_windowHandle{ nullptr };
		int				_windowWidth{ 800 };
		int				_windowHeight{ 600 };
		HICON			_windowIcon{ nullptr };
		HCURSOR			_windowCursor{ nullptr };
		COLORREF		_windowColor{ RGB(0,0,0) };
		String			_windowTitle{ "Xna++ Game Development" };
		DWORD			_windowStyle{ 0 };
		int				_windowPosX{ 0 };
		int				_windowPosY{ 0 };
		float			_windowCenterX{ 0 };
		float			_windowCenterY{ 0 };

	private:
		GameWindow* _gamewindow = nullptr;

		inline void setPosition() {
			_windowPosX = GetSystemMetrics(SM_CXSCREEN) / 2 - _windowWidth / 2;
			_windowPosY = GetSystemMetrics(SM_CYSCREEN) / 2 - _windowHeight / 2;
		}

		inline void setCenter() {
			_windowCenterX = _windowWidth / 2.0f;
			_windowCenterY = _windowHeight / 2.0f;
		}		
	};	
}

#endif