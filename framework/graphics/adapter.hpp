#ifndef XNA_GRAPHICS_ADAPTER_HPP
#define XNA_GRAPHICS_ADAPTER_HPP

#include "../enums.hpp"
#include "../types.hpp"
#include "../forward.hpp"
#include "displaymode.hpp"

namespace xna {
	class IGraphicsAdapter {
	public:
		virtual ~IGraphicsAdapter() {
		}

		static PGraphicsAdapter DefaultAdapter() {
			if (_adaptersList.empty())
				return nullptr;

			if (_defaultAdapterIndex >= _adaptersList.size())
				return nullptr;

			return _adaptersList[_defaultAdapterIndex];
		}

		static constexpr void DefaultAdapter(size_t index) {
			_defaultAdapterIndex = index;
		}

		static constexpr std::vector<PGraphicsAdapter> Adapters() {
			return _adaptersList;
		}

		virtual String Description() const = 0;
		virtual Uint DeviceId() const = 0;
		virtual String DeviceName() const = 0;

		constexpr bool IsDefaultAdapter() const {
			return _index == _defaultAdapterIndex;
		}

		virtual intptr_t MonitorHandle() const = 0;
		virtual Uint Revision() const = 0;
		virtual Uint SubSystemId() const = 0;
		virtual Uint VendorId() const = 0;

		virtual PDisplayModeCollection SupportedDisplayModes() const = 0;
	
	protected:
		Uint _index{ 0 };
		inline static size_t _defaultAdapterIndex = 0;
		static std::vector<PGraphicsAdapter> getAllAdapters();
		inline static std::vector<PGraphicsAdapter> _adaptersList = getAllAdapters();
	};
}

#endif