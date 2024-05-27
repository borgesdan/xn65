#include "enemy.hpp"
#include "level.hpp"
#include "player.hpp"

namespace PlatformerStarterKit {
	Enemy::Enemy(xna::sptr<PlatformerStarterKit::Level> const& level, xna::Vector2 const& position, xna::String const& spriteSet):
		level(level), position(position) 
	{
		LoadContent(spriteSet);
	}

	xna::sptr<PlatformerStarterKit::Level> Enemy::Level() const
	{
		return level;
	}

	xna::Rectangle Enemy::BoundingRectangle() const
	{
		int left = static_cast<int>(std::round(position.X - sprite.Origin().X)) + localBounds.X;
		int top = static_cast<int>(std::round(position.Y - sprite.Origin().Y)) + localBounds.Y;

		return xna::Rectangle(left, top, localBounds.Width, localBounds.Height);
	}

	void Enemy::LoadContent(xna::String const& spriteSet)
	{
		auto _spriteSet = "Sprites/" + spriteSet + "/";
		runAnimation = xna::snew<Animation>(level->Content()->Load<xna::PTexture2D>(_spriteSet + "Run"), 0.1f, true);
		idleAnimation = xna::snew<Animation>(level->Content()->Load<xna::PTexture2D>(_spriteSet + "Idle"), 0.15f, true);
		sprite.PlayAnimation(idleAnimation);
		
		const auto width = static_cast<int>(idleAnimation->FrameWidth() * 0.35);
		const auto left = (idleAnimation->FrameWidth() - width) / 2;
		const auto height = static_cast<int>(idleAnimation->FrameWidth() * 0.7);
		const auto top = idleAnimation->FrameHeight() - height;
		localBounds = xna::Rectangle(left, top, width, height);
	}

	void Enemy::Update(xna::GameTime const& gameTime)
	{
		const auto elapsed = static_cast<float>(gameTime.ElapsedGameTime.TotalSeconds());
		
		const auto posX = position.X + localBounds.Width / 2 * (int)direction;
		const auto tileX = static_cast<int>(std::floor(posX / Tile::Width)) - static_cast<int>(direction);
		const auto tileY = static_cast<int>(std::floor(position.Y / Tile::Height));

		if (waitTime > 0) {
			waitTime = std::max(0.0f, waitTime - static_cast<float>(gameTime.ElapsedGameTime.TotalSeconds()));
			if (waitTime <= 0.0f) {
				direction = (FaceDirection)(-static_cast<int>(direction));
			}
		}
		else {
			if (level->GetCollision(tileX + static_cast<int>(direction), tileY - 1) == TileCollision::Impassable ||
				level->GetCollision(tileX + static_cast<int>(direction), tileY) == TileCollision::Passable) {
				waitTime = MaxWaitTime;
			}
			else {
				const auto velocity = xna::Vector2(static_cast<int>(direction) * MoveSpeed * elapsed, 0.0f);
				position = position + velocity;
			}
		}
	}
	void Enemy::Draw(xna::GameTime const& gameTime, xna::SpriteBatch& spriteBatch)
	{
		if (!level->Player()->IsAlive() ||
			level->ReachedExit() ||
			level->TimeRemaining() == xna::TimeSpan::Zero() ||
			waitTime > 0) {
			sprite.PlayAnimation(idleAnimation);
		}
		else {
			sprite.PlayAnimation(runAnimation);
		}
		
		const auto flip = static_cast<int>(direction) > 0 ? xna::SpriteEffects::FlipHorizontally : xna::SpriteEffects::None;
		sprite.Draw(gameTime, spriteBatch, position, flip);
	}
}