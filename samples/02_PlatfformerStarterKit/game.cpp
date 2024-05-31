// xna.cpp : Defines the entry point for the application.
//

#include "xna.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "level.hpp"
#include "gem.hpp"
#include <format>

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

			// Load fonts
			hudFont = Content()->Load<PSpriteFont>("Fonts/Hud");

			// Load overlay textures
			winOverlay = Content()->Load<PTexture2D>("Overlays/you_win");
			loseOverlay = Content()->Load<PTexture2D>("Overlays/you_lose");
			diedOverlay = Content()->Load<PTexture2D>("Overlays/you_died");

			LoadNextLevel();

			Game::LoadContent();
		}

		void Update(GameTime const& gameTime) override {
			HandleInput();

			level->Update(gameTime);

			Game::Update(gameTime);
		}

		void Draw(GameTime const& gameTime) override {
			graphicsDevice->Clear(Colors::CornflowerBlue);

			spriteBatch->Begin();

			level->Draw(gameTime, *spriteBatch);

			//DrawHud();

			spriteBatch->End();

			Game::Draw(gameTime);
		}

	private:
		void HandleInput() {
			auto keyboardState = Keyboard::GetState();
			auto gamepadState = GamePad::GetState(PlayerIndex::One);
			
			if (gamepadState.Buttons.Back == ButtonState::Pressed)
				Exit();

			bool continuePressed =
				keyboardState.IsKeyDown(Keys::Space) ||
				gamepadState.IsButtonDown(ContinueButton);
			
			if (!wasContinuePressed && continuePressed) {
				if (!level->Player()->IsAlive()) {
					level->StartNewLife();
				}
				else if (level->TimeRemaining() == TimeSpan::Zero()) {
					if (level->ReachedExit())
						LoadNextLevel();
					else
						ReloadCurrentLevel();
				}
			}

			wasContinuePressed = continuePressed;
		}

		void LoadNextLevel() {
			xna::String levelPath;
			
			while (true) {				
				levelPath = std::format("Levels/{0}.txt", ++levelIndex);
				levelPath = "Content/" + levelPath;
				
				if (std::filesystem::exists(levelPath))
					break;
				
				if (levelIndex == 0)
					throw std::exception("No levels found.");
				
				levelIndex = -1;
			}			
			
			level = snew<Level>(services, levelPath);
			level->Initialize();
		}

		void ReloadCurrentLevel() {
			--levelIndex;
			LoadNextLevel();
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
		bool wasContinuePressed = false;
		TimeSpan WarningTime = TimeSpan::FromSeconds(30);
		static constexpr int TargetFrameRate = 60;
		static constexpr Buttons ContinueButton = Buttons::A;
	};
}


int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	xna::Platform::Init();

	auto game = snew<PlatformerStarterKit::Game1>();
	const auto result = game->Run();
	return result;
}
