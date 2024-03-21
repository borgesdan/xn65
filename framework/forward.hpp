#ifndef XNA_FORWARD_HPP
#define XNA_FORWARD_HPP

#include "types.hpp"

namespace xna {
	//Framework
	class BoundingBox;
	class BoundingFrustum;
	class BoundinSphere;
	struct Color;
	class Curve;
	class CurveContinuity;
	class CurveKey;
	class CurveKeyCollection;
	struct Matrix;
	class Plane;
	struct Point;
	class Quaternion;
	class Ray;
	struct Rectangle;
	struct Vector2;
	struct Vector3;
	struct Vector4;

	//Game
	class DrawableGameComponent;
	class Game;
	class GameComponent;
	class GameComponentCollection;
	class GameTime;
	class GameWindow;
	class IDrawable;
	class IGameComponent;
	class IUpdatable;

	//Graphics
	class BlendState;
	class DisplayMode;
	class DisplayModeCollection;
	class GraphicsAdapter;
	class GraphicsDevice;
	class RenderTarget2D;
	class SwapChain;
	class Texture;
	class Texture2D;
	struct Viewport;	

	using PBoundingBox					= std::shared_ptr<BoundingBox>;
	using PBoundingFrustum				= std::shared_ptr<BoundingFrustum>;
	using PBoundinSphere				= std::shared_ptr<BoundinSphere>;
	using PColor						= std::shared_ptr<Color>;
	using PCurve						= std::shared_ptr<Curve>;
	using PCurveContinuity				= std::shared_ptr<CurveContinuity>;
	using PCurveKey						= std::shared_ptr<CurveKey>;
	using PCurveKeyCollection			= std::shared_ptr<CurveKeyCollection>;
	using PMatrix						= std::shared_ptr<Matrix>;
	using PPlane						= std::shared_ptr<Plane>;
	using PPoint						= std::shared_ptr<Point>;
	using PQuaternion					= std::shared_ptr<Quaternion>;
	using PRay							= std::shared_ptr<Ray>;
	using PRectangle					= std::shared_ptr<Rectangle>;
	using PVector2						= std::shared_ptr<Vector2>;
	using PVector3						= std::shared_ptr<Vector3>;
	using PVector4						= std::shared_ptr<Vector4>;	

	using PDrawableGameComponent		= std::shared_ptr<DrawableGameComponent>;
	using PGame							= std::shared_ptr<Game>;
	using PGameComponent				= std::shared_ptr<GameComponent>;
	using PGameComponentCollection		= std::shared_ptr<GameComponentCollection>;
	using PGameTime						= std::shared_ptr<GameTime>;
	using PGameWindow					= std::shared_ptr<GameWindow>;
	using PIDrawable					= std::shared_ptr<IDrawable>;
	using PIGameComponent				= std::shared_ptr<IGameComponent>;
	using PIUpdatable					= std::shared_ptr<IUpdatable>;

	using PBlendState = std::shared_ptr<BlendState>;
	using PDisplayMode = std::shared_ptr<DisplayMode>;
	using PDisplayModeCollection = std::shared_ptr<DisplayModeCollection>;
	using PGraphicsAdapter = std::shared_ptr<GraphicsAdapter>;
	using PGraphicsDevice = std::shared_ptr<GraphicsDevice>;
	using PRenderTarget2D = std::shared_ptr<RenderTarget2D>;
	using PSwapChain = std::shared_ptr<SwapChain>;
	using PTexture = std::shared_ptr<Texture>;
	using PTexture2D = std::shared_ptr<Texture2D>;
	using PViewport = std::shared_ptr<Viewport>;
}

#endif