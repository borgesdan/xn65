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
public:
	Game1() {
		graphics = New<GraphicsDeviceManager>(this);

		XnaErrorCode err;
		texture = Texture2D::FromStream(*_graphicsDevice, "D:\\sprite.jpg", &err);

		spriteBatch = New<SpriteBatch>(*_graphicsDevice);
	}	

	virtual void Update(GameTime const& gameTime) override {

		Game::Update(gameTime);
	}

	virtual void Draw(GameTime const& gameTime) override {
		_graphicsDevice->Clear();

		spriteBatch->Begin();
		spriteBatch->Draw(*texture, Vector2(20, 20), nullptr, Colors::White, 0, {0,0}, 0.5F, SpriteEffects::None, 0);
		spriteBatch->End();

		Game::Draw(gameTime);
	}

private:
	PGraphicsDeviceManager graphics = nullptr;
	PSpriteBatch spriteBatch = nullptr;
	PTexture2D texture = nullptr;
};

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	/*FileStream stream("D:/VS_EXPBSLN_x64_enu.CAB");
	auto pos = stream.Position();
	auto len = stream.Length();
	pos = stream.Position();*/

	Game1 game;
	game.Run();
	return 0;
}
