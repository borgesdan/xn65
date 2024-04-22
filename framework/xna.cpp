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
namespace xna {
	class Game1 : public Game {
	public:
		Game1() {
			auto _game = reinterpret_cast<Game*>(this);
			graphics = New<GraphicsDeviceManager>(_game);
			graphics->PreferredBackBufferWidth(1280);
			graphics->PreferredBackBufferHeight(720);
		}

		void Initialize() override {
			graphics->Initialize();
			Game::Initialize();
		}

		void LoadContent() override {
			spriteBatch = New<SpriteBatch>(*_graphicsDevice);

			XnaErrorCode err;
			texture = Texture2D::FromStream(*_graphicsDevice, "D:\\sprite.jpg", &err);

			auto audio = AudioEngine();

			Game::LoadContent();
		}

		void Update(GameTime const& gameTime) override {
			if (Keyboard::GetState().IsKeyDown(Keys::Escape) || GamePad::GetState(PlayerIndex::One).IsButtonDown(Buttons::Back))
				Exit();

			oldState = currentState;
			currentState = Mouse::GetState();
			const auto rec = Rectangle((graphics->PreferredBackBufferWidth() / 2) - 100, (graphics->PreferredBackBufferHeight() / 2) - 100, 200, 200);

			if (currentState.LeftButton == ButtonState::Pressed && oldState.LeftButton == ButtonState::Released) {				
				graphics->ToggleFullScreen();
			}

			if (currentState.RightButton == ButtonState::Pressed && oldState.RightButton == ButtonState::Released) {
				position.X += 50;
			}

			Game::Update(gameTime);
		}

		void Draw(GameTime const& gameTime) override {
			_graphicsDevice->Clear(Colors::CornflowerBlue);

			spriteBatch->Begin();
			spriteBatch->Draw(*texture, position, Colors::White);
			spriteBatch->End();

			Game::Draw(gameTime);
		}

	private:
		PGraphicsDeviceManager graphics = nullptr;
		PSpriteBatch spriteBatch = nullptr;
		PTexture2D texture = nullptr; //200x200
		Vector2 position{};
		std::vector<Vector2> points;
		MouseState currentState{};
		MouseState oldState{};
		float vel = 1;
		int var = 0;
	};
}


int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	
	auto game = xna::Game1();
	const auto result = game.Run();
	return result;
}
