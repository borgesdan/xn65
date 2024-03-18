#ifndef XNA_PLATFORM_DEVICE_DX_HPP
#define XNA_PLATFORM_DEVICE_DX_HPP

#include "../graphics/device.hpp"
#include "dxgi.h"
#include "d3d11.h"

namespace xna {
	class GraphicsDevice::InternalProperty {
		friend class GraphicsDevice;

	public:
		InternalProperty() {}

		~InternalProperty() {
			if (_device) {
				_device->Release();
				_device = nullptr;
			}

			if (_context) {
				_context->Release();
				_context = nullptr;
			}
		}

		constexpr void SetCreateFlags(D3D11_CREATE_DEVICE_FLAG flags) {
			_createDeviceFlags |= flags;
		}

		constexpr void ClearCreateFlags() {
			_createDeviceFlags = 0;
		}

	public:
		ID3D11Device* _device{ nullptr };
		ID3D11DeviceContext* _context{ nullptr };

	private:
		unsigned int _createDeviceFlags{ 0 };
		D3D_FEATURE_LEVEL _featureLevel{ D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0 };
		float _backgroundColor[4] = { 0, 0, 0, 0 };
	};
}

#endif