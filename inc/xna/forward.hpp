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
	class Object;
	class Type;

	//Content
	class ContentManager;
	class ContentReader;
	class ContentTypeReader;
	class ContentTypeReaderManager;

	//Common
	struct BoundingBox;
	struct BoundingFrustum;
	struct BoundingSphere;
	struct Color;
	struct Curve;
	struct CurveKey;
	struct CurveKeyCollection;
	struct Matrix;
	struct Plane;
	struct Point;
	struct Quaternion;
	struct Ray;
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
	class GameServiceContainer;
	class GameComponentCollection;

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
	struct PresentationParameters;
	class RenderTarget2D;
	class SwapChain;
	class Texture;
	class Texture2D;
	class Texture3D;
	class TextureCube;
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