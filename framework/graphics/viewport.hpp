#ifndef XNA_GRAPHICS_VIEWPORT
#define XNA_GRAPHICS_VIEWPORT

namespace xna {
	struct Viewport {
		float X{ 0 };
		float Y{ 0 };
		float Width{ 0 };
		float Height{ 0 };
		float MinDetph{ 0 };
		float MaxDepth{ 1.0F };

		constexpr Viewport() = default;

		constexpr Viewport(float X, float Y, float Width, float Height, float MinDetph, float MaxDepth)
			: X(X), Y(Y), Width(Width), Height(Height), MinDetph(MinDetph), MaxDepth(MaxDepth) {}

		constexpr bool operator==(const Viewport& other) const	{
			return X == other.X 
				&& Y == other.Y 
				&& Width == other.Width 
				&& Height == other.Height 
				&& MinDetph == other.MinDetph 
				&& MaxDepth == other.MaxDepth;
		}
	};
}

#endif