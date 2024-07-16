#include "xna/graphics/effect.hpp"
#include "xna/xna-dx.hpp"
#include "xna/common/math.hpp"

using DxBasicEffect = DirectX::BasicEffect;
using DxEffectFactory = DirectX::EffectFactory;
using DxVec3 = DirectX::XMFLOAT3;
using DxVec = DirectX::XMVECTOR;
using DxMatrix = DirectX::XMMATRIX;

namespace xna {
	Effect::Effect(sptr<GraphicsDevice> const& device) : GraphicsResource(device) {
		impl = unew<PlatformImplementation>();
	}

	BasicEffect::BasicEffect(sptr<GraphicsDevice> const& device) : Effect(device) {
		impl = unew<PlatformImplementation>();

		uptr<DxEffectFactory> fxFactory = unew<DxEffectFactory>(device->impl->_device.Get());
		DxEffectFactory::EffectInfo info;
		info.name = L"basic";
		info.alpha = 1.f;
		info.ambientColor = DxHelpers::Vector3ToDx(ambientLightColor);
		info.diffuseColor = DxHelpers::Vector3ToDx(diffuseColor);
		info.specularColor = DxHelpers::Vector3ToDx(specularColor);
		info.specularPower = specularPower;

		Effect::impl->dxEffect = fxFactory->CreateEffect(info, device->impl->_context.Get());
		impl->dxBasicEffect = reinterpret_pointer_cast<DxBasicEffect>(Effect::impl->dxEffect);			
	}

	void BasicEffect::Alpha(float value) {
		const auto a = MathHelper::Clamp(value, 0.0f, 1.0);
		impl->dxBasicEffect->SetAlpha(a);
	}

	void BasicEffect::AmbientLightColor(Vector3 const& value) {
		DxVec vec3 = DxHelpers::VectorToDx(value);
		impl->dxBasicEffect->SetAmbientLightColor(vec3);
	}

	void BasicEffect::DiffuseColor(Vector3 const& value) {
		DxVec vec3 = DxHelpers::VectorToDx(value);
		impl->dxBasicEffect->SetDiffuseColor(vec3);
	}

	void BasicEffect::EmissiveColor(Vector3 const& value) {
		DxVec vec3 = DxHelpers::VectorToDx(value);
		impl->dxBasicEffect->SetEmissiveColor(vec3);
	}

	void BasicEffect::FogColor(Vector3 const& value) {
		DxVec vec3 = DxHelpers::VectorToDx(value);
		impl->dxBasicEffect->SetFogColor(vec3);
	}

	void BasicEffect::FogEnabled(bool value) {
		impl->dxBasicEffect->SetFogEnabled(value);
	}

	void BasicEffect::FogEnd(float value) {
		impl->dxBasicEffect->SetFogEnd(value);
	}

	void BasicEffect::FogStart(float value) {
		impl->dxBasicEffect->SetFogStart(value);
	}

	void BasicEffect::LightingEnabled(bool value) {
		impl->dxBasicEffect->SetLightingEnabled(value);		
	}

	void BasicEffect::PreferPerPixelLighting(bool value) {
		impl->dxBasicEffect->SetPerPixelLighting(value);
	}

	void BasicEffect::Projection(Matrix const& value) {
		auto m = DxHelpers::MatrixToDx(value);
		impl->dxBasicEffect->SetProjection(m);
	}

	void BasicEffect::SpecularColor(Vector3 const& value) {
		DxVec vec3 = DxHelpers::VectorToDx(value);
		impl->dxBasicEffect->SetSpecularColor(vec3);
	}

	void BasicEffect::SpecularPower(float value) {
		impl->dxBasicEffect->SetSpecularPower(value);		
	}

	void BasicEffect::View(Matrix const& value) {
		auto m = DxHelpers::MatrixToDx(value);
		impl->dxBasicEffect->SetView(m);
	}

	void BasicEffect::World(Matrix const& value) {
		auto m = DxHelpers::MatrixToDx(value);
		impl->dxBasicEffect->SetWorld(m);
	}

	void BasicEffect::Texture(sptr<xna::Texture2D> const& value) {
		if (!value || !value->impl || !value->impl->dxShaderResource)
			Exception::Throw(Exception::ARGUMENT_IS_NULL);

		impl->dxBasicEffect->SetTexture(value->impl->dxShaderResource.Get());
	}

	void BasicEffect::TextureEnabled(bool value) {
		impl->dxBasicEffect->SetTextureEnabled(value);
	}

	void BasicEffect::VertexColorEnabled(bool value) {
		impl->dxBasicEffect->SetVertexColorEnabled(value);
	}

	void BasicEffect::SetDirectionalLight(Int index, DirectionalLight const& direction) {
		DxVec vec3 = DxHelpers::VectorToDx(direction.Direction);

		const auto value = static_cast<Int>(MathHelper::Clamp(static_cast<float>(index), 0, impl->dxBasicEffect->MaxDirectionalLights));

		impl->dxBasicEffect->SetLightDirection(value, vec3);
	}
}