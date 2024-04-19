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
		GamePad::Initialize();
	}

	void LoadContent() override {
		spriteBatch = New<SpriteBatch>(*_graphicsDevice);

		XnaErrorCode err;
		texture = Texture2D::FromStream(*_graphicsDevice, "D:\\sprite.jpg", &err);

		Game::LoadContent();
	}

	void Update(GameTime const& gameTime) override {
		auto state = GamePad::GetState(PlayerIndex::One);
		
		if (state.IsButtonDown(Buttons::DPadRight))
			position.X += 1.0F * gameTime.ElapsedGameTime.TotalMilliseconds();
		if (state.IsButtonDown(Buttons::DPadLeft))
			position.X -= 1.0F * gameTime.ElapsedGameTime.TotalMilliseconds();

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
	std::vector<Vector2> points;
	MouseState currentState{};
	MouseState oldState{};
	float vel = 1;
};

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	auto game = Game1();
	const auto result = game.Run();
	return result;
}
