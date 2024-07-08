#ifndef XNA_PLATFORMDX_DX_HPP
#define XNA_PLATFORMDX_DX_HPP

//---------------- DX INCLUDES ----------------//

//DirectX
#if defined(_XBOX_ONE) && defined(_TITLE)
#include <d3d11_x.h>
#define NO_D3D11_DEBUG_NAME
#endif
#include "dxgi.h"
#include "d3d11.h"
#include <d3d11_1.h>
#include <d3d11_2.h>
//HSLS AND EFFECTS
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include "effects11/d3dx11effect.h"
//DirectXTK
#include <DirectXMath.h>
#include <Audio.h>
#include <BufferHelpers.h>
#include <CommonStates.h>
#include <DDSTextureLoader.h>
#include <DirectXHelpers.h>
#include <Effects.h>
#include <GamePad.h>
#include <GeometricPrimitive.h>
#include <GraphicsMemory.h>
#include <Keyboard.h>
#include <Model.h>
#include <Mouse.h>
#include <PostProcess.h>
#include <PrimitiveBatch.h>
#include <ScreenGrab.h>
#include <SimpleMath.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
//Windows
#define NOMINMAX
#include <Windows.h>
#include <windowsx.h>
#include <Windows.Foundation.h>
#include <wrl\wrappers\corewrappers.h>
#include <wrl\client.h>

//---------------- USINGS ----------------//

template <typename T>
using comptr = Microsoft::WRL::ComPtr<T>;

//---------------- INCLUDES ----------------//

#include "../xna.hpp"

//---------------- CLASSES ----------------//

namespace xna {	
	//---------------- HELPERS ----------------//

	struct DxHelpers {
		static constexpr DirectX::XMVECTOR VectorToDx(Vector2 const& value) {
			DirectX::XMVECTOR v;

			v.m128_f32[0] = value.X;
			v.m128_f32[1] = value.Y;

			return v;
		}


		static constexpr DirectX::XMVECTOR VectorToDx(Vector3 const& value) {
			DirectX::XMVECTOR v;

			v.m128_f32[0] = value.X;
			v.m128_f32[1] = value.Y;
			v.m128_f32[2] = value.Z;

			return v;
		}

		static constexpr DirectX::XMFLOAT3 Vector3ToDx(Vector3 const& value) {
			DirectX::XMFLOAT3 v;

			v.x = value.X;
			v.y = value.Y;
			v.z = value.Z;

			return v;
		}

		static constexpr DirectX::XMVECTOR VectorToDx(Vector4 const& value) {
			DirectX::XMVECTOR v;

			v.m128_f32[0] = value.X;
			v.m128_f32[1] = value.Y;
			v.m128_f32[2] = value.Z;
			v.m128_f32[3] = value.W;

			return v;
		}

		static constexpr DirectX::XMMATRIX MatrixToDx(Matrix const& value) {
			auto m = DirectX::XMMATRIX(
				value.M11,
				value.M12,
				value.M13,
				value.M14,
				value.M21,
				value.M22,
				value.M23,
				value.M24,
				value.M31,
				value.M32,
				value.M33,
				value.M34,
				value.M41,
				value.M42,
				value.M43,
				value.M44
			);

			return m;
		}


		static constexpr DirectX::SpriteSortMode SpriteSortToDx(SpriteSortMode value) {
			return static_cast<DirectX::SpriteSortMode>(static_cast<int>(value));
		}

