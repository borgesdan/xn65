// xna.cpp : Defines the entry point for the application.
//

#include "xna.h"

using namespace std;
using namespace xna;

namespace xna {
	class Game1 : public Game {
	public:
		Game1() : Game() {
			auto _game = reinterpret_cast<Game*>(this);
			graphics = New<GraphicsDeviceManager>(_game);
			graphics->PreferredBackBufferWidth(1280);
			graphics->PreferredBackBufferHeight(720);	
			contentManager = New<ContentManager>("Content", _services);
		}

		void Initialize() override {
			graphics->Initialize();
			Game::Initialize();
		}

		void LoadContent() override {
			spriteBatch = New<SpriteBatch>(*_graphicsDevice);

			XnaErrorCode err{0};
			//texture = Texture2D::FromStream(*_graphicsDevice, "D:\\sprite.jpg", &err);			
			//texture = New<Texture2D>(_graphicsDevice.get(), 256, 256);
			//std::vector<Color> data(256 * 256, 4278190080U);
			//std::vector<UINT> data(256 * 256, 0xffffffff);
			//std::vector<Uint> data(256 * 256, 4278190080U);
			//texture->SetData(data, 0, data.size());
			tx = contentManager->Load<Texture2D>("Idle");
			tx.Bind(_graphicsDevice.get());
			tx.Initialize(&err);

			Game::LoadContent();
		}

		void Update(GameTime const& gameTime) override {
			if (Keyboard::GetState().IsKeyDown(Keys::Escape) || GamePad.GetState(PlayerIndex::One).IsButtonDown(Buttons::Back))
				Exit();

			Game::Update(gameTime);
		}

		void Draw(GameTime const& gameTime) override {
			_graphicsDevice->Clear(Colors::CornflowerBlue);

			spriteBatch->Begin();
			spriteBatch->Draw(tx, position, Colors::White);
			spriteBatch->End();

			Game::Draw(gameTime);
		}

	private:
		sptr<GraphicsDeviceManager> graphics = nullptr;
		sptr<SpriteBatch> spriteBatch = nullptr;
		sptr<Texture2D> texture = nullptr; //200x200
		Vector2 position{};
		std::vector<Vector2> points;
		MouseState currentState{};
		MouseState oldState{};
		float vel = 1;
		int var = 0;		
		Texture2D tx;
	};
}


int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	xna::InitPlatform::Init();

	auto game = xna::Game1();
	const auto result = game.Run();
	return result;
}
