#include "xna-dx/framework.hpp"

namespace xna {
	KeyboardState Keyboard::GetState() {
		if (!impl || !impl->_dxKeyboard)
			return KeyboardState();

		const auto state = Keyboard::impl->_dxKeyboard->GetState();
		auto ptr = reinterpret_cast<const uint32_t*>(&state);
		const auto xnaState = reinterpret_cast<const KeyboardState*>(ptr);

		return *xnaState;
	}

	void Keyboard::Initialize() {
		impl = unew<PlatformImplementation>();
	}	
}