		static constexpr DXGI_FORMAT SurfaceFormatToDx(SurfaceFormat format)
		{
			switch (format)
			{
			case SurfaceFormat::Color://21                
				return DXGI_FORMAT_R8G8B8A8_UNORM;
			case SurfaceFormat::Bgr565: //23                
				return DXGI_FORMAT_B5G6R5_UNORM;
			case SurfaceFormat::Bgra5551://25                
				return DXGI_FORMAT_B5G5R5A1_UNORM;
			case SurfaceFormat::Bgra4444://26                
				return DXGI_FORMAT_B4G4R4A4_UNORM;
			case SurfaceFormat::Dxt1://827611204                
				return DXGI_FORMAT_BC1_UNORM;
			case SurfaceFormat::Dxt3://861165636                
				return DXGI_FORMAT_BC2_UNORM;
			case SurfaceFormat::Dxt5://894720068                
				return DXGI_FORMAT_BC3_UNORM;
			case SurfaceFormat::NormalizedByte2://60                
				return DXGI_FORMAT_R8G8_SNORM;
			case SurfaceFormat::NormalizedByte4://63                
				return DXGI_FORMAT_R8G8B8A8_SNORM;
			case SurfaceFormat::Rgba1010102://31                
				return DXGI_FORMAT_R10G10B10A2_UNORM;
			case SurfaceFormat::Rg32://34                
				return DXGI_FORMAT_R16G16_UNORM;
			case SurfaceFormat::Rgba64://36                
				return DXGI_FORMAT_R16G16B16A16_UNORM;
			case SurfaceFormat::Alpha8://28
				return DXGI_FORMAT_A8_UNORM;
			case SurfaceFormat::Single://114                
				return DXGI_FORMAT_R32_FLOAT;
			case SurfaceFormat::Vector2://115                
				return DXGI_FORMAT_R32G32_FLOAT;
			case SurfaceFormat::Vector4://116                
				return DXGI_FORMAT_R32G32B32A32_FLOAT;
			case SurfaceFormat::HalfSingle://111                
				return DXGI_FORMAT_R16_FLOAT;
			case SurfaceFormat::HalfVector2://112                
				return DXGI_FORMAT_R16G16_FLOAT;
			case SurfaceFormat::HalfVector4://113                
				return DXGI_FORMAT_R16G16B16A16_FLOAT;
			case SurfaceFormat::HdrBlendable://113                
				return DXGI_FORMAT_R16G16B16A16_FLOAT;
			default://0                
				return DXGI_FORMAT_UNKNOWN;
			}
		}

		static constexpr SurfaceFormat SurfaceFormatToXna(DXGI_FORMAT format) {
			switch (format)
			{
			case DXGI_FORMAT_R8G8B8A8_UNORM:
			case DXGI_FORMAT_B8G8R8A8_UNORM:
				return SurfaceFormat::Color;
			case DXGI_FORMAT_B5G6R5_UNORM:
				return SurfaceFormat::Bgr565;
			case DXGI_FORMAT_B5G5R5A1_UNORM:
				return SurfaceFormat::Bgra5551;
			case DXGI_FORMAT_B4G4R4A4_UNORM:
				return SurfaceFormat::Bgra4444;
			case DXGI_FORMAT_BC2_UNORM:
				return SurfaceFormat::Dxt3;
			case DXGI_FORMAT_BC3_UNORM:
				return SurfaceFormat::Dxt5;
			case DXGI_FORMAT_R8G8_SNORM:
				return  SurfaceFormat::NormalizedByte2;
			case DXGI_FORMAT_R8G8B8A8_SNORM:
				return SurfaceFormat::NormalizedByte4;
			case DXGI_FORMAT_R10G10B10A2_UNORM:
				return SurfaceFormat::Rgba1010102;
			case DXGI_FORMAT_R16G16_UNORM:
				return SurfaceFormat::Rg32;
			case DXGI_FORMAT_R16G16B16A16_UNORM:
				return SurfaceFormat::Rgba64;
			case DXGI_FORMAT_A8_UNORM:
				return SurfaceFormat::Alpha8;
			case DXGI_FORMAT_R32_FLOAT:
				return SurfaceFormat::Single;
			case DXGI_FORMAT_R32G32_FLOAT:
				return SurfaceFormat::Vector2;
			case DXGI_FORMAT_R32G32B32A32_FLOAT:
				return SurfaceFormat::Vector4;
			case DXGI_FORMAT_R16_FLOAT:
				return SurfaceFormat::HalfSingle;
			case DXGI_FORMAT_R16G16_FLOAT:
				return SurfaceFormat::HalfVector2;
			case DXGI_FORMAT_R16G16B16A16_FLOAT:
				return SurfaceFormat::HalfVector4;
			default:
				return SurfaceFormat::Unknown;
			}
		}

