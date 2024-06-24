// xna.cpp : Defines the entry point for the application.
//

#include "xna/xna.hpp"
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
			srand(354668);
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
			
			DrawHud();			

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

		void DrawHud()
		{
			auto titleSafeArea = graphicsDevice->Viewport().Bounds();
			auto hudLocation = Vector2(titleSafeArea.X, titleSafeArea.Y);
			auto center = Vector2(titleSafeArea.X + titleSafeArea.Width / 2.0f,
				titleSafeArea.Y + titleSafeArea.Height / 2.0f);
			
			string timeString = "TIME: " + to_string(level->TimeRemaining().Minutes()) + ":" + to_string(level->TimeRemaining().Seconds());			
			Color timeColor;
			if (level->TimeRemaining() > WarningTime ||
				level->ReachedExit() ||
				static_cast<int>(level->TimeRemaining().TotalSeconds()) % 2 == 0)
			{
				timeColor = Colors::Yellow;
			} else {
				timeColor = Colors::Red;
			}
			DrawShadowedString(*hudFont, timeString, hudLocation, timeColor);

			
			float timeHeight = hudFont->MeasureString(timeString).Y;
			DrawShadowedString(*hudFont, "SCORE: " + to_string(level->Score()), hudLocation + Vector2(0.0f, timeHeight * 1.2f), Colors::Yellow);
			
			PTexture2D status = nullptr;
			if (level->TimeRemaining() == TimeSpan::Zero())
			{
				if (level->ReachedExit()) {
					status = winOverlay;
				} else {
					status = loseOverlay;
				}
			}
			else if (!level->Player()->IsAlive()) {
				status = diedOverlay;
			}

			if (status)
			{				
				const auto statusSize = Vector2(status->Width(), status->Height());
				spriteBatch->Draw(status, center - statusSize / 2, Colors::White);
			}
		}

		void DrawShadowedString(SpriteFont& font, String const& value, Vector2 const& position, Color const& color)
		{
			spriteBatch->DrawString(font, value, position + Vector2(1.0f, 1.0f), Colors::Black);
			spriteBatch->DrawString(font, value, position, color);
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
