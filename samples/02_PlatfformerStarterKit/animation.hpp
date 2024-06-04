#ifndef PLATFORMSTARTERKIT_ANIMATION_HPP
#define PLATFORMSTARTERKIT_ANIMATION_HPP

#include "xna/xna.hpp"

namespace PlatformerStarterKit {
	/*
	* Represents an animated texture.
	* Currently, this class assumes that each frame of animation is
	* as wide as each animation is tall. The number of frames in the
	* animation are inferred from this.
	*/
	class Animation {
	public:
		Animation(xna::sptr<xna::Texture2D> const& texture, float frameTime, bool isLooping):
			texture(texture), frameTime(frameTime), isLooping(isLooping){}

		inline xna::sptr<xna::Texture2D> Texture() const {
			return texture;
		}

		constexpr float FrameTime() const {
			return frameTime;
		}

		constexpr bool IsLooping() const {
			return isLooping;
		}

		inline int FrameCount() const {
			return texture->Width() / texture->Height();
		}

		inline int FrameWidth() const {
			return texture->Height();
		}

		inline int FrameHeight() const {
			return texture->Height();
		}

	private:
		xna::sptr<xna::Texture2D> texture = nullptr;
		float frameTime = 0;
		bool isLooping = false;
	};

	//Controls playback of an Animation.
	struct AnimationPlayer {
		xna::sptr<Animation> Animation() const {
			return animation;
		}

		constexpr int FrameIndex() const {
			return frameIndex;
		}

		inline xna::Vector2 Origin() const {
			return xna::Vector2(
				animation->FrameWidth() / 2.0f,
				animation->FrameHeight()
			);
		}

		void PlayAnimation(xna::sptr<PlatformerStarterKit::Animation> const& animation);
		void Draw(xna::GameTime const& gameTime, xna::SpriteBatch& spriteBatch, xna::Vector2& position, xna::SpriteEffects spriteEffects);

	private:
		xna::sptr<PlatformerStarterKit::Animation> animation = nullptr;
		int frameIndex = 0;
		float time = 0;		
	};
}

#endif