		static constexpr Blend BlendToXna(D3D11_BLEND blend) {
			switch (blend) {
			case D3D11_BLEND_ZERO:
				return Blend::Zero;
			case D3D11_BLEND_ONE:
				return Blend::One;
			case D3D11_BLEND_SRC_COLOR:
				return Blend::SourceColor;
			case D3D11_BLEND_INV_SRC_COLOR:
				return Blend::InverseSourceColor;
			case D3D11_BLEND_SRC_ALPHA:
				return Blend::SourceAlpha;
			case D3D11_BLEND_INV_SRC_ALPHA:
				return Blend::InverseSourceAlpha;
			case D3D11_BLEND_DEST_ALPHA:
				return Blend::DestinationAlpha;
			case D3D11_BLEND_INV_DEST_ALPHA:
				return Blend::InverseDestinationAlpha;
			case D3D11_BLEND_DEST_COLOR:
				return Blend::DestinationColor;
			case D3D11_BLEND_INV_DEST_COLOR:
				return Blend::InverseDestinationColor;
			case D3D11_BLEND_SRC_ALPHA_SAT:
				return Blend::SourceAlphaSaturation;
			case D3D11_BLEND_BLEND_FACTOR:
				return Blend::BlendFactor;
			case D3D11_BLEND_INV_BLEND_FACTOR:
				return Blend::InverseBlendFactor;
			case D3D11_BLEND_SRC1_COLOR:
				return Blend::Source1Color;
			case D3D11_BLEND_INV_SRC1_COLOR:
				return Blend::InverseSource1Color;
			case D3D11_BLEND_SRC1_ALPHA:
				return Blend::Source1Alpha;
			case D3D11_BLEND_INV_SRC1_ALPHA:
				return Blend::InverseSource1Alpha;
			default:
				return Blend::Zero;
			}
		}

		static constexpr D3D11_BLEND BlendToDx(Blend blend) {
			switch (blend)
			{
			case xna::Blend::Zero:
				return D3D11_BLEND_ZERO;
			case xna::Blend::One:
				return D3D11_BLEND_ONE;
			case xna::Blend::SourceColor:
				return D3D11_BLEND_SRC_COLOR;
			case xna::Blend::InverseSourceColor:
				return D3D11_BLEND_INV_SRC_COLOR;
			case xna::Blend::SourceAlpha:
				return D3D11_BLEND_SRC_ALPHA;
			case xna::Blend::InverseSourceAlpha:
				return D3D11_BLEND_INV_SRC_ALPHA;
			case xna::Blend::DestinationAlpha:
				return D3D11_BLEND_DEST_ALPHA;
			case xna::Blend::InverseDestinationAlpha:
				return D3D11_BLEND_INV_DEST_ALPHA;
			case xna::Blend::DestinationColor:
				return D3D11_BLEND_DEST_COLOR;
			case xna::Blend::InverseDestinationColor:
				return D3D11_BLEND_INV_DEST_COLOR;
			case xna::Blend::SourceAlphaSaturation:
				return D3D11_BLEND_SRC_ALPHA_SAT;
			case xna::Blend::BlendFactor:
				return D3D11_BLEND_BLEND_FACTOR;
			case xna::Blend::InverseBlendFactor:
				return D3D11_BLEND_INV_BLEND_FACTOR;
			case xna::Blend::Source1Color:
				return D3D11_BLEND_SRC1_COLOR;
			case xna::Blend::InverseSource1Color:
				return D3D11_BLEND_INV_SRC1_COLOR;
			case xna::Blend::Source1Alpha:
				return D3D11_BLEND_SRC1_ALPHA;
			case xna::Blend::InverseSource1Alpha:
				return D3D11_BLEND_INV_SRC1_ALPHA;
			default:
				return D3D11_BLEND_ZERO;
			}
		}

		static constexpr D3D11_BLEND_OP BlendOperationToDx(BlendOperation op) {
			return static_cast<D3D11_BLEND_OP>(static_cast<int>(op) + 1);
		}

		static constexpr BlendOperation BlendOperationToXna(D3D11_BLEND_OP op) {
			return static_cast<BlendOperation>(static_cast<int>(op) - 1);
		}

