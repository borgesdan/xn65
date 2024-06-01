#ifndef PLATFORMSTARTERKIT_GEM_HPP
#define PLATFORMSTARTERKIT_GEM_HPP

#include "xna.hpp"
#include "circle.hpp"
#include "tile.hpp"

namespace PlatformerStarterKit {
	class Level;
	class Player;

	class Gem {
	public:
		Gem(xna::sptr<PlatformerStarterKit::Level> const& level, xna::Vector2 const& position);

	public:
		static constexpr int PointValue = 30;
		static constexpr xna::Color Color = xna::Colors::White;

		constexpr xna::Vector2 Position() const {
			return basePosition + xna::Vector2(0.0f, bounce);
		}

		constexpr Circle BoundingCircle() const {
			return Circle(Position(), Tile::Width / 3.0f);
		}

		xna::sptr<PlatformerStarterKit::Level> Level() const;

		void LoadContent();
		void Update(xna::GameTime const& gameTime);
		void OnCollected(xna::sptr<Player>& collectedBy);
		void Draw(xna::GameTime const& gameTime, xna::SpriteBatch& spriteBatch);
		
		constexpr bool IsCollected() const {
			return isCollected;
		}

	private:
		xna::PTexture2D texture = nullptr;
		xna::Vector2 origin{};
		xna::PSoundEffect collectedSound = nullptr;
		xna::Vector2 basePosition{};
		float bounce{ 0.0F };
		xna::sptr<PlatformerStarterKit::Level> level = nullptr;
		bool isCollected{ false };
	};
}

#endif