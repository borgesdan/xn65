// xna.cpp : Defines the entry point for the application.
//

#include "xna.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "level.hpp"
#include "gem.hpp"

using namespace std;
using namespace xna;

namespace PlatformerStarterKit {
	class Game1 : public Game {
	public:
		Game1() : Game() {
			Content()->RootDirectory("Content");
		}

		void Initialize() override {
			auto game = reinterpret_cast<Game*>(this);
			graphics = New<GraphicsDeviceManager>(game->shared_from_this());
			graphics->Initialize();

			std::any device = graphicsDevice;
			services->AddService(*typeof<GraphicsDevice>(), device);

			Game::Initialize();
		}

		void LoadContent() override {
			spriteBatch = New<SpriteBatch>(*graphicsDevice);					

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
		sptr<SpriteFont> hudFont = nullptr;
		sptr<Texture2D> winOverlay = nullptr;
		sptr<Texture2D> loseOverlay = nullptr;
		sptr<Texture2D> diedOverlay = nullptr;
		int levelIndex = -1;
		sptr<Level> level = nullptr;
		bool wasContinuePressed;
		TimeSpan WarningTime = TimeSpan::FromSeconds(30);
		static constexpr int TargetFrameRate = 60;
		static constexpr int BackBufferWidth = 1280;
		static constexpr int BackBufferHeight = 720;
		static constexpr Buttons ContinueButton = Buttons::A;
	};
}


int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	xna::Platform::Init();

	auto game = snew<PlatformerStarterKit::Game1>();
	const auto result = game->Run();
	return result;
}
