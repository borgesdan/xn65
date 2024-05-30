#include "player.hpp"
#include "level.hpp"
#include "enemy.hpp"
#include "extensions.hpp"

namespace PlatformerStarterKit {
    Player::Player(xna::sptr<PlatformerStarterKit::Level> const& level, xna::Vector2 const& position)
        : level(level)
    {
        LoadContent();
        Reset(position);
    }

    void Player::LoadContent()
    {
        idleAnimation = xna::snew<Animation>(level->Content()->Load<xna::PTexture2D>("Sprites/Player/Idle"), 0.1f, true);
        runAnimation = xna::snew<Animation>(level->Content()->Load<xna::PTexture2D>("Sprites/Player/Run"), 0.1f, true);
        jumpAnimation = xna::snew<Animation>(level->Content()->Load<xna::PTexture2D>("Sprites/Player/Jump"), 0.1f, false);
        celebrateAnimation = xna::snew<Animation>(level->Content()->Load<xna::PTexture2D>("Sprites/Player/Celebrate"), 0.1f, false);
        dieAnimation = xna::snew<Animation>(level->Content()->Load<xna::PTexture2D>("Sprites/Player/Die"), 0.1f, false);
        
        const auto width = static_cast<int>(idleAnimation->FrameWidth() * 0.4);
        const auto left = (idleAnimation->FrameWidth() - width) / 2;
        const auto height = static_cast<int>(idleAnimation->FrameWidth() * 0.8);
        const auto top = idleAnimation->FrameHeight() - height;
        localBounds = xna::Rectangle(left, top, width, height);
                
        killedSound = level->Content()->Load<xna::PSoundEffect>("Sounds/PlayerKilled");
        jumpSound = level->Content()->Load<xna::PSoundEffect>("Sounds/PlayerJump");
        fallSound = level->Content()->Load<xna::PSoundEffect>("Sounds/PlayerFall");
    }

    void Player::Reset(xna::Vector2 const& position)
    {
        Position = position;
        Velocity = xna::Vector2::Zero();
        isAlive = true;
        sprite.PlayAnimation(idleAnimation);
    }

    void Player::ApplyPhysics(xna::GameTime const& gameTime)
    {
        float elapsed = static_cast<float>(gameTime.ElapsedGameTime.TotalSeconds());

        auto previousPosition = Position;
               
        Velocity.X += movement * MoveAcceleration * elapsed;
        Velocity.Y = xna::MathHelper::Clamp(Velocity.Y + GravityAcceleration * elapsed, -MaxFallSpeed, MaxFallSpeed);

        Velocity.Y = DoJump(Velocity.Y, gameTime);
                
        if (IsOnGround())
            Velocity.X *= GroundDragFactor;
        else
            Velocity.X *= AirDragFactor;
        
        Velocity.X = xna::MathHelper::Clamp(Velocity.X, -MaxMoveSpeed, MaxMoveSpeed);
        
        Position = Position + (Velocity * elapsed);
        Position = xna::Vector2(std::round(Position.X), std::round(Position.Y));
        
        HandleCollisions();
        
        if (Position.X == previousPosition.X)
            Velocity.X = 0;

        if (Position.Y == previousPosition.Y)
            Velocity.Y = 0;
    }

    void Player::Update(xna::GameTime const& gameTime)
    {
        GetInput();

        ApplyPhysics(gameTime);

        if (IsAlive() && IsOnGround()) {
            if (std::abs(Velocity.X) - 0.02f > 0) {
                sprite.PlayAnimation(runAnimation);
            }
            else {
                sprite.PlayAnimation(idleAnimation);
            }
        }
        
        movement = 0.0f;
        isJumping = false;
    }

    void Player::GetInput()
    {
        auto gamePadState = xna::GamePad::GetState(xna::PlayerIndex::One);
        auto keyboardState = xna::Keyboard::GetState();
        
        movement = gamePadState.ThumbSticks.Left().X * MoveStickScale;
                
        if (std::abs(movement) < 0.5f)
            movement = 0.0f;
        
        if (gamePadState.IsButtonDown(xna::Buttons::DPadLeft) ||
            keyboardState.IsKeyDown(xna::Keys::Left) ||
            keyboardState.IsKeyDown(xna::Keys::A)) {
            movement = -1.0f;
        }
        else if (gamePadState.IsButtonDown(xna::Buttons::DPadRight) ||
            keyboardState.IsKeyDown(xna::Keys::Right) ||
            keyboardState.IsKeyDown(xna::Keys::D)) {
            movement = 1.0f;
        }
        
        isJumping =
            gamePadState.IsButtonDown(JumpButton) ||
            keyboardState.IsKeyDown(xna::Keys::Space) ||
            keyboardState.IsKeyDown(xna::Keys::Up) ||
            keyboardState.IsKeyDown(xna::Keys::W);
    }

