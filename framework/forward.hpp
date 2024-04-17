#ifndef XNA_FORWARD_HPP
#define XNA_FORWARD_HPP

#include "types.hpp"

namespace xna {
	//CShap
	struct TimeSpan;
	using PTimeSpan						= std::shared_ptr<TimeSpan>;
	class Stream;
	using PStream						= std::shared_ptr<Stream>;
	class FileStream;
	using PFileStream					= std::shared_ptr<FileStream>;
	class MemoryStream;
	using PMemoryStream					= std::shared_ptr<MemoryStream>;

	//Framework
	class BoundingBox;
	using PBoundingBox					= std::shared_ptr<BoundingBox>;
	class BoundingFrustum;
	using PBoundingFrustum				= std::shared_ptr<BoundingFrustum>;
	class BoundinSphere;
	using PBoundinSphere				= std::shared_ptr<BoundinSphere>;
	struct Color;
	using PColor						= std::shared_ptr<Color>;
	class Curve;
	using PCurve						= std::shared_ptr<Curve>;
	class CurveContinuity;
	using PCurveContinuity				= std::shared_ptr<CurveContinuity>;
	class CurveKey;
	using PCurveKey						= std::shared_ptr<CurveKey>;
	class CurveKeyCollection;
	using PCurveKeyCollection			= std::shared_ptr<CurveKeyCollection>;
	struct Matrix;
	using PMatrix						= std::shared_ptr<Matrix>;
	class Plane;
	using PPlane						= std::shared_ptr<Plane>;
	struct Point;
	using PPoint						= std::shared_ptr<Point>;
	class Quaternion;
	using PQuaternion					= std::shared_ptr<Quaternion>;
	class Ray;
	using PRay							= std::shared_ptr<Ray>;
	struct Rectangle;
	using PRectangle					= std::shared_ptr<Rectangle>;
	struct Vector2;
	using Vec2 = Vector2;
	using PVector2						= std::shared_ptr<Vector2>;
	using PVec2							= std::shared_ptr<Vector2>;
	struct Vector3;
	using Vec3 = Vector3;
	using PVector3						= std::shared_ptr<Vector3>;
	using PVec3							= std::shared_ptr<Vector3>;
	struct Vector4;
	using Vec4 = Vector4;
	using PVector4						= std::shared_ptr<Vector4>;
	using PVec4							= std::shared_ptr<Vector4>;

	//Game
	class DrawableGameComponent;
	using PDrawableGameComponent		= std::shared_ptr<DrawableGameComponent>;
	class Game;
	using PGame							= std::shared_ptr<Game>;
	class GameComponent;
	using PGameComponent				= std::shared_ptr<GameComponent>;
	class GameComponentCollection;
	using PGameComponentCollection		= std::shared_ptr<GameComponentCollection>;
	class GameClock;
	using PGameClock					= std::shared_ptr<GameClock>;
	class GameTime;
	using PGameTime						= std::shared_ptr<GameTime>;	
	class GameWindow;
	using PGameWindow					= std::shared_ptr<GameWindow>;
	class GraphicsDeviceInformation;
	using PGraphicsDeviceInformation = std::shared_ptr<GraphicsDeviceInformation>;
	class GraphicsDeviceManager;
	using PGraphicsDeviceManager		= std::shared_ptr<GraphicsDeviceManager>;
	class IDrawable;
	using PIDrawable					= std::shared_ptr<IDrawable>;
	class IGameComponent;
	using PIGameComponent				= std::shared_ptr<IGameComponent>;
	class IUpdatable;
	using PIUpdatable					= std::shared_ptr<IUpdatable>;	

	//Graphics
	class BlendState;
	using PBlendState					= std::shared_ptr<BlendState>;
	class ConstantBuffer;
	using PConstantBuffer				= std::shared_ptr<ConstantBuffer>;
	class DataBuffer;
	using PDataBuffer					= std::shared_ptr<DataBuffer>;
	class DepthStencilState;
	using PDepthStencilState			= std::shared_ptr<DepthStencilState>;
	class DisplayMode;
	using PDisplayMode					= std::shared_ptr<DisplayMode>;
	class DisplayModeCollection;
	using PDisplayModeCollection		= std::shared_ptr<DisplayModeCollection>;
	class GraphicsAdapter;
	using PGraphicsAdapter				= std::shared_ptr<GraphicsAdapter>;
	class GraphicsDevice;
	using PGraphicsDevice				= std::shared_ptr<GraphicsDevice>;
	class GraphicsDeviceInformation;
	using PGraphicsDeviceInformation	= std::shared_ptr<GraphicsDeviceInformation>;
	class PresentationParameters;
	using PPresentationParameters		= std::shared_ptr< PresentationParameters>;
	class RenderTarget2D;
	using PRenderTarget2D				= std::shared_ptr<RenderTarget2D>;
	class SwapChain;
	using PSwapChain					= std::shared_ptr<SwapChain>;
	class Texture;
	using PTexture						= std::shared_ptr<Texture>;
	class Texture2D;
	using PTexture2D					= std::shared_ptr<Texture2D>;
	class RasterizerState;
	using PRasterizerState				= std::shared_ptr<RasterizerState>;
	class SamplerState;
	using PSamplerState					= std::shared_ptr<SamplerState>;
	class Shader;
	using PShader						= std::shared_ptr<Shader>;
	class SpriteBatch;
	using PSpriteBatch					= std::shared_ptr<SpriteBatch>;
	class SpriteFont;
	using PSpriteFont					= std::shared_ptr<SpriteFont>;
	class VertexBuffer;
	using PVertexBuffer					= std::shared_ptr<VertexBuffer>;
	class VertexInputLayout;
	using PVertexInputLayout			= std::shared_ptr<VertexInputLayout>;
	struct VertexPositionColor;
	using PVertexPositionColor			= std::shared_ptr<VertexPositionColor>;
	class VertexShader;
	using pVertexShader					= std::shared_ptr<VertexShader>;
	struct Viewport;	
	using PViewport						= std::shared_ptr<Viewport>;

	//Input
	class GamePad;
	using PGamePad						= std::shared_ptr<GamePad>;
	struct GamePadTriggers;
	using PGamePagTriggers				= std::shared_ptr<GamePadTriggers>;
	struct GamePadThumbSticks;
	using PGamePadThumbSticks			= std::shared_ptr<GamePadThumbSticks>;
	struct GamePadDPad;
	using PGamePadDPad					= std::shared_ptr<GamePadDPad>;
	struct GamePadCapabilities;
	using PGamePadCapabilities			= std::shared_ptr<GamePadCapabilities>;
	struct GamePadButtons;
	using PGamePadButtons				= std::shared_ptr<GamePadButtons>;
	struct GamePadState;
	using PGamePadState					= std::shared_ptr<GamePadState>;
	struct KeyboardState;
	using PKeyboardState				= std::shared_ptr<KeyboardState>;
	struct MouseState;
	using PMouseState					= std::shared_ptr<MouseState>;
}

#endif