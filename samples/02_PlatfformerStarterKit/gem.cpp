#include "gem.hpp"
#include "level.hpp"

namespace PlatformerStarterKit {
	Gem::Gem(xna::sptr<PlatformerStarterKit::Level> const& level, xna::Vector2 const& position) :
		level(level), basePosition(position){
		LoadContent();
	}

	void Gem::LoadContent()
	{
		texture = level->Content()->Load<xna::PTexture2D>("Sprites/Gem");
		origin = xna::Vector2(texture->Width() / 2.0f, texture->Height() / 2.0f);
		collectedSound = level->Content()->Load<xna::PSoundEffect>("Sounds/GemCollected");
	}

	void Gem::Update(xna::GameTime const& gameTime)
	{		
		constexpr float BounceHeight = 0.18f;
		constexpr float BounceRate = 3.0f;
		constexpr float BounceSync = -0.75f;
		
		const auto t = gameTime.TotalGameTime.TotalSeconds()
			* BounceRate + Position().X * BounceSync;

		bounce = static_cast<float>(std::sin(t)) * BounceHeight * texture->Height();
	}

	void Gem::OnCollected(xna::sptr<Player>& collectedBy)
	{
		collectedSound->Play();
	}

	void Gem::Draw(xna::GameTime const& gameTime, xna::SpriteBatch& spriteBatch)
	{
		spriteBatch.Draw(texture, Position(), nullptr, Color, 0.0f, origin, 1.0f, xna::SpriteEffects::None, 0.0f);
	}
}