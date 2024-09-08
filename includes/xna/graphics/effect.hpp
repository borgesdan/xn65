#ifndef XNA_GRAPHICS_EFFECT_HPP
#define XNA_GRAPHICS_EFFECT_HPP

#include "../common/numerics.hpp"
#include "../default.hpp"
#include "gresource.hpp"
#include "../graphics/texture.hpp"

namespace xna {
	//Gets or sets transformation matrix parameters for the current effect. 
	class IEffectMatrices {
		//Gets or sets the projection matrix in the current effect.
		virtual Matrix World() const = 0;
		//Gets or sets the view matrix in the current effect.
		virtual Matrix View() const = 0;
		//Gets or sets the world matrix in the current effect.
		virtual Matrix Projection() const = 0;

		//Gets or sets the projection matrix in the current effect.
		virtual void World(Matrix const& value) = 0;
		//Gets or sets the view matrix in the current effect.
		virtual void View(Matrix const& value) = 0;
		//Gets or sets the world matrix in the current effect.
		virtual void Projection(Matrix const& value) = 0;
	};

	//Creates a DirectionalLight object. 
	struct DirectionalLight {
		//Gets or sets the diffuse color of the light.
		Vector3 DiffuseColor{ Vector3::Zero() };
		//Gets or sets the light direction. This value must be a unit vector.
		Vector3 Direction{ Vector3::Zero() };
		//Gets or sets the specular color of the light.
		Vector3 SpecularColor{ Vector3::Zero() };
		//Gets or sets light enable flag.
		bool Enabled{ false };
	};

	//Gets or sets lighting parameters for the current effect. 
	class IEffectLights {
		//Gets the first directional light for the current effect.
		virtual DirectionalLight DirectionalLight0() const = 0;
		//Gets the second directional light for the current effect.
		virtual DirectionalLight DirectionalLight1() const = 0;
		//Gets the third directional light for the current effect.
		virtual DirectionalLight DirectionalLight2() const = 0;

		//Gets or sets a value indicating that per-pixel lighting should be used if it is available for the current adapter.
		virtual bool PreferPerPixelLighting() const = 0;
		//Gets or sets a value indicating that per-pixel lighting should be used if it is available for the current adapter.
		virtual void PreferPerPixelLighting(bool value) = 0;

		//Gets or sets the ambient light color for the current effect.
		virtual Vector3 AmbientLightColor() const = 0;
		//Gets or sets the ambient light color for the current effect.
		virtual void AmbientLightColor(Vector3 const& value) = 0;

		//Enables or disables lighting in an IEffectLights.
		virtual bool LightingEnabled() const = 0;
		//Enables or disables lighting in an IEffectLights.
		virtual void LightingEnabled(bool value) = 0;
	};

	//Gets or sets fog parameters for the current effect. 
	class IEffectFog
	{
		//Enables or disables fog.
		virtual bool FogEnabled() const = 0;
		//Gets or sets the fog ending distance.
		virtual float FogStart() const = 0;
		//Gets or sets the fog ending distance.
		virtual float FogEnd() const = 0;
		//Gets or sets the fog color.
		virtual Vector3 FogColor() const = 0;

		//Enables or disables fog.
		virtual void FogEnabled(bool value) = 0;
		//Gets or sets the fog ending distance.
		virtual void FogStart(float value) = 0;
		//Gets or sets the fog ending distance.
		virtual void FogEnd(float value) = 0;
		//Gets or sets the fog color.
		virtual void FogColor(Vector3 const& value) = 0;
	};

	class Effect : public GraphicsResource {
	public:
		Effect(sptr<GraphicsDevice> const& device);

		virtual ~Effect() {}

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl;
	};

	//Contains a basic rendering effect. 
	class BasicEffect : public Effect, public IEffectMatrices, public IEffectLights, public IEffectFog {
	public:
		BasicEffect(sptr<GraphicsDevice> const& device);
		
		//
		// IEffectMatrices
		//

		//Gets or sets the world matrix.
		virtual Matrix World() const override { return world; }
		//Gets or sets the world matrix.
		virtual void World(Matrix const& value) override;
		//Gets or sets the view matrix.
		virtual constexpr Matrix View() const override { return view; }
		//Gets or sets the view matrix.
		virtual void View(Matrix const& value) override;		
		//Gets or sets the projection matrix. Use this matrix to change how a 3D image is converted to a 2D image that is rendered to the computer screen.
		virtual constexpr Matrix Projection() const override { return projection; }
		//Gets or sets the projection matrix. Use this matrix to change how a 3D image is converted to a 2D image that is rendered to the computer screen.
		virtual void Projection(Matrix const& value) override;

		//
		// IEffectLights
		//
		
		//Gets the first directional light for this effect. 
		virtual DirectionalLight DirectionalLight0() const override { return directionalLight0; }
		//Gets the second directional light for this effect.
		virtual DirectionalLight DirectionalLight1() const override { return directionalLight1; }
		//Gets the second directional light for this effect.
		virtual DirectionalLight DirectionalLight2() const override { return directionalLight2; }

