#include "Xna/Framework/Input/Keyboard.hpp"
#include "CSharp/Exception.hpp"
#include "InputImplementations.hpp"

using std::unique_ptr;
using std::make_unique;
using CSharp::InvalidOperationException;

namespace Xna {
	const std::string SR_NULL_IMPLEMENTATION = "The internal keyboard handler is null.";

	

	unique_ptr<Keyboard::Implementation> Keyboard::implementation = make_unique<Keyboard::Implementation>();

	KeyboardState Keyboard::GetState() {
		if (!implementation)
			throw InvalidOperationException(SR_NULL_IMPLEMENTATION);

		const auto implState = Keyboard::implementation->keyboard.GetState();
		auto ptr = reinterpret_cast<const uint32_t*>(&implState);
		const auto state = reinterpret_cast<const KeyboardState*>(ptr);

		return *state;
	}
}