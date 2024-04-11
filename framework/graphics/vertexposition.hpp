#ifndef XNA_GRAPHICS_VERTEXPOSITION_HPP
#define XNA_GRAPHICS_VERTEXPOSITION_HPP

#include "../common/vectors.hpp"
#include "../common/color.hpp"
#include "../default.hpp"

namespace xna {
	struct VertexPositionColor {
		Vector3 position{};
		Color color{};

		constexpr VertexPositionColor() = default;			

		constexpr bool operator==(const VertexPositionColor& other) const {
			return position == other.position && color == other.color;
		}
	};

	struct VertexPositionTexture {
		Vector3 position{};
		Vector2 textureCoordinate{};

		constexpr VertexPositionTexture() = default;		

		constexpr bool operator==(const VertexPositionTexture& other) const {
			return position == other.position 
				&& textureCoordinate == other.textureCoordinate;
		}
	};

	struct VertexPositionColorTexture {
		Vector3 position{};
		Vector2 textureCoodinate{};
		Color color{};

		constexpr VertexPositionColorTexture() = default;

		constexpr bool operator==(const VertexPositionColorTexture& other) const {
			return position == other.position 
				&& textureCoodinate == other.textureCoodinate 
				&& color == other.color;
		}
	};

	struct VertexPositionNormalTexture {
		Vector3 position{};
		Vector3 normal{};
		Vector2 textureCoodinate{};

		constexpr VertexPositionNormalTexture() = default;		

		bool operator==(const VertexPositionNormalTexture& other) const	{
			return position == other.position 
				&& normal == other.normal 
				&& textureCoodinate == other.textureCoodinate;
		}
	};
}

#endif