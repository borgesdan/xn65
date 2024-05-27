#ifndef PLATFORMSTARTERKIT_PLAYER_HPP
#define PLATFORMSTARTERKIT_PLAYER_HPP

#include "xna.hpp"
#include "animation.hpp"

namespace PlatformerStarterKit {
    class Level;
    class Enemy;

    // Our fearless adventurer!
	class Player {
    public:
        Player(xna::sptr<Level> const& level, xna::Vector2 const& position);        

	public:
        xna::sptr<PlatformerStarterKit::Level> Level() const;

        constexpr bool IsAlive() const {
            return isAlive;
        }          

        constexpr bool IsOnGround() const {
            return isOnGround;
        }

        xna::Rectangle BoundingRectangle() const;

        void LoadContent();
        void Reset(xna::Vector2 const& position);
        void ApplyPhysics(xna::GameTime const& gameTime);
        void Update(xna::GameTime const& gameTime);
        void GetInput();
        void OnKilled(xna::sptr<Enemy>& killedBy);
        void Draw(xna::GameTime const& gameTime, xna::SpriteBatch& spriteBatch);
        void OnReachedExit();

    public:
        xna::Vector2 Position{};
        xna::Vector2 Velocity{};

	private:
        xna::sptr<Animation> idleAnimation = nullptr;
        xna::sptr<Animation> runAnimation = nullptr;
        xna::sptr<Animation> jumpAnimation = nullptr;
        xna::sptr<Animation> celebrateAnimation = nullptr;
        xna::sptr<Animation> dieAnimation = nullptr;
        xna::SpriteEffects flip = xna::SpriteEffects::None;
        xna::sptr<AnimationPlayer> sprite = nullptr;
        xna::sptr<xna::SoundEffect> killedSound = nullptr;
        xna::sptr<xna::SoundEffect> jumpSound = nullptr;
        xna::sptr<xna::SoundEffect> fallSound = nullptr;
        xna::sptr<PlatformerStarterKit::Level> level = nullptr;
        bool isAlive = false;
        float previousBottom = 0.0F;
        bool isOnGround = false;
        float movement = 0.0f;
        bool isJumping = false;
        bool wasJumping = false;
        float jumpTime = false;
        xna::Rectangle localBounds{};

        static constexpr float MoveAcceleration = 14000.0f;
        static constexpr float MaxMoveSpeed = 2000.0f;
        static constexpr float GroundDragFactor = 0.58f;
        static constexpr float AirDragFactor = 0.65f;        
        static constexpr float MaxJumpTime = 0.35f;
        static constexpr float JumpLaunchVelocity = -4000.0f;
        static constexpr float GravityAcceleration = 3500.0f;
        static constexpr float MaxFallSpeed = 600.0f;
        static constexpr float JumpControlPower = 0.14f;        
        static constexpr float MoveStickScale = 1.0f;
        static constexpr xna::Buttons JumpButton = xna::Buttons::A;

        float DoJump(float velocityY, xna::GameTime const& gameTime);
        void HandleCollisions();                
	};
}

#endif