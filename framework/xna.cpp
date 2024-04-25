// xna.cpp : Defines the entry point for the application.
//

#include "xna.h"

using namespace std;
using namespace xna;

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
			
			std::vector<VertexPositionColor> data(3);
			data[0] = VertexPositionColor(Vector3(0.0F, 0.5F, 0.5F), Colors::AliceBlue);
			data[0] = VertexPositionColor(Vector3(0.5F, -0.5F, 0.5F), Colors::Red);
			data[0] = VertexPositionColor(Vector3(-0.5F, -0.5F, 0.5F), Colors::AliceBlue);

			VertexBuffer<VertexPositionColor> vbuffer(data);
			vbuffer.Initialize(*_graphicsDevice, &err);
			
			D3D11_BUFFER_DESC desc;
			vbuffer.dxBuffer->GetDesc(&desc);			
			

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
