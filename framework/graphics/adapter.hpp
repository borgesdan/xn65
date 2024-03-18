#ifndef XNA_GRAPHICS_ADAPTER_HPP
#define XNA_GRAPHICS_ADAPTER_HPP

#include "../enums.hpp"
#include "../types.hpp"
#include "../forward.hpp"
#include "displaymode.hpp"

namespace xna {
	class GraphicsAdapter {
	public:
		PLATFORM_DEVELOPMENT
			GraphicsAdapter();

		static PGraphicsAdapter DefaultAdapter() {
			if (_adaptersList.empty())
				return nullptr;

			if (_defaultAdapter >= _adaptersList.size())
				return nullptr;

			return _adaptersList[_defaultAdapter];
		}

		static constexpr void DefaultAdapter(size_t index) {
			_defaultAdapter = index;
		}

		static constexpr std::vector<PGraphicsAdapter> Adapters() {
			return _adaptersList;
		}

		PLATFORM_DEVELOPMENT
			String Description() const;
		PLATFORM_DEVELOPMENT
			Uint DeviceId() const;
		PLATFORM_DEVELOPMENT
			String DeviceName() const;

		bool IsDefaultAdapter() const {
			auto def = DefaultAdapter();
			return this == def.get();
		}

		PLATFORM_DEVELOPMENT
			intptr_t MonitorHandle() const;
		PLATFORM_DEVELOPMENT
			Uint Revision() const;
		PLATFORM_DEVELOPMENT
			Uint SubSystemId() const;
		PLATFORM_DEVELOPMENT
			Uint VendorId() const;
		PLATFORM_DEVELOPMENT
			PDisplayModeCollection SupportedDisplayModes() const;

	private:
		inline static size_t _defaultAdapter = 0;

		PLATFORM_DEVELOPMENT
			static std::vector<PGraphicsAdapter> getAllAdapters();

		inline static std::vector<PGraphicsAdapter> _adaptersList = getAllAdapters();

	public:
		class InternalProperty;
		friend class InternalProperty;
		sptr<InternalProperty> ip_GraphicsAdapter{ nullptr };
	};
}

#endif