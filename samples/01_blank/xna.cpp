// xna.cpp : Defines the entry point for the application.
//

#include "xna-dx/framework.hpp"
#include "csharp/io/binary.hpp"

using namespace std;
using namespace xna;

namespace xna {
	class Game1 : public Game {
	public:
		Game1() : Game() {
			Content()->RootDirectory("Content");

			short inteiro = 0xC3 | 0xA7;
			char charpa = inteiro;

			auto stream = std::make_shared<csharp::FileStream>("D:/file.bin", csharp::FileMode::Open);
			auto reader = csharp::BinaryReader(stream);
			auto bo = reader.ReadBoolean(); //reader.ReadChar()
			auto x = reader.ReadChar(); //x
			auto sb = reader.ReadSByte(); //127
			auto by = reader.ReadByte(); //255
			auto i16 = reader.ReadInt16(); //32767
			auto ui16 = reader.ReadUInt16();//65535
			auto i32 = reader.ReadInt32(); //2147483647
			auto ui32 = reader.ReadUInt32(); //4294967295
			auto i64 = reader.ReadInt64(); //9223372036854775807
			auto ui64 = reader.ReadUInt64(); //18446744073709551615
			auto str = reader.ReadString(); //The string in stream.
			auto str2 = reader.ReadString8(); //Ç de cedilha e ñ com til mas sem ¨ trema.
		}

		void Initialize() override {
			auto game = reinterpret_cast<Game*>(this);
			graphics = snew<GraphicsDeviceManager>(game->shared_from_this());			
			//graphics->Initialize();
			graphics->ApplyChanges();

			std::any device = Device();
			Services()->AddService(*typeof<GraphicsDevice>(), device);

			Game::Initialize();
		}

		void LoadContent() override {
			spriteBatch = snew<SpriteBatch>(Device());
			Game::LoadContent();
		}

		void Update(GameTime const& gameTime) override {
			if (Keyboard::GetState().IsKeyDown(Keys::Escape) || GamePad::GetState(PlayerIndex::One).IsButtonDown(Buttons::Back))
				Exit();

			Game::Update(gameTime);
		}

		void Draw(GameTime const& gameTime) override {
			Device()->Clear(Colors::CornflowerBlue);
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
