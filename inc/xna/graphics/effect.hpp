#ifndef XNA_GRAPHICS_EFFECT_HPP
#define XNA_GRAPHICS_EFFECT_HPP

#include "../common/numerics.hpp"
#include "../default.hpp"
#include "gresource.hpp"

namespace xna {
	//Represents an annotation to an EffectParameter. 
	class EffectAnnotation {
	public:
		Int ColumCount() const;
		String Name() const;
		EffectParameterClass ParameterClass() const;
		Int RowCount() const;
		String Semantic() const;
		bool GetValueBoolean() const;
		Int GetValueInt32() const;
		Matrix GetValueMatrix() const;
		float GetValueSingle() const;
		String GetValueString() const;
		Vector2 GetValueVector2() const;
		Vector3 GetValueVector3() const;
		Vector4 GetValueVector4() const;

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl;

	public:
		EffectAnnotation();
	};
	using PEffectAnnotation = sptr<EffectAnnotation>;

	class EffectAnnotationCollection {
	public:
		EffectAnnotationCollection() {}

		EffectAnnotationCollection(std::vector<PEffectAnnotation> const& data) : data(data)
		{
		}

		constexpr size_t Count() const {
			return data.size();
		}

		PEffectAnnotation operator[](size_t index) {
			if (index >= data.size())
				return nullptr;

			return data[index];
		}

		PEffectAnnotation operator[](String const& name) {
			for (size_t i = 0; i < data.size(); ++i) {
				const auto& p = data[i];

				if (p->Name() == name)
					return p;
			}

			return nullptr;
		}

	public:
		std::vector<PEffectAnnotation> data;
	};
	using PEffectAnnotationCollection = sptr<EffectAnnotationCollection>;

	class EffectPass {
	public:
		//Gets the name of this pass. 
		String Name() const;
		//The EffectAnnotationCollection containing EffectAnnotation objects for this EffectPass.
		PEffectAnnotationCollection Annotations() const;

		//Begins this pass.
		void Apply();
	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl;

	public:
		EffectPass();
	};
	using PEffectPass = sptr<EffectPass>;

	class EffectPassCollection {
	public:
		EffectPassCollection() {}

		EffectPassCollection(std::vector<PEffectPass> const& data) : data(data)
		{
		}

		constexpr size_t Count() const {
			return data.size();
		}

		PEffectPass operator[](size_t index) {
			if (index >= data.size())
				return nullptr;

			return data[index];
		}

		PEffectPass operator[](String const& name) {
			for (size_t i = 0; i < data.size(); ++i) {
				const auto& p = data[i];

				if (p->Name() == name)
					return p;
			}

			return nullptr;
		}

	public:
		std::vector<PEffectPass> data;
	};
	using PEffectPassCollection = sptr<EffectPassCollection>;

	class EffectTechnique {
	public:
		PEffectAnnotationCollection Annotations() const;
		String Name() const;
		PEffectPassCollection Passes() const;

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl;

	public:
		EffectTechnique();
	};
	using PEffectTechnique = sptr<EffectTechnique>;

	class EffectParameterCollection;

	class EffectParameter {
	public:
		//Gets the collection of EffectAnnotation objects for this parameter.
		PEffectAnnotationCollection Annotations() const;
		//Gets the number of columns in the parameter description.
		Int ColumnCount() const;
		//Gets the number of rows in the parameter description.
		Int RowCount() const;
		//Gets the semantic meaning, or usage, of the parameter.
		String Semantic() const;
		//Gets the type of the parameter.
		EffectParameterType ParameterType() const;
		//Gets the class of the parameter.
		EffectParameterClass ParameterClass() const;
		//Gets the name of the parameter.
		String Name() const;
		//Gets the collection of effect parameters.
		sptr<EffectParameterCollection> Elements() const;
		//Gets the collection of structure members.
		sptr<EffectParameterCollection> StructureMembers() const;

