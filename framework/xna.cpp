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
			
			Content()->RootDirectory("Content");
		}

		void Initialize() override {
			graphics->Initialize();

			std::any device = graphicsDevice;
			services->AddService(*typeof<GraphicsDevice>(), device);

			Game::Initialize();
		}

		void LoadContent() override {
			spriteBatch = New<SpriteBatch>(*graphicsDevice);

			texture = Content()->Load<PTexture2D>("idlse");

			Game::LoadContent();
		}

		void Update(GameTime const& gameTime) override {
			if (Keyboard::GetState().IsKeyDown(Keys::Escape) || GamePad.GetState(PlayerIndex::One).IsButtonDown(Buttons::Back))
				Exit();

			Game::Update(gameTime);
		}

		void Draw(GameTime const& gameTime) override {
			graphicsDevice->Clear(Colors::CornflowerBlue);			

			spriteBatch->Begin();
			spriteBatch->Draw(*texture, Vector2(10, 10), Colors::White);
			spriteBatch->End();

			Game::Draw(gameTime);
		}

	private:
		sptr<GraphicsDeviceManager> graphics = nullptr;
		sptr<SpriteBatch> spriteBatch = nullptr;
		PTexture2D texture = nullptr;
	};
}


int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	xna::InitPlatform::Init();

	auto game = xna::Game1();
	const auto result = game.Run();
	return result;
}
