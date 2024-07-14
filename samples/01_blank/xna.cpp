// xna.cpp : Defines the entry point for the application.
//

#include "xna/xna.hpp"
#include "xna/xna-dx.hpp"

using namespace std;
using namespace xna;

namespace xna {
	class Game1 : public Game {
	public:
		Game1() : Game() {
			Content()->RootDirectory("Content");
		}

		void Initialize() override {
			auto game = reinterpret_cast<Game*>(this);
			graphics = snew<GraphicsDeviceManager>(game->shared_from_this());
			graphics->Initialize();

			std::any device = graphicsDevice;
			services->AddService(*typeof<GraphicsDevice>(), device);

			Game::Initialize();
		}

		void LoadContent() override {
			spriteBatch = snew<SpriteBatch>(graphicsDevice);
			Game::LoadContent();
		}

		void Update(GameTime const& gameTime) override {
			if (Keyboard::GetState().IsKeyDown(Keys::Escape) || GamePad::GetState(PlayerIndex::One).IsButtonDown(Buttons::Back))
				Exit();

			Game::Update(gameTime);
		}

		void Draw(GameTime const& gameTime) override {
			graphicsDevice->Clear(Colors::CornflowerBlue);
			Game::Draw(gameTime);
		}

	private:
		sptr<GraphicsDeviceManager> graphics = nullptr;
		sptr<SpriteBatch> spriteBatch = nullptr;
		sptr<Texture2D> texture = nullptr;
	};
}


int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	xna::Platform::Init();

	auto game = snew<Game1>();
	const auto result = game->Run();
	return result;
}
