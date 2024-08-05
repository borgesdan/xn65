#ifndef XNA_GRAPHICS_VERTEXPOSITION_HPP
#define XNA_GRAPHICS_VERTEXPOSITION_HPP

#include "../common/numerics.hpp"
#include "../common/color.hpp"
#include "../default.hpp"

namespace xna {
	//Describes a custom vertex format structure that contains position and color information. 
	struct VertexPositionColor {
		Vector3 Position{};
		xna::Color Color{};

		constexpr VertexPositionColor() = default;			
		constexpr VertexPositionColor(Vector3 const& position, xna::Color const& color):
			Position(position), Color(color){}

		constexpr bool operator==(const VertexPositionColor& other) const {
			return Position == other.Position && Color == other.Color;
		}
	};

	//Describes a custom vertex format structure that contains position and one set of texture coordinates.
	struct VertexPositionTexture {
		Vector3 Position{};
		Vector2 TextureCoordinate{};

		constexpr VertexPositionTexture() = default;		

		constexpr bool operator==(const VertexPositionTexture& other) const {
			return Position == other.Position
				&& TextureCoordinate == other.TextureCoordinate;
		}

		constexpr VertexPositionTexture(const Vector3& position, const Vector2& textureCoordinate)
			: Position(position), TextureCoordinate(textureCoordinate)
		{
		}
	};

	//Describes a custom vertex format structure that contains position, color, and one set of texture coordinates.
	struct VertexPositionColorTexture {
		Vector3 Position{};
		Vector2 TextureCoodinate{};
		xna::Color Color{};

		constexpr VertexPositionColorTexture() = default;

		constexpr bool operator==(const VertexPositionColorTexture& other) const {
			return Position == other.Position
				&& TextureCoodinate == other.TextureCoodinate
				&& Color == other.Color;
		}

		constexpr VertexPositionColorTexture(const Vector3& position, const Vector2& textureCoodinate, const xna::Color& color)
			: Position(position), TextureCoodinate(textureCoodinate), Color(color)
		{
		}
	};

	//Describes a custom vertex format structure that contains position, normal data, and one set of texture coordinates.
	struct VertexPositionNormalTexture {
		Vector3 Position{};
		Vector3 Normal{};
		Vector2 TextureCoodinate{};

		constexpr VertexPositionNormalTexture() = default;		

		bool operator==(const VertexPositionNormalTexture& other) const	{
			return Position == other.Position
				&& Normal == other.Normal
				&& TextureCoodinate == other.TextureCoodinate;
		}

		constexpr VertexPositionNormalTexture(const Vector3& position, const Vector3& normal, const Vector2& textureCoodinate)
			: Position(position), Normal(normal), TextureCoodinate(textureCoodinate)
		{
		}
	};
}

#endif