#ifndef XNA_GRAPHICS_RASTERIZER_HPP
#define XNA_GRAPHICS_RASTERIZER_HPP

#include "gresource.hpp"
#include "../platform.hpp"

namespace xna {

	//Defines winding orders that may be used to identify back faces for culling. 
	enum class CullMode {
		//Do not cull back faces. 
		None,
		//Cull back faces with clockwise vertices. 
		CullClockwiseFace,
		//Cull back faces with counterclockwise vertices. 
		CullCounterClockwiseFace,
	};

	//Describes options for filling the vertices and lines that define a primitive. 
	enum class FillMode
	{
		//Draw lines connecting the vertices that define a primitive face.
		WireFrame,
		//Draw solid faces for each primitive.
		Solid,
	};

	struct RasterizerStateImplementation;

	//Contains rasterizer state, which determines how to convert vector data (shapes) into raster data (pixels). 
	class RasterizerState : public GraphicsResource, public PlatformImplementation<RasterizerStateImplementation> {
	public:
		RasterizerState();
		RasterizerState(std::shared_ptr<GraphicsDevice> const& device);

		//Specifies the conditions for culling or removing triangles. The default value is CullMode.CounterClockwise. 
		xna::CullMode CullMode() const;
		//Specifies the conditions for culling or removing triangles. The default value is CullMode.CounterClockwise. 
		void CullMode(xna::CullMode value);
		//The fill mode, which defines how a triangle is filled during rendering. The default is FillMode.Solid. 
		xna::FillMode FillMode() const;
		//The fill mode, which defines how a triangle is filled during rendering. The default is FillMode.Solid. 
		void FillMode(xna::FillMode value);

		//Enables or disables multisample antialiasing. The default is true. 
		bool MultiSampleAntiAlias() const;
		//Enables or disables multisample antialiasing. The default is true. 
		void MultiSampleAntiAlias(bool value);

		//Sets or retrieves the depth bias for polygons, which is the amount of bias to apply to the depth of a primitive
		//to alleviate depth testing problems for primitives of similar depth. The default value is 0. 
		float DepthBias() const;
		//Sets or retrieves the depth bias for polygons, which is the amount of bias to apply to the depth of a primitive
		//to alleviate depth testing problems for primitives of similar depth. The default value is 0. 
		void DepthBias(float value);

		//Gets or sets a bias value that takes into account the slope of a polygon. This bias value is applied to coplanar primitives 
		// to reduce aliasing and other rendering artifacts caused by z-fighting. The default is 0. 
		float SlopeScaleDepthBias() const;
		//Gets or sets a bias value that takes into account the slope of a polygon. This bias value is applied to coplanar primitives 
		// to reduce aliasing and other rendering artifacts caused by z-fighting. The default is 0. 
		void SlopeScaleDepthBias(float value);

		//Enables or disables scissor testing. The default is false. 
		bool ScissorTestEnable() const;
		//Enables or disables scissor testing. The default is false. 
		void ScissorTestEnable(bool value);

		//A built-in state object with settings for not culling any primitives.
		static std::unique_ptr<RasterizerState> CullNone();
		//A built-in state object with settings for culling primitives with clockwise winding order.
		static std::unique_ptr<RasterizerState> CullClockwise();
		//A built-in state object with settings for culling primitives with counter-clockwise winding order.
		static std::unique_ptr<RasterizerState> CullCounterClockwise();

		bool Initialize();
		bool Apply();	
	};	
}

#endif