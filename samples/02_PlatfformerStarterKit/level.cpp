#include "enemy.hpp"
#include "extensions.hpp"
#include "level.hpp"
#include "player.hpp"
#include "gem.hpp"
#include <cstdlib>
#include <fstream>
#include <string>

namespace PlatformerStarterKit {
	Level::Level(xna::sptr<xna::IServiceProvider> const& serviceProvider, xna::String const& path) : path(path)
	{
		srand(354668);
		
		content = xna::snew<xna::ContentManager>("Content", serviceProvider);
		timeRemaining = xna::TimeSpan::FromMinutes(2.0);

		//LoadTiles(path);

		layers = std::vector<xna::PTexture2D>(3);

		for (size_t i = 0; i < layers.size(); ++i) {			
			const auto segmentIndex = rand() % 3;
            xna::String file = "Backgrounds/Layer" + std::to_string(i) + "_" + std::to_string(segmentIndex);
			layers[i] = content->Load<xna::PTexture2D>(file);

			exitReachedSound = content->Load<xna::PSoundEffect>("Sounds/ExitReached");
		}
	}
    xna::sptr<PlatformerStarterKit::Player> Level::Player() const
    {
        return player;
    }

	void Level::LoadTiles(xna::String const& path)
	{
        int width = 0;
        std::vector<xna::String> lines;

        std::fstream reader(path);
        xna::String line;
        std::getline(reader, line);
        width = line.size();

        while (reader.good()) {
            if (line.size() != width)
                std::exception("The length of line {0} is different from all preceeding lines.");

            lines.push_back(line);
            line.clear();
            std::getline(reader, line);
        }
        
        tiles = std::vector<std::vector<Tile>>(width, std::vector<Tile>(lines.size()));        

        for (size_t y = 0; y < lines.size(); ++y) {
            for (size_t x = 0; x < width; ++x) {

                auto tileType = lines[y][x];

                tiles[x][y] = LoadTile(tileType, x, y);
            }
        }
       
        if (!player)
            std::exception("A level must have a starting point.");
        if (exit == InvalidPosition)
            std::exception("A level must have an exit.");
	}

    Tile Level::LoadTile(char tileType, int x, int y) {
        switch (tileType) {
        case '.':
            return Tile(nullptr, TileCollision::Passable);
        case 'X':
            return LoadExitTile(x, y);
        case 'G':
            return LoadGemTile(x, y);
        case '-':
            return LoadTile("Platform", TileCollision::Platform);
        case 'A':
            return LoadEnemyTile(x, y, "MonsterA");
        case 'B':
            return LoadEnemyTile(x, y, "MonsterB");
        case 'C':
            return LoadEnemyTile(x, y, "MonsterC");
        case 'D':
            return LoadEnemyTile(x, y, "MonsterD");
        case '~':
            return LoadVarietyTile("BlockB", 2, TileCollision::Platform);
        case ':':
            return LoadVarietyTile("BlockB", 2, TileCollision::Passable);
        case '1':
            return LoadStartTile(x, y);
        case '#':
            return LoadVarietyTile("BlockA", 7, TileCollision::Impassable);
        default:
            std::exception("Unsupported tile type character");
        }
    }

    Tile Level::LoadTile(xna::String const& name, TileCollision collision) {
        return Tile(content->Load<xna::PTexture2D>("Tiles/" + name), collision);
    }

    Tile Level::LoadVarietyTile(xna::String const& baseName, int variationCount, TileCollision collision) {
        auto index = rand() % variationCount;
        return LoadTile(baseName + std::to_string(index), collision);
    }

    Tile Level::LoadStartTile(int x, int y) {
        if (player != nullptr)
            std::exception("A level may only have one starting point.");

        start = RectangleExtensions::GetBottomCenter(GetBounds(x, --y));
        const auto _this = shared_from_this();
        player = xna::snew<PlatformerStarterKit::Player>(_this, start);

        return Tile(nullptr, TileCollision::Passable);
    }

    Tile Level::LoadExitTile(int x, int y) {
        if (exit != InvalidPosition)
            std::exception("A level may only have one exit.");

        exit = GetBounds(x, y).Center();

        return LoadTile("Exit", TileCollision::Passable);
    }

    Tile Level::LoadEnemyTile(int x, int y, xna::String const& spriteSet) {
        const auto position = RectangleExtensions::GetBottomCenter(GetBounds(x, y));
        const auto _this = shared_from_this();
        enemies.push_back(xna::snew<Enemy>(_this, position, spriteSet));

        return Tile(nullptr, TileCollision::Passable);
    }

