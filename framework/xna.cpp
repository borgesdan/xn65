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
		auto _game = reinterpret_cast<Game*>(this);
		graphics = New<GraphicsDeviceManager>(_game);
	}

	void Initialize() override {
		graphics->Initialize();

		Game::Initialize();
	}

	void LoadContent() override {
		spriteBatch = New<SpriteBatch>(*_graphicsDevice);

		XnaErrorCode err;
		texture = Texture2D::FromStream(*_graphicsDevice, "D:\\sprite.jpg", &err);

		Game::LoadContent();
	}

	void Update(GameTime const& gameTime) override {

		const auto state = Keyboard::GetState();

		if (state.IsKeyDown(Keys::Right)) {
			position.X += 1 * gameTime.ElapsedGameTime.TotalMilliseconds();
		}
		if (state.IsKeyDown(Keys::Left)) {
			position.X -= 1 * gameTime.ElapsedGameTime.TotalMilliseconds();
		}
		if (state.IsKeyDown(Keys::Up)) {
			position.Y -= 1 * gameTime.ElapsedGameTime.TotalMilliseconds();
		}
		if (state.IsKeyDown(Keys::Down)) {
			position.Y += 1 * gameTime.ElapsedGameTime.TotalMilliseconds();
		}

		/*if (position.X > 1280 || position.X < 0)
			vel *= -1;

		if (gameTime.ElapsedGameTime.TotalMilliseconds() > 1) {

		}

		position.X += 0.05 * (gameTime.ElapsedGameTime.TotalMilliseconds() * vel);*/
		//position.X += 2 * vel;

		Game::Update(gameTime);
	}

	void Draw(GameTime const& gameTime) override {
		_graphicsDevice->Clear(Colors::CornflowerBlue);

		spriteBatch->Begin();
		spriteBatch->Draw(*texture, position, nullptr, Colors::White, 0, { 0,0 }, 0.5F, SpriteEffects::None, 0);
		spriteBatch->End();

		Game::Draw(gameTime);
	}

private:
	PGraphicsDeviceManager graphics = nullptr;
	PSpriteBatch spriteBatch = nullptr;
	PTexture2D texture = nullptr;
	Vector2 position{};
	float vel = 1;
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