		static constexpr D3D11_COLOR_WRITE_ENABLE ColorWriteChannelsToDx(ColorWriteChannels colorWrite) {
			switch (colorWrite)
			{
			case xna::ColorWriteChannels::Red:
				return D3D11_COLOR_WRITE_ENABLE_RED;
			case xna::ColorWriteChannels::Green:
				return D3D11_COLOR_WRITE_ENABLE_GREEN;
			case xna::ColorWriteChannels::Blue:
				return D3D11_COLOR_WRITE_ENABLE_BLUE;
			case xna::ColorWriteChannels::Alpha:
				return D3D11_COLOR_WRITE_ENABLE_ALPHA;
			case xna::ColorWriteChannels::All:
				return D3D11_COLOR_WRITE_ENABLE_ALL;			
			default:
				return D3D11_COLOR_WRITE_ENABLE_ALL;
			}
		}

		static constexpr D3D11_TEXTURE_ADDRESS_MODE TextureAddresModeToDx(TextureAddressMode value) {
			return static_cast<D3D11_TEXTURE_ADDRESS_MODE>(static_cast<int>(value) + 1);
		}

		static constexpr TextureAddressMode TextureAddresModeToXna(D3D11_TEXTURE_ADDRESS_MODE value) {
			return static_cast<TextureAddressMode>(value - 1);
		}
	};	

	struct PlatformInit {
		static void Init() {
			InitRegisteredTypes();
			InitActivadors();
		}

		static void InitRegisteredTypes();
		static void InitActivadors();

	private:
		template <typename T>
		static void insertRegisteredReader(String const& readerName) {
			const auto reader = typeof<T>();
			//Type::NameOfRegisteredTypes.insert({ "xna::" + readerName, reader });
			Type::NameOfRegisteredTypes.insert({ reader->FullName(), reader });
			Type::NameOfRegisteredTypes.insert({ "Microsoft.Xna.Framework.Content." + readerName, reader });
		}

		template <typename T>
		static void insertRegisteredReader(String const& readerName, String const& microsoftNameFullName) {
			const auto reader = typeof<T>();
			//Type::NameOfRegisteredTypes.insert({ "xna::" + readerName, reader });
			Type::NameOfRegisteredTypes.insert({ reader->FullName(), reader });
			Type::NameOfRegisteredTypes.insert({ microsoftNameFullName, reader });
		}

		template <typename T>
		static void insertActivadorReader() {
			ContentTypeReaderActivador::SetActivador(typeof<T>(), []() -> sptr<ContentTypeReader> {
				auto obj = snew<T>();
				return reinterpret_pointer_cast<ContentTypeReader>(obj);
				});
		}
	};
	
	// Helper class for animation and simulation timing.
	class StepTimer
	{
	public:
		StepTimer() noexcept(false) :
			m_elapsedTicks(0),
			m_totalTicks(0),
			m_leftOverTicks(0),
			m_frameCount(0),
			m_framesPerSecond(0),
			m_framesThisSecond(0),
			m_qpcSecondCounter(0),
			m_isFixedTimeStep(false),
			m_targetElapsedTicks(TicksPerSecond / 60)
		{
			if (!QueryPerformanceFrequency(&m_qpcFrequency))
			{
				throw std::exception();
			}

			if (!QueryPerformanceCounter(&m_qpcLastTime))
			{
				throw std::exception();
			}

			// Initialize max delta to 1/10 of a second.
			m_qpcMaxDelta = static_cast<uint64_t>(m_qpcFrequency.QuadPart / 10);
		}

		// Get elapsed time since the previous Update call.
		uint64_t GetElapsedTicks() const noexcept { return m_elapsedTicks; }
		double GetElapsedSeconds() const noexcept { return TicksToSeconds(m_elapsedTicks); }

		// Get total time since the start of the program.
		uint64_t GetTotalTicks() const noexcept { return m_totalTicks; }
		double GetTotalSeconds() const noexcept { return TicksToSeconds(m_totalTicks); }

		// Get total number of updates since start of the program.
		uint32_t GetFrameCount() const noexcept { return m_frameCount; }

