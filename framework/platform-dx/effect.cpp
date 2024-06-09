#include "xna/graphics/effect.hpp"
#include "xna/platform-dx/dx.hpp"

namespace xna {
	Effect::Effect(sptr<GraphicsDevice> const& device, std::vector<Byte> const& effectCode) :
		GraphicsResource(device) {

		if (!device)
			throw std::invalid_argument("Effect::Effect: invalid argument (device).");

		impl = unew<PlatformImplementation>();

		const auto result = D3DX11CreateEffectFromMemory(
			//void* pData,
			effectCode.data(),
			//SIZE_T        DataLength,
			effectCode.size(),
			//UINT          FXFlags,
			0,
			//ID3D11Device * pDevice,
			device->impl->_device,
			//ID3DX11Effect * *ppEffect
			&impl->dxEffect
		);				

		if FAILED(result)
			throw std::runtime_error("Effect::Effect: Unable to create an effect with memory data.");
	}

	EffectAnnotation::EffectAnnotation() {
		impl = unew<PlatformImplementation>();
	}

	Int EffectAnnotation::ColumCount() const {
		auto type = impl->dxVariable->GetType();
		D3DX11_EFFECT_TYPE_DESC desc{};
		type->GetDesc(&desc);

		return static_cast<Int>(desc.Columns);
	}

	String EffectAnnotation::Name() const {
		auto type = impl->dxVariable->GetType();
		D3DX11_EFFECT_TYPE_DESC desc{};
		type->GetDesc(&desc);

		return String(desc.TypeName);
	}

	EffectParameterClass EffectAnnotation::ParameterClass() const {
		auto type = impl->dxVariable->GetType();
		D3DX11_EFFECT_TYPE_DESC desc{};
		type->GetDesc(&desc);

		switch (desc.Class)
		{
		case D3D_SHADER_VARIABLE_CLASS::D3D_SVC_MATRIX_COLUMNS:
		case D3D_SHADER_VARIABLE_CLASS::D3D_SVC_MATRIX_ROWS:
			return EffectParameterClass::Matrix;
		case D3D_SHADER_VARIABLE_CLASS::D3D_SVC_OBJECT:
			return EffectParameterClass::Object;
		case D3D_SHADER_VARIABLE_CLASS::D3D_SVC_SCALAR:
			return EffectParameterClass::Scalar;
		case D3D_SHADER_VARIABLE_CLASS::D3D_SVC_STRUCT:
			return EffectParameterClass::Struct;
		case D3D_SHADER_VARIABLE_CLASS::D3D_SVC_VECTOR:
			return EffectParameterClass::Vector;
		default:
			throw std::runtime_error(" EffectAnnotation::ParameterClass: invalid EffectParameterClass.");
		}
	}

	Int EffectAnnotation::Rowcount() const {
		auto type = impl->dxVariable->GetType();
		D3DX11_EFFECT_TYPE_DESC desc{};
		type->GetDesc(&desc);

		return static_cast<Int>(desc.Rows);
	}

	String EffectAnnotation::Semantic() const {
		auto type = impl->dxVariable->GetType();
		auto semantic = type->GetMemberSemantic(0);
		return std::string(semantic);
	}

	bool EffectAnnotation::GetValueBoolean() const {
		auto type = impl->dxVariable->GetType();
		auto scalar = impl->dxVariable->AsScalar();
		
		bool value = false;
		auto hr = scalar->GetBool(&value);

		if FAILED(hr)
			throw std::runtime_error("EffectAnnotation::GetValueBoolean: Unable to get boolean value.");

		return value;
	}

	Int EffectAnnotation::GetValueInt32() const {
		auto type = impl->dxVariable->GetType();
		auto scalar = impl->dxVariable->AsScalar();

		int value = 0;
		auto hr = scalar->GetInt(&value);

		if FAILED(hr)
			throw std::runtime_error("EffectAnnotation::GetValueBoolean: Unable to get interger value.");

		return static_cast<Int>(value);
	}

	Matrix EffectAnnotation::GetValueMatrix() const {
		auto type = impl->dxVariable->GetType();
		auto matrix = impl->dxVariable->AsMatrix();

		float values[16];
		auto hr = matrix->GetMatrix(values);

		if FAILED(hr)
			throw std::runtime_error("EffectAnnotation::GetValueBoolean: Unable to get matrix value.");

		Matrix m;
		m.M11 = values[0];
		m.M12 = values[1];
		m.M13 = values[2];
		m.M14 = values[3];
		m.M21 = values[4];
		m.M22 = values[5];
		m.M23 = values[6];
		m.M24 = values[7];
		m.M31 = values[8];
		m.M32 = values[9];
		m.M33 = values[10];
		m.M34 = values[11];
		m.M41 = values[12];
		m.M42 = values[13];
		m.M43 = values[14];
		m.M44 = values[15];

		return m;
	}

	float EffectAnnotation::GetValueSingle() const {
		auto type = impl->dxVariable->GetType();
		auto scalar = impl->dxVariable->AsScalar();

		float value = 0;
		auto hr = scalar->GetFloat(&value);

		if FAILED(hr)
			throw std::runtime_error("EffectAnnotation::GetValueBoolean: Unable to get float value.");

		return value;
	}

	String EffectAnnotation::GetValueString() const {
		return {};
	}

	Vector2 EffectAnnotation::GetValueVector2() const {
		auto type = impl->dxVariable->GetType();
		auto scalar = impl->dxVariable->AsScalar();

		float values[2];
		auto hr = scalar->GetFloatArray(values, 0, 2);

		if FAILED(hr)
			throw std::runtime_error("EffectAnnotation::GetValueBoolean: Unable to get Vector2 value.");

		Vector2 v;
		v.X = values[0];
		v.Y = values[1];

		return v;
	}

	Vector3 EffectAnnotation::GetValueVector3() const {
		auto type = impl->dxVariable->GetType();
		auto scalar = impl->dxVariable->AsScalar();

		float values[3];
		auto hr = scalar->GetFloatArray(values, 0, 3);

		if FAILED(hr)
			throw std::runtime_error("EffectAnnotation::GetValueBoolean: Unable to get Vector3 value.");

		Vector3 v;
		v.X = values[0];
		v.Y = values[1];
		v.Z = values[2];

		return v;
	}

	Vector4 EffectAnnotation::GetValueVector4() const {
		auto type = impl->dxVariable->GetType();
		auto scalar = impl->dxVariable->AsScalar();

		float values[4];
		auto hr = scalar->GetFloatArray(values, 0, 4);

		if FAILED(hr)
			throw std::runtime_error("EffectAnnotation::GetValueBoolean: Unable to get Vector4 value.");

		Vector4 v;
		v.X = values[0];
		v.Y = values[1];
		v.Z = values[2];
		v.W = values[3];

		return v;
	}
}