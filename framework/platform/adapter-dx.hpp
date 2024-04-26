#ifndef XNA_PLATFORM_ADAPTER_DX_HPP
#define XNA_PLATFORM_ADAPTER_DX_HPP

#include "../graphics/adapter.hpp"
#include "displaymode-dx.hpp"
#include "dxheaders.hpp"


namespace xna {
	class GraphicsAdapter : public IGraphicsAdapter {
	public:
		friend class IGraphicsAdapter;

		GraphicsAdapter() {}

		virtual ~GraphicsAdapter() override {
			if (dxadapter) {
				dxadapter->Release();
				dxadapter = nullptr;
			}
		}

		virtual String Description() const override;
		virtual Uint DeviceId() const override;
		virtual String DeviceName() const override;
		virtual intptr_t MonitorHandle() const override;
		virtual Uint Revision() const override;
		virtual Uint SubSystemId() const override;
		virtual Uint VendorId() const override;
		virtual UDisplayModeCollection SupportedDisplayModes() const override;
		virtual UDisplayModeCollection SupportedDisplayModes(SurfaceFormat surfaceFormat) const override;
		virtual PDisplayMode CurrentDisplayMode() override;
		virtual void CurrentDisplayMode(SurfaceFormat surfaceFormat, Uint width, Uint height) override;
		virtual constexpr bool IsDefaultAdapter() const { return _index == 0; }	
		bool GetOutput(UINT slot, IDXGIOutput*& output);

	public:
		IDXGIAdapter1* dxadapter{ nullptr };	
	private:
		Uint _index{ 0 };
		PDisplayMode _currentDisplayMode = nullptr;

	public:
		static constexpr DXGI_FORMAT ConvertSurfaceToDXGIFORMAT(SurfaceFormat format)
		{
			switch (format)
			{
			case SurfaceFormat::Color://21                
				return DXGI_FORMAT_B8G8R8A8_UNORM;
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

		static constexpr SurfaceFormat ConvertDXGIFORMATToSurface(DXGI_FORMAT format) {
			switch (format)
			{
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
	};			
}

#endif