		// Get the current framerate.
		uint32_t GetFramesPerSecond() const noexcept { return m_framesPerSecond; }

		// Set whether to use fixed or variable timestep mode.
		void SetFixedTimeStep(bool isFixedTimestep) noexcept { m_isFixedTimeStep = isFixedTimestep; }

		// Set how often to call Update when in fixed timestep mode.
		void SetTargetElapsedTicks(uint64_t targetElapsed) noexcept { m_targetElapsedTicks = targetElapsed; }
		void SetTargetElapsedSeconds(double targetElapsed) noexcept { m_targetElapsedTicks = SecondsToTicks(targetElapsed); }

		// Integer format represents time using 10,000,000 ticks per second.
		static constexpr uint64_t TicksPerSecond = 10000000;

		static constexpr double TicksToSeconds(uint64_t ticks) noexcept { return static_cast<double>(ticks) / TicksPerSecond; }
		static constexpr uint64_t SecondsToTicks(double seconds) noexcept { return static_cast<uint64_t>(seconds * TicksPerSecond); }

		// After an intentional timing discontinuity (for instance a blocking IO operation)
		// call this to avoid having the fixed timestep logic attempt a set of catch-up
		// Update calls.

		void ResetElapsedTime()
		{
			if (!QueryPerformanceCounter(&m_qpcLastTime))
			{
				throw std::exception();
			}

			m_leftOverTicks = 0;
			m_framesPerSecond = 0;
			m_framesThisSecond = 0;
			m_qpcSecondCounter = 0;
		}

		// Update timer state, calling the specified Update function the appropriate number of times.
		template<typename TUpdate>
		void Tick(const TUpdate& update)
		{
			// Query the current time.
			LARGE_INTEGER currentTime;

			if (!QueryPerformanceCounter(&currentTime))
			{
				throw std::exception();
			}

			uint64_t timeDelta = static_cast<uint64_t>(currentTime.QuadPart - m_qpcLastTime.QuadPart);

			m_qpcLastTime = currentTime;
			m_qpcSecondCounter += timeDelta;

			// Clamp excessively large time deltas (e.g. after paused in the debugger).
			if (timeDelta > m_qpcMaxDelta)
			{
				timeDelta = m_qpcMaxDelta;
			}

			// Convert QPC units into a canonical tick format. This cannot overflow due to the previous clamp.
			timeDelta *= TicksPerSecond;
			timeDelta /= static_cast<uint64_t>(m_qpcFrequency.QuadPart);

			const uint32_t lastFrameCount = m_frameCount;

			if (m_isFixedTimeStep)
			{
				// Fixed timestep update logic

				// If the app is running very close to the target elapsed time (within 1/4 of a millisecond) just clamp
				// the clock to exactly match the target value. This prevents tiny and irrelevant errors
				// from accumulating over time. Without this clamping, a game that requested a 60 fps
				// fixed update, running with vsync enabled on a 59.94 NTSC display, would eventually
				// accumulate enough tiny errors that it would drop a frame. It is better to just round
				// small deviations down to zero to leave things running smoothly.

				if (static_cast<uint64_t>(std::abs(static_cast<int64_t>(timeDelta - m_targetElapsedTicks))) < TicksPerSecond / 4000)
				{
					timeDelta = m_targetElapsedTicks;
				}

				m_leftOverTicks += timeDelta;

				while (m_leftOverTicks >= m_targetElapsedTicks)
				{
					m_elapsedTicks = m_targetElapsedTicks;
					m_totalTicks += m_targetElapsedTicks;
					m_leftOverTicks -= m_targetElapsedTicks;
					m_frameCount++;

					update();
				}
			}
			else
			{
				// Variable timestep update logic.
				m_elapsedTicks = timeDelta;
				m_totalTicks += timeDelta;
				m_leftOverTicks = 0;
				m_frameCount++;

				update();
			}

			// Track the current framerate.
			if (m_frameCount != lastFrameCount)
			{
				m_framesThisSecond++;
			}

			if (m_qpcSecondCounter >= static_cast<uint64_t>(m_qpcFrequency.QuadPart))
			{
				m_framesPerSecond = m_framesThisSecond;
				m_framesThisSecond = 0;
				m_qpcSecondCounter %= static_cast<uint64_t>(m_qpcFrequency.QuadPart);
			}
		}

