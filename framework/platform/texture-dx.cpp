#include "texture-dx.hpp"

namespace xna {
	Texture2D::Texture2D() {
		ip_Texture2D = New<InternalProperty>();
	}
}