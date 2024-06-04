#ifndef PLATFORMSTARTERKIT_ENEMY_HPP
#define PLATFORMSTARTERKIT_ENEMY_HPP

#include "xna/xna.hpp"
#include "animation.hpp"

namespace PlatformerStarterKit {
    //Facing direction along the X axis.
    enum class FaceDirection {
        Left = -1,
        Right = 1,
    };

    class Level;

    // A monster who is impeding the progress of our fearless adventurer.
    class Enemy {
    public:
        Enemy(xna::sptr<PlatformerStarterKit::Level> const& level, xna::Vector2 const& position, xna::String const& spriteSet);
  
    public:
        xna::sptr<PlatformerStarterKit::Level> Level() const;

        constexpr xna::Vector2 Position() const {
            return position;
        }

        xna::Rectangle BoundingRectangle() const;

        void LoadContent(xna::String const& spriteSet);
        void Update(xna::GameTime const& gameTime);
        void Draw(xna::GameTime const& gameTime, xna::SpriteBatch& spriteBatch);

    private:
        static constexpr float MaxWaitTime = 0.5f;
        static constexpr float MoveSpeed = 128.0f;

        xna::sptr<PlatformerStarterKit::Level> level;
        xna::Vector2 position{};
        xna::Rectangle localBounds{};

        xna::sptr<Animation> runAnimation = nullptr;
        xna::sptr<Animation> idleAnimation = nullptr;
        AnimationPlayer sprite{};
        FaceDirection direction = FaceDirection::Left;

        float waitTime = 0.0F;
    };
}

#endif