	private:
		// Source timing data uses QPC units.
		LARGE_INTEGER m_qpcFrequency;
		LARGE_INTEGER m_qpcLastTime;
		uint64_t m_qpcMaxDelta;

		// Derived timing data uses a canonical tick format.
		uint64_t m_elapsedTicks;
		uint64_t m_totalTicks;
		uint64_t m_leftOverTicks;

		// Members for tracking the framerate.
		uint32_t m_frameCount;
		uint32_t m_framesPerSecond;
		uint32_t m_framesThisSecond;
		uint64_t m_qpcSecondCounter;

		// Members for configuring fixed timestep mode.
		bool m_isFixedTimeStep;
		uint64_t m_targetElapsedTicks;
	};

	//---------------- IMPL ----------------//

	struct SpriteFont::PlatformImplementation {
		uptr<DirectX::SpriteFont> _dxSpriteFont{ nullptr };
	};

	struct SpriteBatch::PlatformImplementation {
		sptr<DirectX::SpriteBatch> _dxspriteBatch = nullptr;
		comptr<ID3D11InputLayout> dxInputLayout = nullptr;
	};

	struct GraphicsAdapter::PlatformImplementation {
		comptr<IDXGIAdapter1> dxadapter = nullptr;

	private:
		friend class GraphicsAdapter;
		Uint _index{ 0 };
		sptr<DisplayMode> _currentDisplayMode = nullptr;

	public:
		bool GetOutput(UINT slot, IDXGIOutput*& output);
	};

	struct BlendRenderTarget {
		bool Enabled{ true };
		Blend Source{ Blend::SourceAlpha };
		Blend Destination{ Blend::InverseSourceAlpha };
		BlendOperation Operation{ BlendOperation::Add };
		Blend SourceAlpha{ Blend::One };
		Blend DestinationAlpha{ Blend::Zero };
		BlendOperation OperationAlpha{ BlendOperation::Add };
		ColorWriteChannels WriteMask{ ColorWriteChannels::All };

		constexpr BlendRenderTarget() = default;
	};

	struct BlendState::PlatformImplementation {
		comptr<ID3D11BlendState> dxBlendState = nullptr;
		D3D11_BLEND_DESC dxDescription{};
		float blendFactor[4]{ 1.0F, 1.0F, 1.0F, 1.0F };
		UINT sampleMask{ 0xffffffff };
	};	

	struct DepthStencilState::PlatformImplementation {
		comptr<ID3D11DepthStencilState> dxDepthStencil = nullptr;
		D3D11_DEPTH_STENCIL_DESC dxDescription{};
	};

	struct DisplayModeRefreshRate {
		constexpr DisplayModeRefreshRate() = default;

		constexpr DisplayModeRefreshRate(DXGI_RATIONAL const& dxrational) {
			Numerator = dxrational.Numerator;
			Denominator = dxrational.Denominator;
		}
		constexpr DisplayModeRefreshRate(Uint numerator, Uint denominator)
			: Numerator(numerator), Denominator(denominator) {}

		Uint Numerator{ 0 };
		Uint Denominator{ 0 };

		constexpr bool operator==(const DisplayModeRefreshRate& other) const
		{
			return Numerator == other.Numerator && Denominator == other.Denominator;
		}
	};

	struct DisplayModeDescription {
		DisplayModeScanlineOrder _scanlineOrdering{ DisplayModeScanlineOrder::Unspecified };
		DisplayModeScaling _scaling{ DisplayModeScaling::Unspecified };
		DisplayModeRefreshRate _refreshRate{};

		constexpr bool operator==(const DisplayModeDescription& other) const
		{
			return _scanlineOrdering == other._scanlineOrdering && _scaling == other._scaling && _refreshRate == other._refreshRate;
		}
	};

	struct DisplayMode::PlatformImplementation {
		std::vector<DisplayModeDescription> Descriptions;
	};

