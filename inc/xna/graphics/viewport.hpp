#ifndef XNA_GRAPHICS_VIEWPORT
#define XNA_GRAPHICS_VIEWPORT

#include "../common/numerics.hpp"

namespace xna {
	//Defines the window dimensions of a render-target surface onto which a 3D volume projects. 
	struct Viewport {
		//Gets or sets the pixel coordinate of the upper-left corner of the viewport on the render-target surface.
		float X{ 0 };
		//Gets or sets the pixel coordinate of the upper-left corner of the viewport on the render-target surface.
		float Y{ 0 };
		//Gets or sets the width dimension of the viewport on the render-target surface, in pixels.
		float Width{ 0 };
		//Gets or sets the height dimension of the viewport on the render-target surface, in pixels.
		float Height{ 0 };
		//Gets or sets the minimum depth of the clip volume.
		float MinDetph{ 0 };
		//Gets or sets the maximum depth of the clip volume.
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

		//Gets the size of this resource.
		constexpr Rectangle Bounds() const {
			return { 
				static_cast<int>(X),
				static_cast<int>(Y),
				static_cast<int>(Width),
				static_cast<int>(Height),
			};
		}
	};
}

#endif