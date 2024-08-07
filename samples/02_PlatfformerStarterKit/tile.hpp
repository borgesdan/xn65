#ifndef PLATFORMSTARTERKIT_TILE_HPP
#define PLATFORMSTARTERKIT_TILE_HPP

#include "headers.hpp"

namespace PlatformerStarterKit {
	// Controls the collision detection and response behavior of a tile.
	enum class TileCollision {
		Passable = 0,
		Impassable = 1,
		Platform = 2,
	};

	struct Tile {
		xna::PTexture2D Texture = nullptr;
		TileCollision Collision{};
		constexpr static int Width = 40;
		constexpr static int Height = 32;

		constexpr static xna::Vector2 Size() {
			return{ Width, Height };
		}

		constexpr Tile() = default;
		Tile(xna::PTexture2D const& texture, TileCollision collision) :
			Texture(texture), Collision(collision) {}
	};
}

#endif