		//Gets or sets a value indicating that per-pixel lighting should be used if it is available for the current adapter.
		virtual constexpr bool PreferPerPixelLighting() const override { return preferPerPixelLighting; }
		//Gets or sets a value indicating that per-pixel lighting should be used if it is available for the current adapter.
		virtual void PreferPerPixelLighting(bool value) override;

		//Gets or sets the ambient color for a light, the range of color values is from 0 to 1. 
		virtual constexpr Vector3 AmbientLightColor() const override { return ambientLightColor; }
		//Gets or sets the ambient color for a light, the range of color values is from 0 to 1. 
		virtual void AmbientLightColor(Vector3 const& value) override;

		//Enables lighting for this effect. 
		virtual constexpr bool LightingEnabled() const override { return lightingEnabled; }
		//Enables lighting for this effect. 
		virtual void LightingEnabled(bool value) override;

		//
		// IEffectFog
		//
				
		//Gets or sets the minimum z value for fog, which ranges from 0 to 1.
		virtual constexpr float FogStart() const override { return fogStart; }
		//Gets or sets the minimum z value for fog, which ranges from 0 to 1.
		virtual void FogStart(float value) override;

		//Gets or sets the maximum z value for fog, which ranges from 0 to 1.
		virtual constexpr float FogEnd() const override { return fogEnd; }
		//Gets or sets the maximum z value for fog, which ranges from 0 to 1.
		virtual void FogEnd(float value) override;

		//Enables fog.
		virtual constexpr bool FogEnabled() const override { return fogEnabled; }
		//Enables fog.
		virtual void FogEnabled(bool value) override;

		//Gets or sets the fog color, the range of color values is from 0 to 1.
		virtual constexpr Vector3 FogColor() const override { return fogColor; }
		//Gets or sets the fog color, the range of color values is from 0 to 1.
		virtual void FogColor(Vector3 const& value) override;

		//
		// BasicEffect
		//

		//Gets or sets the material alpha which determines its transparency. 
		//Range is between 1 (fully opaque) and 0 (fully transparent). 
		constexpr float Alpha() const {	return alpha; }
		////Gets or sets the material alpha which determines its transparency. 
		//Range is between 1 (fully opaque) and 0 (fully transparent). 
		void Alpha(float value);		
		//Gets or sets the diffuse color for a material, the range of color values is from 0 to 1. 
		constexpr Vector3 DiffuseColor() const { return diffuseColor; }
		//Gets or sets the diffuse color for a material, the range of color values is from 0 to 1. 
		void DiffuseColor(Vector3 const& value);		
		//Gets or sets the emissive color for a material, the range of color values is from 0 to 1. 
		constexpr Vector3 EmissiveColor() const { return emissiveColor; }
		//Gets or sets the emissive color for a material, the range of color values is from 0 to 1. 
		void EmissiveColor(Vector3 const& value);
		//Gets or sets the emissive color for a material, the range of color values is from 0 to 1. 
		constexpr Vector3 SpecularColor() const { return specularColor; }
		//Gets or sets the emissive color for a material, the range of color values is from 0 to 1. 
		void SpecularColor(Vector3 const& value);
		//Gets or sets the specular power of this effect material.
		constexpr float SpecularPower() const { return specularPower; }
		//Gets or sets the specular power of this effect material.
		void SpecularPower(float value);
		//Gets or sets a texture to be applied by this effect.
		sptr<xna::Texture2D> Texture() const { return texture; }
		//Gets or sets a texture to be applied by this effect.
		void Texture(sptr<xna::Texture2D> const& value);
		//Enables textures for this effect. 
		constexpr bool TextureEnabled() const { return textureEnabled; }
		//Enables textures for this effect. 
		void TextureEnabled(bool value);		
		//Enables textures for this effect. 
		constexpr bool VertexColorEnabled() const { return vertexColorEnabled; }
		//Enables textures for this effect. 
		void VertexColorEnabled(bool value);

		void SetDirectionalLight(Int index, DirectionalLight const& direction);

	private:
		bool fogEnabled{ false };
		bool lightingEnabled{ false };
		bool preferPerPixelLighting{ false };
		bool textureEnabled{ false };
		bool vertexColorEnabled{ false };
		float alpha{0};
		float fogStart{0};
		float fogEnd{0};
		float specularPower{ 16.0f};
		Vector3 ambientLightColor{ Vector3::Zero() };
		Vector3 diffuseColor{ Vector3::One() };
		Vector3 emissiveColor{ Vector3::Zero() };
		Vector3 specularColor{ Vector3::One() };
		Vector3 fogColor{};
		Matrix projection{ Matrix::Identity() };
		Matrix view{ Matrix::Identity() };
		Matrix world{ Matrix::Identity() };
		DirectionalLight directionalLight0{};
		DirectionalLight directionalLight1{};
		DirectionalLight directionalLight2{};
		sptr<Texture2D> texture{ nullptr };

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl;
	};
}

#endif