		bool GetValueBoolean() const;
		std::vector<bool> GetValueBooleanArray(size_t count) const;
		Int GetValueInt32() const;
		std::vector <Int> GetValueInt32Array(size_t count) const;
		Matrix GetValueMatrix() const;
		std::vector <Matrix> GetValueMatrixArray(size_t count) const;
		Matrix GetValueMatrixTranspose() const;
		std::vector <Matrix> GetValueMatrixTransposeArray(size_t count) const;
		Quaternion GetValueQuaternion() const;
		std::vector <Quaternion> GetValueQuaternionArray() const;
		float GetValueSingle() const;
		std::vector<float> GetValueSingleArray() const;
		String GetValueString() const;
		sptr<Texture2D> GetValueTexture2D() const;
		sptr<Texture3D> GetValueTexture3D() const;
		sptr<TextureCube> GetValueTextureCube() const;
		Vector2 GetValueVector2() const;
		std::vector <Vector2> GetValueVector2Array() const;
		Vector3 GetValueVector3() const;
		std::vector <Vector3> GetValueVector3Array() const;
		Vector4 GetValueVector4() const;
		std::vector <Vector4> GetValueVector4Array() const;

		void SetValue(bool value);
		void SetValue(std::vector<bool> const& value);
		void SetValue(Int value);
		void SetValue(std::vector<Int> const& value);
		void SetValue(float value);
		void SetValue(std::vector<float> const& value);
		void SetValue(Matrix const& value);
		void SetValue(std::vector<Matrix> const& value);
		void SetValue(Quaternion const& value);
		void SetValue(std::vector<Quaternion> const& value);
		void SetValue(Vector2 const& value);
		void SetValue(std::vector<Vector2> const& value);
		void SetValue(Vector3 const& value);
		void SetValue(std::vector<Vector3> const& value);
		void SetValue(Vector4 const& value);
		void SetValue(std::vector<Vector4> const& value);
		void SetValue(String const& value);
		void SetValue(sptr<Texture> const& value);

		void SetValueTranspose(Matrix const& value);
		void SetValueTranspose(std::vector<Matrix> const& value);

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl;

	public:
		EffectParameter();
	};
	using PEffectParameter = sptr<EffectParameter>;

	class EffectParameterCollection {
	public:
		EffectParameterCollection() {}

		EffectParameterCollection(std::vector<PEffectParameter> const& data) : data(data)
		{
		}

		constexpr size_t Count() const {
			return data.size();
		}

		PEffectParameter operator[](size_t index) {
			if (index >= data.size())
				return nullptr;

			return data[index];
		}

		PEffectParameter operator[](String const& name) {
			for (size_t i = 0; i < data.size(); ++i) {
				const auto& p = data[i];

				if (p->Name() == name)
					return p;
			}

			return nullptr;
		}

	public:
		std::vector<PEffectParameter> data;
	};
	using PEffectPassCollection = sptr<EffectPassCollection>;

	class Effect : public GraphicsResource {
		Effect(sptr<GraphicsDevice> const& device, std::vector<Byte> const& effectCode);

		PEffectTechnique CurrentTechnique() const;

	public:
		struct PlatformImplementation;
		uptr<PlatformImplementation> impl;
	};

	class IEffectMatrices {
		virtual Matrix World() const = 0;
		virtual Matrix View() const = 0;
		virtual Matrix Projection() const = 0;

		virtual void World(Matrix const& value) = 0;
		virtual void View(Matrix const& value) = 0;
		virtual void Projection(Matrix const& value) = 0;
	};

	class DirectionalLight;

	class IEffectLights {
		virtual DirectionalLight DirectionalLight0() const = 0;
		virtual DirectionalLight DirectionalLight1() const = 0;
		virtual DirectionalLight DirectionalLight2() const = 0;

		virtual Vector3 AmbientLightColor() const = 0;
		virtual void AmbientLightColor(Vector3 const& value) = 0;

		virtual bool LightingEnabled() const = 0;
		virtual void LightingEnabled(bool value) = 0;

		virtual void EnableDefaultLighting() = 0;
	};

	class IEffectFog
	{
		virtual bool FogEnabled() const = 0;
		virtual float FogStart() const = 0;
		virtual float FogEnd() const = 0;
		virtual Vector3 FogColor() const = 0;

		virtual void FogEnabled(bool value) const = 0;
		virtual void FogStart(float value) const = 0;
		virtual void FogEnd(float value) const = 0;
		virtual void FogColor(Vector3 const& value) const = 0;
	};
}

#endif