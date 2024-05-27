#include "animation.hpp"

namespace PlatformerStarterKit {
	void AnimationPlayer::PlayAnimation(xna::sptr<PlatformerStarterKit::Animation> const& animation) {
		if (this->animation == animation)
			return;

		// Start the new animation.
		this->animation = animation;
		this->frameIndex = 0;
		this->time = 0.0f;
	}

	void AnimationPlayer::Draw(xna::GameTime const& gameTime, xna::SpriteBatch& spriteBatch, xna::Vector2& position, xna::SpriteEffects spriteEffects) {
		if (animation == nullptr)
			std::exception();

		// Process passing time.
		time += (float)gameTime.ElapsedGameTime.TotalSeconds();

		while (time > animation->FrameTime()) {
			time -= animation->FrameTime();

			// Advance the frame index; looping or clamping as appropriate.
			if (animation->IsLooping()) {
				frameIndex = (frameIndex + 1) % animation->FrameCount();
			}
			else {
				frameIndex = std::min(frameIndex + 1, animation->FrameCount() - 1);
			}
		}

		// Calculate the source rectangle of the current frame.
		const auto source = xna::Rectangle(frameIndex * animation->Texture()->Height(), 0, animation->Texture()->Height(), animation->Texture()->Height());

		// Draw the current frame.
		spriteBatch.Draw(animation->Texture(), position, &source, xna::Colors::White, 0.0f, Origin(), 1.0f, spriteEffects, 0.0f);
	}
}