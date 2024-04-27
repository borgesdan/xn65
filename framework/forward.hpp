#ifndef XNA_FORWARD_HPP
#define XNA_FORWARD_HPP

#include "types.hpp"

namespace xna {
	//Audio
	class SoundEffect;
	struct SoundEffectInstance;
	class AudioEngine;
	struct WaveFormat;

	//CShap
	struct TimeSpan;
	class Stream;
	class FileStream;
	class MemoryStream;

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
	struct Quaternion;
	class Ray;
	struct Rectangle;
	struct Vector2;	
	struct Vector3;	
	struct Vector4;	

	//Game	
	class Game;
	class GameComponent;
	class GameClock;
	class GameTime;
	class GameWindow;
	class GraphicsDeviceInformation;
	class GraphicsDeviceManager;
	class IGameTime;
	class IGameComponent;

	//Graphics
	class BlendState;
	class ConstantBuffer;
	class DataBuffer;
	class DepthStencilState;
	class DisplayMode;
	class DisplayModeCollection;
	class GraphicsAdapter;
	class GraphicsDevice;
	class GraphicsDeviceInformation;
	class PresentationParameters;
	class RenderTarget2D;
	class SwapChain;
	class Texture;
	class Texture2D;
	class RasterizerState;
	class SamplerState;
	class Shader;
	class SpriteBatch;
	class SpriteFont;
	struct VertexPositionColor;
	class VertexShader;
	struct Viewport;	

	//Input	
	struct GamePadTriggers;
	struct GamePadThumbSticks;
	struct GamePadDPad;
	struct GamePadCapabilities;
	struct GamePadButtons;
	struct GamePadState;
	struct KeyboardState;
	struct MouseState;
}

#endif