#ifndef XNA_DEFAULT_HPP
#define XNA_DEFAULT_HPP

#include <any>
#include <cassert>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "enumerations.hpp"
#include "exception.hpp"
#include "helpers.hpp"

namespace xna {

	//
	//C# nameof
	//
#define nameof(name) std::string(#name)

	//
	// C# standard types
	//

	using Sbyte = int8_t;
	using Byte = uint8_t;
	using Short = int16_t;
	using Ushort = uint16_t;
	using Int = int32_t;
	using Uint = uint32_t;
	using Long = int64_t;
	using Ulong = uint64_t;
	using Char = char16_t;

	//
	// C# Min and Max Value
	//

	constexpr Sbyte		SbyteMaxValue = (std::numeric_limits<Sbyte>::max)();
	constexpr Sbyte		SbyteMinValue = (std::numeric_limits<Sbyte>::min)();
	constexpr Byte		ByteMaxValue = (std::numeric_limits<Byte>::max)();
	constexpr Byte		ByteMinValue = (std::numeric_limits<Byte>::min)();
	constexpr Short		ShortMaxValue = (std::numeric_limits<Short>::max)();
	constexpr Short		ShortMinValue = (std::numeric_limits<Short>::min)();
	constexpr Ushort	UshortMaxValue = (std::numeric_limits<Ushort>::max)();
	constexpr Ushort	UshortMinValue = (std::numeric_limits<Ushort>::min)();
	constexpr Int		IntMaxValue = (std::numeric_limits<Int>::max)();
	constexpr Int		IntMinValue = (std::numeric_limits<Int>::min)();
	constexpr Uint		UintMaxValue = (std::numeric_limits<Uint>::max)();
	constexpr Uint		UintMinValue = (std::numeric_limits<Uint>::min)();
	constexpr Long		LongMaxValue = (std::numeric_limits<Long>::max)();
	constexpr Long		LongMinValue = (std::numeric_limits<Long>::min)();
	constexpr Ulong		UlongMaxValue = (std::numeric_limits<Ulong>::max)();
	constexpr Ulong		UlongMinValue = (std::numeric_limits<Ulong>::min)();
	constexpr Char		CharMaxValue = (std::numeric_limits<Char>::max)();
	constexpr Char		CharMinValue = (std::numeric_limits<Char>::min)();
	constexpr float     FloatMaxValue = (std::numeric_limits<float>::max)();
	constexpr float     FloatMinValue = (std::numeric_limits<float>::min)();
	constexpr double    DoubleMaxValue = (std::numeric_limits<double>::max)();
	constexpr double    DoubleMinValue = (std::numeric_limits<double>::min)();

	//
	// C# Object
	//

	//Same as std::any
	using Object = std::any;

	//
	// About strings: https://stackoverflow.com/questions/402283/stdwstring-vs-stdstring
	//

	//Same as std::string
	using String = std::string;
	//Same as std::wstring
	using WString = std::wstring;

	//Same as std::shared_ptr
	template <typename T>
	using sptr = std::shared_ptr<T>;
	//Same as std::weak_ptr
	template <typename T>
	using wptr = std::weak_ptr<T>;
	//Same as std::unique_ptr
	template <typename T>
	using uptr = std::unique_ptr<T>;

	//Same as std::make_shared
	template <class _Ty, class... _Types>
	inline std::shared_ptr<_Ty> snew(_Types&&... _Args) {
		return std::make_shared<_Ty>(std::forward<_Types>(_Args)...);
	}

	//Same as std::make_unique
	template <class _Ty, class... _Types>
	inline std::unique_ptr<_Ty> unew(_Types&&... _Args) {
		return std::make_unique<_Ty>(std::forward<_Types>(_Args)...);
	}

	//
	// Forward
	//

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
	struct GraphicsDeviceInformation;
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
	class Effect;
	class GraphicsAdapter;
	class GraphicsDevice;
	struct PresentationParameters;
	class RenderTarget2D;
	class SwapChain;
	class Texture;
	class Texture2D;
	class Texture3D;
	class TextureCube;
	class RasterizerState;
	class SamplerState;
	class SamplerStateCollection;
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