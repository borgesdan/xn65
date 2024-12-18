#ifndef XNA_GRAPHICS_DEPTHSTENCILSTATE_HPP
#define XNA_GRAPHICS_DEPTHSTENCILSTATE_HPP

#include "gresource.hpp"
#include "shared.hpp"
#include <cstdint>
#include <memory>

namespace xna {
	enum class StencilOperation
	{
		Keep,
		Zero,
		Replace,
		IncrementSaturation,
		DecrementSaturation,
		Invert,
		Increment,
		Decrement,
	};	

	struct DepthStencilStateImplementation;

	//Contains depth-stencil state for the device. 
	class DepthStencilState : public GraphicsResource {
	public:
		DepthStencilState();
		DepthStencilState(std::shared_ptr<GraphicsDevice> const& device);
		
		//Gets or sets the stencil operation to perform if the stencil test passes and the depth-buffer test fails for a counterclockwise triangle.
		//The default is StencilOperation.Keep. 
		void CounterClockwiseStencilDepthBufferFail(StencilOperation value);
		//Gets or sets the stencil operation to perform if the stencil test passes and the depth-buffer test fails for a counterclockwise triangle.
		//The default is StencilOperation.Keep. 
		StencilOperation CounterClockwiseStencilDepthBufferFail() const;
		//Gets or sets the stencil operation to perform if the stencil test fails for a counterclockwise triangle. 
		//The default is StencilOperation.Keep.
		void CounterClockwiseStencilFail(StencilOperation value);
		//Gets or sets the stencil operation to perform if the stencil test fails for a counterclockwise triangle. 
		//The default is StencilOperation.Keep.
		StencilOperation CounterClockwiseStencilFail() const;
		//Gets or sets the comparison function to use for counterclockwise stencil tests. The default is CompareFunction.Always. 
		void CounterClockwiseStencilFunction(CompareFunction value);
		//Gets or sets the comparison function to use for counterclockwise stencil tests. The default is CompareFunction.Always. 
		CompareFunction CounterClockwiseStencilFunction() const;
		//Gets or sets the stencil operation to perform if the stencil and depth-tests pass for a counterclockwise triangle. 
		//The default is StencilOperation.Keep. 
		void CounterClockwiseStencilPass(StencilOperation value);
		//Gets or sets the stencil operation to perform if the stencil and depth-tests pass for a counterclockwise triangle. 
		//The default is StencilOperation.Keep. 
		StencilOperation CounterClockwiseStencilPass() const;
		
		//Enables or disables depth buffering. The default is true.
		void DepthBufferEnable(bool value);
		//Enables or disables depth buffering. The default is true.
		bool DepthBufferEnable() const;
		//Enables or disables writing to the depth buffer. The default is true. 
		void DepthBufferWriteEnable(bool value);
		//Enables or disables writing to the depth buffer. The default is true. 
		bool DepthBufferWriteEnable() const;
		//Gets or sets the comparison function for the depth-buffer test. The default is CompareFunction.LessEqual
		void DepthBufferFunction(CompareFunction value);
		//Gets or sets the comparison function for the depth-buffer test. The default is CompareFunction.LessEqual
		CompareFunction DepthBufferFunction() const;		
		
		//Gets or sets stencil enabling. The default is false.
		void StencilEnable(bool value);
		//Gets or sets stencil enabling. The default is false. 
		bool StencilEnable() const;
		//Gets or sets the stencil operation to perform if the stencil test fails. The default is StencilOperation.Keep. 
		void StencilFail(StencilOperation value);
		//Gets or sets the stencil operation to perform if the stencil test fails. The default is StencilOperation.Keep. 
		StencilOperation StencilFail() const;	
		//Gets or sets the comparison function for the stencil test.The default is CompareFunction.Always.
		void StencilFunction(CompareFunction value);
		//Gets or sets the comparison function for the stencil test. The default is CompareFunction.Always. 
		CompareFunction StencilFunction() const;		
		//Gets or sets the mask applied to the reference value and each stencil buffer entry to determine the significant bits for the stencil test.
		//The default mask is Int32.MaxValue. 
		void StencilMask(int32_t value);
		//Gets or sets the mask applied to the reference value and each stencil buffer entry to determine the significant bits for the stencil test.
		//The default mask is Int32.MaxValue. 
		int32_t StencilMask() const;
		//Gets or sets the write mask applied to values written into the stencil buffer. The default mask is Int32.MaxValue. 
		void StencilWriteMask(int32_t value);
		//Gets or sets the write mask applied to values written into the stencil buffer. The default mask is Int32.MaxValue. 
		int32_t StencilWriteMask() const;
		//Gets or sets the stencil operation to perform if the stencil test passes. The default is StencilOperation.Keep. 
		void StencilPass(StencilOperation value);
		//Gets or sets the stencil operation to perform if the stencil test passes. The default is StencilOperation.Keep. 
		StencilOperation StencilPass() const;
		//Gets or sets the stencil operation to perform if the stencil test passes and the depth-test fails. The default is StencilOperation.Keep. 
		void StencilDepthBufferFail(StencilOperation value);
		//Gets or sets the stencil operation to perform if the stencil test passes and the depth-test fails. The default is StencilOperation.Keep. 
		StencilOperation StencilDepthBufferFail() const;		

		//A built-in state object with settings for not using a depth stencil buffer.
		static std::unique_ptr<DepthStencilState> None();
		//A built-in state object with default settings for using a depth stencil buffer.
		static std::unique_ptr<DepthStencilState> Default();
		//A built-in state object with settings for enabling a read-only depth stencil buffer.
		static std::unique_ptr<DepthStencilState> DepthRead();

		bool Initialize();
		bool Apply();	

		std::unique_ptr<DepthStencilStateImplementation> Implementation;
	};
}

#endif