	struct GamePad::PlatformImplementation {
		uptr<DirectX::GamePad> _dxGamePad = unew<DirectX::GamePad>();

		void Suspend() const {
			if (_dxGamePad)
				_dxGamePad->Suspend();
		}

		void Resume() const {
			if (_dxGamePad)
				_dxGamePad->Resume();
		}
	};

	struct Keyboard::PlatformImplementation {
		uptr<DirectX::Keyboard> _dxKeyboard = unew<DirectX::Keyboard>();

		void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam) const {
			if (_dxKeyboard)
				_dxKeyboard->ProcessMessage(message, wParam, lParam);
		}
	};

	struct Mouse::PlatformImplementation {
		uptr<DirectX::Mouse> _dxMouse = unew<DirectX::Mouse>();

		void ProcessMessage(UINT message, WPARAM wParam, LPARAM lParam) const {
			if (_dxMouse)
				_dxMouse->ProcessMessage(message, wParam, lParam);
		}
	};

	struct RasterizerState::PlatformImplementation {
		comptr<ID3D11RasterizerState> dxRasterizerState = nullptr;
		D3D11_RASTERIZER_DESC dxDescription{};
	};

	struct SamplerState::PlatformImplementation {
		comptr<ID3D11SamplerState> _samplerState = nullptr;
		D3D11_SAMPLER_DESC _description{};
	};	

	struct SwapChain::PlatformImplementation {
		comptr<IDXGISwapChain1> dxSwapChain{ nullptr };
		DXGI_SWAP_CHAIN_DESC1 dxDescription{};
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC dxFullScreenDescription{};

		bool GetBackBuffer(comptr<ID3D11Texture2D>& texture2D) {
			if (!dxSwapChain)
				return false;

			const auto hr = dxSwapChain->GetBuffer(0, __uuidof(texture2D), (void**)texture2D.GetAddressOf());

			return !FAILED(hr);
		}
	};

	struct Texture2D::PlatformImplementation {
		comptr<ID3D11Texture2D> dxTexture2D{ nullptr };
		comptr<ID3D11ShaderResourceView> dxShaderResource{ nullptr };
		D3D11_SUBRESOURCE_DATA dxSubResource{};
		D3D11_TEXTURE2D_DESC dxDescription{};
		D3D11_SHADER_RESOURCE_VIEW_DESC dxShaderDescription{};
	};

	struct RenderTarget2D::PlatformImplementation {		
		comptr<ID3D11RenderTargetView> _renderTargetView = nullptr;
		D3D11_RENDER_TARGET_VIEW_DESC _renderTargetDesc{};
	};	

	enum class GameWindowMode : UINT {
		Fullscreen = WS_POPUP | WS_VISIBLE,
		Windowed = WS_OVERLAPPED | WS_SYSMENU | WS_VISIBLE,
		Borderless = WS_EX_TOPMOST | WS_POPUP | WS_VISIBLE,
	};

	struct GameWindow::PlatformImplementation {
	public:
		constexpr void Mode(GameWindowMode mode) {
			_windowStyle = static_cast<int>(mode);
		}

		constexpr GameWindowMode Mode() const {
			return static_cast<GameWindowMode>(_windowStyle);
		}

		void Position(int width, int height, bool update = true);
		void Size(int width, int height, bool update = true);

		inline HINSTANCE HInstance() const {
			return _hInstance;
		}

		inline HWND WindowHandle() const {
			return _windowHandle;
		}

		constexpr int Width() const {
			return _windowWidth;
		}

		constexpr int Height() const {
			return _windowHeight;
		}

		inline void Icon(unsigned int icon) {
			_windowIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(icon));
		}

		inline void Icon(HICON icon) {
			_windowIcon = icon;
		}

		inline void Cursor(unsigned int cursor) {
			_windowCursor = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(cursor));
		}

		inline void Cursor(HCURSOR cursor) {
			_windowCursor = cursor;
		}

		constexpr float CenterX() const {
			return _windowCenterX;
		}

		constexpr float CenterY() const {
			return _windowCenterY;
		}

		inline void CursorVisibility(bool visible) const {
			ShowCursor(visible);
		}

		inline void Close() {
			PostMessage(_windowHandle, WM_DESTROY, 0, 0);
		}

		constexpr COLORREF Color()	const {
			return _windowColor;
		}

		constexpr void Color(COLORREF color) {
			_windowColor = color;
		}

		constexpr void Color(BYTE r, BYTE g, BYTE b) {
			_windowColor = RGB(r, g, b);
		}

		bool Create();
		bool Update();

		static LRESULT CALLBACK WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	private:
		friend class GameWindow;

		HINSTANCE		_hInstance{ nullptr };
		HWND			_windowHandle{ nullptr };
		int				_windowWidth{ 800 };
		int				_windowHeight{ 480 };
		HICON			_windowIcon{ nullptr };
		HCURSOR			_windowCursor{ nullptr };
		COLORREF		_windowColor{ RGB(0,0,0) };
		String			_windowTitle{ "Xna++ Game Development" };
		DWORD			_windowStyle{ 0 };
		int				_windowPosX{ 0 };
		int				_windowPosY{ 0 };
		float			_windowCenterX{ 0 };
		float			_windowCenterY{ 0 };

		inline void setPosition() {
			_windowPosX = GetSystemMetrics(SM_CXSCREEN) / 2 - _windowWidth / 2;
			_windowPosY = GetSystemMetrics(SM_CYSCREEN) / 2 - _windowHeight / 2;
		}

		inline void setCenter() {
			_windowCenterX = _windowWidth / 2.0f;
			_windowCenterY = _windowHeight / 2.0f;
		}
	};

	struct AudioEngine::PlatformImplementation {
		PlatformImplementation() {
			_dxAudioEngine = unew<DirectX::AudioEngine>(
#ifdef _DEBUG
				DirectX::AudioEngine_Debug
#endif
			);
		}

		~PlatformImplementation() {
			if (_dxAudioEngine) {
				_dxAudioEngine->Suspend();
			}
		}

		uptr<DirectX::AudioEngine> _dxAudioEngine = nullptr;
	};

	struct GraphicsDevice::PlatformImplementation {
		PlatformImplementation() {
			_blendState = xna::BlendState::Opaque();
			_depthStencilState = xna::DepthStencilState::Default();			
			_rasterizerState = xna::RasterizerState::CullCounterClockwise();
			_samplerStates = snew<SamplerStateCollection>();
		}				

	public:
		comptr<ID3D11Device> _device = nullptr;
		comptr<ID3D11DeviceContext> _context = nullptr;
		comptr<IDXGIFactory1> _factory = nullptr;
		
		PBlendState _blendState = nullptr;
		PRasterizerState _rasterizerState = nullptr;
		PDepthStencilState _depthStencilState = nullptr;
		PSamplerStateCollection _samplerStates = nullptr;
		Int _multiSampleMask = 0xffffffff;
		
		sptr<SwapChain> _swapChain = nullptr;
		sptr<GraphicsAdapter> _adapter = nullptr;
		sptr<RenderTarget2D> _renderTarget2D = nullptr;
		sptr<GameWindow> _gameWindow = nullptr;
		xna::Viewport _viewport{};
		sptr<xna::PresentationParameters> _presentationParameters;
		D3D_FEATURE_LEVEL _featureLevel{ D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0 };

	private:
		friend class GraphicsDevice;
		float _backgroundColor[4] = { 0, 0, 0, 0 };
		bool _usevsync{ true };
	};

	struct Game::PlatformImplementation {
	private:
		friend class Game;

		xna::StepTimer _stepTimer{};
	};

	struct SoundEffectInstance::PlatformImplementation {
		uptr<DirectX::SoundEffectInstance> _dxInstance = nullptr;
	};

	struct SoundEffect::PlatformImplementation {
		uptr<DirectX::SoundEffect> _dxSoundEffect = nullptr;
	};

	struct Effect::PlatformImplementation {
		sptr<DirectX::IEffect> dxEffect = nullptr;
	};

	struct BasicEffect::PlatformImplementation {
		sptr<DirectX::BasicEffect> dxBasicEffect = nullptr;
	};
}
#endif