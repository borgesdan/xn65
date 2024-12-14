#ifndef PLATFORMSTARTERKIT_LEVEL_HPP
#define PLATFORMSTARTERKIT_LEVEL_HPP

#include "headers.hpp"
#include "tile.hpp"
#include "csharp/time.hpp"

namespace PlatformerStarterKit {

	class Player;
	class Gem;
	class Enemy;

	/*
	* A uniform grid of tiles with collections of gems and enemies.
	* The level owns the player and controls the game's win and lose
	* conditions as well as scoring.
	*/
	class Level : public std::enable_shared_from_this<Level> {		
	public:
		Level(xna::sptr<xna::IServiceProvider> const& serviceProvider, xna::String const& path );

	public:
		xna::sptr<PlatformerStarterKit::Player> Player() const;

		constexpr int Score() const {
			return score;
		}

		constexpr bool ReachedExit() const {
			return reachedExit;
		}

		constexpr csharp::TimeSpan TimeRemaining() const {
			return timeRemaining;
		}

		xna::sptr<xna::ContentManager> Content() const {
			return content;
		}

		constexpr xna::Rectangle GetBounds(int x, int y) const {
			return xna::Rectangle(x * Tile::Width, y * Tile::Height, Tile::Width, Tile::Height);
		}

		constexpr TileCollision GetCollision(int x, int y) const {			
			if (x < 0 || x >= Width())
				return TileCollision::Impassable;			
			if (y < 0 || y >= Height())
				return TileCollision::Passable;

			return tiles[x][y].Collision;
		}

		constexpr int Width() const {
			return tiles.size();
		}

		constexpr int Height() const {
			return tiles[0].size();
		}

		void Initialize();
		void Update(xna::GameTime const& gameTime);
		void Draw(xna::GameTime const& gameTime, xna::SpriteBatch& spriteBatch);
		void StartNewLife();

	private:
		static constexpr xna::Point InvalidPosition = xna::Point(-1, -1);
		static constexpr int PointsPerSecond = 5;

		std::vector<std::vector<Tile>> tiles;
		std::vector<xna::PTexture2D> layers;
		static constexpr int EntityLayer = 2;
		xna::sptr<PlatformerStarterKit::Player> player = nullptr;
		std::vector<xna::sptr<Gem>> gems;
		std::vector<xna::sptr<Enemy>> enemies;
		
		xna::Vector2 start{};
		xna::Point exit = InvalidPosition;		
		int score = 0;
		bool reachedExit = false;
		csharp::TimeSpan timeRemaining{};	
		xna::sptr<xna::ContentManager> content = nullptr;
		xna::sptr<xna::SoundEffect> exitReachedSound = nullptr;
		xna::String path;

	private:
		void LoadTiles(xna::String const& path);
		Tile LoadTile(char tileType, int x, int y);
		Tile LoadTile(xna::String const& name, TileCollision collision);
		Tile LoadVarietyTile(xna::String const& baseName, int variationCount, TileCollision collision);
		Tile LoadStartTile(int x, int y);
		Tile LoadExitTile(int x, int y);
		Tile LoadEnemyTile(int x, int y, xna::String const& spriteSet);
		Tile LoadGemTile(int x, int y);
		void UpdateGems(xna::GameTime const& gameTime);
		void OnPlayerKilled(xna::sptr<Enemy> killedBy);
		void UpdateEnemies(xna::GameTime const& gameTime);
		void OnExitReached();
		void DrawTiles(xna::SpriteBatch& spriteBatch);
		void OnGemCollected(xna::sptr<Gem>& gem, xna::sptr<PlatformerStarterKit::Player>& collectedBy);
	};
}

#endif