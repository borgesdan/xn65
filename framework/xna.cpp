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

class Game1 : public Game {
	virtual void Update(GameTime const& gameTime) {

		Game::Update(gameTime);
	}

	virtual void Draw(GameTime const& gameTime) {
		_graphicsDevice->Clear();

		Game::Draw(gameTime);
	}
};

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {	
	/*Game game;
	game.Run();*/
	Game1 game;
	game.Run();
	return 0;
}