    Tile Level::LoadGemTile(int x, int y) {
        const auto position = GetBounds(x, y).Center();
        const auto _this = shared_from_this();
        gems.push_back(xna::snew<Gem>(_this, xna::Vector2(position.X, position.Y)));

        return Tile(nullptr, TileCollision::Passable);
    }

    void Level::UpdateGems(xna::GameTime const& gameTime)
    {
        for (size_t i = 0; i < gems.size(); ++i) {
            auto& gem = gems[i];

            if (gem->IsCollected())
                continue;

            gem->Update(gameTime);            

            if (gem->BoundingCircle().Intersects(player->BoundingRectangle())) {                
                OnGemCollected(gem, player);
            }
        }
    }

    void Level::OnPlayerKilled(xna::sptr<Enemy> killedBy)
    {
        player->OnKilled(killedBy);
    }

    void Level::UpdateEnemies(xna::GameTime const& gameTime)
    {
        for (size_t i = 0; i < enemies.size(); ++i) {
            auto& enemy = enemies[i];

            enemy->Update(gameTime);

            if (enemy->BoundingRectangle().Intersects(player->BoundingRectangle())) {
                OnPlayerKilled(enemy);
            }
        }
    }

    void Level::OnExitReached()
    {
        player->OnReachedExit();
        exitReachedSound->Play();
        reachedExit = true;
    }

    void Level::DrawTiles(xna::SpriteBatch& spriteBatch)
    {
        for (size_t y = 0; y < Height(); ++y) {
            for (size_t x = 0; x < Width(); ++x) {                
                auto& texture = tiles[x][y].Texture;
                if (texture) { 
                    const auto s = xna::Vector2(x, y);
                    const auto position = s * Tile::Size();
                    spriteBatch.Draw(texture, position, xna::Colors::White);
                }
            }
        }
    }

    void Level::OnGemCollected(xna::sptr<Gem>& gem, xna::sptr<PlatformerStarterKit::Player>& collectedBy)
    {
        score += Gem::PointValue;
        gem->OnCollected(collectedBy);
    }

    void Level::Initialize()
    {
        LoadTiles(path);
    }

    void Level::Update(xna::GameTime const& gameTime) {
        if (!player->IsAlive() || timeRemaining == xna::TimeSpan::Zero()) {            
            player->ApplyPhysics(gameTime);
        }
        else if (reachedExit) {
            auto seconds = static_cast<int64_t>(std::round(gameTime.ElapsedGameTime.TotalSeconds() * 100.0f));
            seconds = std::min(seconds, static_cast<int64_t>(std::ceil(timeRemaining.TotalSeconds())));
            timeRemaining = timeRemaining - xna::TimeSpan::FromSeconds(seconds);
            score += seconds * PointsPerSecond;
        }
        else {
            timeRemaining = timeRemaining - gameTime.ElapsedGameTime;

            player->Update(gameTime);

            UpdateGems(gameTime);
            
            auto playerBounds = player->BoundingRectangle();
            auto heigth = Height();

            if (playerBounds.Top() >= heigth * Tile::Height)
                OnPlayerKilled(nullptr);

            UpdateEnemies(gameTime);
         
            if (player->IsAlive() &&
                player->IsOnGround() &&
                player->BoundingRectangle().Contains(exit)) {
                OnExitReached();
            }
        }        
        
        if (timeRemaining < xna::TimeSpan::Zero())
            timeRemaining = xna::TimeSpan::Zero();
    }

    void Level::Draw(xna::GameTime const& gameTime, xna::SpriteBatch& spriteBatch)
    {
        for (size_t i = 0; i <= EntityLayer; ++i) {
            auto& layer = layers[i];
            spriteBatch.Draw(layer, xna::Vector2::Zero(), xna::Colors::White);
        }

        DrawTiles(spriteBatch);

        for (size_t i = 0; i < gems.size(); ++i) {
            auto& gem = gems[i];

            if (gem->IsCollected())
                continue;

            gem->Draw(gameTime, spriteBatch);
        }        

        player->Draw(gameTime, spriteBatch);

        for (size_t i = 0; i < enemies.size(); ++i) {
            auto& enemy = enemies[i];
            enemy->Draw(gameTime, spriteBatch);
        }        

        for (size_t i = EntityLayer + 1; i < layers.size(); ++i) {
            auto& layer = layers[i];
            spriteBatch.Draw(layer, xna::Vector2::Zero(), xna::Colors::White);
        }
            
    }

    void Level::StartNewLife() {
        player->Reset(start);
    }
}