    void Player::OnKilled(xna::sptr<Enemy>& killedBy)
    {
        isAlive = false;

        if (!killedBy)
            killedSound->Play();
        else
            fallSound->Play();

        sprite.PlayAnimation(dieAnimation);
    }

    void Player::Draw(xna::GameTime const& gameTime, xna::SpriteBatch& spriteBatch)
    {
        if (Velocity.X > 0)
            flip = xna::SpriteEffects::FlipHorizontally;
        else if (Velocity.X < 0)
            flip = xna::SpriteEffects::None;
        
        sprite.Draw(gameTime, spriteBatch, Position, flip);
    }

    void Player::OnReachedExit()
    {
        sprite.PlayAnimation(celebrateAnimation);
    }

    float Player::DoJump(float velocityY, xna::GameTime const& gameTime)
    {
        if (isJumping) {
            if ((!wasJumping && IsOnGround()) || jumpTime > 0.0f) {
                if (jumpTime == 0.0f)
                    jumpSound->Play();

                jumpTime += static_cast<float>(gameTime.ElapsedGameTime.TotalSeconds());
                sprite.PlayAnimation(jumpAnimation);
            }
            
            if (0.0f < jumpTime && jumpTime <= MaxJumpTime) {                
                velocityY = JumpLaunchVelocity * (1.0f - static_cast<float>(std::pow(jumpTime / MaxJumpTime, JumpControlPower)));
            }
            else {
                jumpTime = 0.0f;
            }
        }
        else {
            jumpTime = 0.0f;
        }
        wasJumping = isJumping;

        return velocityY;
    }

    void Player::HandleCollisions()
    {
        auto bounds = BoundingRectangle();
        auto leftTile = std::floor(static_cast<float>(bounds.Left()) / Tile::Width);
        auto rightTile = std::ceil((static_cast<float>(bounds.Right()) / Tile::Width)) - 1;
        auto topTile = std::floor(static_cast<float>(bounds.Top()) / Tile::Height);
        auto bottomTile = std::ceil((static_cast<float>(bounds.Bottom()) / Tile::Height)) - 1;
        
        isOnGround = false;
        
        for (size_t y = topTile; y <= bottomTile; ++y) {
            for (size_t x = leftTile; x <= rightTile; ++x) {                
                auto collision = level->GetCollision(x, y);
                if (collision != TileCollision::Passable) {                    
                    auto tileBounds = level->GetBounds(x, y);
                    auto depth = RectangleExtensions::GetIntersectionDepth(bounds, tileBounds);
                    if (depth != xna::Vector2::Zero()) {
                        auto absDepthX = std::abs(depth.X);
                        auto absDepthY = std::abs(depth.Y);
                                                
                        if (absDepthY < absDepthX || collision == TileCollision::Platform) {
                            if (previousBottom <= tileBounds.Top())
                                isOnGround = true;
                            
                            if (collision == TileCollision::Impassable || IsOnGround()) {
                                Position = xna::Vector2(Position.X, Position.Y + depth.Y);
                                bounds = BoundingRectangle();
                            }
                        }
                        else if (collision == TileCollision::Impassable)
                        {
                            Position = xna::Vector2(Position.X + depth.X, Position.Y);
                            bounds = BoundingRectangle();
                        }
                    }
                }
            }
        }
        
        previousBottom = bounds.Bottom();
    }

    xna::sptr<PlatformerStarterKit::Level> Player::Level() const
    {
        return level;
    }

    xna::Rectangle Player::BoundingRectangle() const {
        auto left = std::round(Position.X - sprite.Origin().X) + localBounds.X;
        auto top = std::round(Position.Y - sprite.Origin().Y) + localBounds.Y;

        return xna::Rectangle(
            static_cast<int>(left),
            static_cast<int>(top),
            localBounds.Width,
            localBounds.Height);
    }
}