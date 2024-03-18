// xna.cpp : Defines the entry point for the application.
//

#include "xna.h"

using namespace std;
using namespace xna;

//int main()
//{
//	cout << "Hello CMake." << endl;
//	return 0;
//}

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {	

	GameWindow gw;
	MSG  msg = { 0 };	
	gw.ip_GameWindow->Color(255, 155, 55);
	gw.Title("Teste de título");
	gw.ip_GameWindow->Create();

	GraphicsDevice device;
	device.Initialize(gw);	

	do {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			device.Clear();
			device.Present();
		}

	} while (msg.message != WM_QUIT);

	return 0;
}
