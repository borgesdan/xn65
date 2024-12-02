#ifndef XNA_GRAPHICS_SHARED_HPP
#define XNA_GRAPHICS_SHARED_HPP

namespace xna {
	//Defines comparison functions that can be chosen for alpha, stencil, or depth-buffer tests. 
	enum class ComparisonFunction {
		//Always fail the test.
		Never,
		//Accept the new pixel if its value is less than the value of the current pixel.
		Less,
		//Accept the new pixel if its value is equal to the value of the current pixel.
		Equal,
		//Accept the new pixel if its value is less than or equal to the value of the current pixel.
		LessEquals,
		//Accept the new pixel if its value is greater than the value of the current pixel. 
		Greater,
		//Accept the new pixel if its value does not equal the value of the current pixel.
		NotEqual,
		//Accept the new pixel if its value is greater than or equal to the value of the current pixel.
		GreaterEqual,
		//Always pass the test.
		Always
	};

	//Defines comparison functions that can be chosen for alpha, stencil, or depth-buffer tests. 
	using CompareFunction = ComparisonFunction;

	//Defines various types of surface formats. 
	enum class SurfaceFormat {
		//(Unsigned format) 32-bit ARGB pixel format with alpha, using 8 bits per channel.
		Color = 0,
		//(Unsigned format) 16-bit BGR pixel format with 5 bits for blue, 6 bits for green, and 5 bits for red.
		Bgr565 = 1,
		//(Unsigned format) 16-bit BGRA pixel format where 5 bits are reserved for each color and 1 bit is reserved for alpha.
		Bgra5551 = 2,
		//(Unsigned format) 16-bit BGRA pixel format with 4 bits for each channel.
		Bgra4444 = 3,
		//DXT1 compression texture format. 
		//The runtime will not allow an application to create a surface using a DXTn format unless the surface dimensions are multiples of 4.
		//This applies to offscreen-plain surfaces, render targets, 2D textures, cube textures, and volume textures.
		Dxt1 = 4,
		//DXT3 compression texture format.
		//The runtime will not allow an application to create a surface using a DXTn format unless the surface dimensions are multiples of 4.
		//This applies to offscreen-plain surfaces, render targets, 2D textures, cube textures, and volume textures.
		Dxt3 = 5,
		//DXT5 compression texture format.
		//The runtime will not allow an application to create a surface using a DXTn format unless the surface dimensions are multiples of 4.
		//This applies to offscreen-plain surfaces, render targets, 2D textures, cube textures, and volume textures.
		Dxt5 = 6,
		//Signed format) 16-bit bump-map format using 8 bits each for u and v data.
		NormalizedByte2 = 7,
		//Signed format) 32-bit bump-map format using 8 bits for each channel.
		NormalizedByte4 = 8,
		//(Unsigned format) 32-bit RGBA pixel format using 10 bits for each color and 2 bits for alpha.
		Rgba1010102 = 9,
		//Unsigned format) 32-bit pixel format using 16 bits each for red and green. 
		Rg32 = 10,
		//(Unsigned format) 64-bit RGBA pixel format using 16 bits for each component.
		Rgba64 = 11,
		//(Unsigned format) 8-bit alpha only.
		Alpha8 = 12,
		//(IEEE format) 32-bit float format using 32 bits for the red channel.
		Single = 13,
		//(IEEE format) 64-bit float format using 32 bits for the red channel and 32 bits for the green channel.
		Vector2 = 14,
		//(IEEE format) 128-bit float format using 32 bits for each channel (alpha, blue, green, red).
		Vector4 = 15,
		//(Floating-point format) 16-bit float format using 16 bits for the red channel.
		HalfSingle = 16,
		//(Floating-point format) 32-bit float format using 16 bits for the red channel and 16 bits for the green channel.
		HalfVector2 = 17,
		//(Floating-point format) 64-bit float format using 16 bits for each channel (alpha, blue, green, red).
		HalfVector4 = 18,
		//(Floating-point format) for high dynamic range data.
		HdrBlendable = 19,
		//Not implemented.
		Unknown,
	};

	//Defines the format of data in a depth-stencil buffer. 
	enum class DepthFormat {
		//Do not create a depth buffer.
		None,
		//A buffer that contains 16-bits of depth data.
		Depth16,
		//A buffer that contains 24-bits of depth data.
		Depth24,
		//A 32 bit buffer that contains 24 bits of depth data and 8 bits of stencil data.
		Depth24Stencil8
	};
}

#endif