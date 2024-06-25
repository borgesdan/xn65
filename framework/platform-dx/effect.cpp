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
			device->impl->_device.Get(),
			//ID3DX11Effect * *ppEffect
			impl->dxEffect.ReleaseAndGetAddressOf()
		);

		if FAILED(result)
			throw std::runtime_error("Effect::Effect: Unable to create an effect with memory data.");
	}

	PEffectTechnique Effect::CurrentTechnique() const {
		D3DX11_EFFECT_DESC desc;
		impl->dxEffect->GetDesc(&desc);

		auto tech = impl->dxEffect->GetTechniqueByIndex(0);

		auto technique = snew<EffectTechnique>();
		technique->impl->dxContext = m_device->impl->_context;
		technique->impl->dxContext->AddRef();

		tech->Release();
		tech = nullptr;

		return technique;
	}

	EffectAnnotation::EffectAnnotation() {
		impl = unew<PlatformImplementation>();
	}

	Int EffectAnnotation::ColumCount() const {
		auto type = impl->dxVariable->GetType();
		D3DX11_EFFECT_TYPE_DESC desc{};
		const auto hr = type->GetDesc(&desc);

		type->Release();
		type = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectAnnotation::ColumCount: error getting D3DX11_EFFECT_TYPE_DESC");

		return static_cast<Int>(desc.Columns);
	}

	String EffectAnnotation::Name() const {
		auto type = impl->dxVariable->GetType();
		D3DX11_EFFECT_TYPE_DESC desc{};
		const auto hr = type->GetDesc(&desc);

		type->Release();
		type = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectAnnotation::Name: error getting D3DX11_EFFECT_TYPE_DESC");

		return String(desc.TypeName);
	}

	EffectParameterClass EffectAnnotation::ParameterClass() const {
		auto type = impl->dxVariable->GetType();
		D3DX11_EFFECT_TYPE_DESC desc{};
		const auto hr = type->GetDesc(&desc);

		type->Release();
		type = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectAnnotation::ParameterClass: error getting D3DX11_EFFECT_TYPE_DESC");

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
			throw std::runtime_error("EffectAnnotation::ParameterClass: invalid EffectParameterClass.");
		}
	}

	Int EffectAnnotation::RowCount() const {
		auto type = impl->dxVariable->GetType();
		D3DX11_EFFECT_TYPE_DESC desc{};
		const auto hr = type->GetDesc(&desc);

		type->Release();
		type = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectAnnotation::RowCount: error getting D3DX11_EFFECT_TYPE_DESC");

		return static_cast<Int>(desc.Rows);
	}

	String EffectAnnotation::Semantic() const {
		auto type = impl->dxVariable->GetType();
		auto semantic = type->GetMemberSemantic(0);

		type->Release();
		type = nullptr;

		return std::string(semantic);
	}

	bool EffectAnnotation::GetValueBoolean() const {
		auto scalar = impl->dxVariable->AsScalar();

		bool value;
		auto hr = scalar->GetBool(&value);

		scalar->Release();
		scalar = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectAnnotation::GetValueBoolean: Unable to get boolean value.");

		return value;
	}

	Int EffectAnnotation::GetValueInt32() const {
		auto scalar = impl->dxVariable->AsScalar();

		int value;
		auto hr = scalar->GetInt(&value);

		scalar->Release();
		scalar = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectAnnotation::GetValueInt32: Unable to get interger value.");

		return static_cast<Int>(value);
	}

	Matrix EffectAnnotation::GetValueMatrix() const {
		auto matrix = impl->dxVariable->AsMatrix();

		float values[16];
		auto hr = matrix->GetMatrix(values);
		matrix->Release();
		matrix = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectAnnotation::GetValueMatrix: Unable to get matrix value.");

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
		auto scalar = impl->dxVariable->AsScalar();

		float value;
		auto hr = scalar->GetFloat(&value);

		scalar->Release();
		scalar = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectAnnotation::GetValueSingle: Unable to get float value.");

		return value;
	}

	String EffectAnnotation::GetValueString() const {
		auto str = impl->dxVariable->AsString();

		LPCSTR data;
		const auto hr = str->GetString(&data);

		str->Release();
		str = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectAnnotation::GetValueString: Unable to get string value.");

		return String(data);
	}

	Vector2 EffectAnnotation::GetValueVector2() const {
		auto scalar = impl->dxVariable->AsScalar();

		float values[2];
		auto hr = scalar->GetFloatArray(values, 0, 2);

		scalar->Release();
		scalar = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectAnnotation::GetValueVector2: Unable to get Vector2 value.");

		Vector2 v;
		v.X = values[0];
		v.Y = values[1];

		return v;
	}

	Vector3 EffectAnnotation::GetValueVector3() const {
		auto scalar = impl->dxVariable->AsScalar();

		float values[3];
		auto hr = scalar->GetFloatArray(values, 0, 3);

		scalar->Release();
		scalar = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectAnnotation::GetValueVector3: Unable to get Vector3 value.");

		Vector3 v;
		v.X = values[0];
		v.Y = values[1];
		v.Z = values[2];

		return v;
	}

	Vector4 EffectAnnotation::GetValueVector4() const {
		auto scalar = impl->dxVariable->AsScalar();

		float values[4];
		auto hr = scalar->GetFloatArray(values, 0, 4);

		scalar->Release();
		scalar = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectAnnotation::GetValueVector4: Unable to get Vector4 value.");

		Vector4 v;
		v.X = values[0];
		v.Y = values[1];
		v.Z = values[2];
		v.W = values[3];

		return v;
	}

	EffectPass::EffectPass() {
		impl = unew<PlatformImplementation>();
	}

	String EffectPass::Name() const {
		if (!impl->dxPass)
			throw std::runtime_error("EffectPass::Name: The class was not initialized correctly");

		D3DX11_PASS_DESC desc{};
		impl->dxPass->GetDesc(&desc);

		return String(desc.Name);
	}

	PEffectAnnotationCollection EffectPass::Annotations() const {
		if (!impl->dxPass)
			throw std::runtime_error("EffectPass::Annotations: The class was not initialized correctly");

		D3DX11_PASS_DESC desc{};
		const auto hr = impl->dxPass->GetDesc(&desc);

		if FAILED(hr)
			throw std::runtime_error("EffectPass::Annotations: error getting D3DX11_PASS_DESC");

		auto annotCount = desc.Annotations;

		if (annotCount == 0)
			return snew<EffectAnnotationCollection>();

		std::vector<PEffectAnnotation> list(annotCount);

		for (size_t i = 0; i < annotCount; ++i) {
			auto current = impl->dxPass->GetAnnotationByIndex(i);
			auto annotation = snew<EffectAnnotation>();
			annotation->impl->dxVariable = current;
			annotation->impl->dxVariable->AddRef();

			current->Release();
			current = nullptr;

			list[i] = annotation;
		}

		auto collection = snew<EffectAnnotationCollection>(list);
		return collection;
	}

	void EffectPass::Apply() {
		if (!impl->dxPass)
			throw std::runtime_error("EffectPass::Apply: The class was not initialized correctly");

		const auto hr = impl->dxPass->Apply(0, impl->dxContext.Get());

		if FAILED(hr)
			throw std::runtime_error("EffectPass::Apply: error to call Apply");
	}

	EffectTechnique::EffectTechnique() {
		impl = unew<PlatformImplementation>();
	}

	String EffectTechnique::Name() const {
		D3DX11_TECHNIQUE_DESC desc;
		const auto hr = impl->dxTechnique->GetDesc(&desc);

		if FAILED(hr)
			throw std::runtime_error("EffectTechnique::Name: error getting D3DX11_TECHNIQUE_DESC");

		return String(desc.Name);
	}

	PEffectAnnotationCollection EffectTechnique::Annotations() const {
		D3DX11_TECHNIQUE_DESC desc;
		const auto hr = impl->dxTechnique->GetDesc(&desc);

		if FAILED(hr)
			throw std::runtime_error("EffectTechnique::Annotations: error getting D3DX11_TECHNIQUE_DESC");

		auto annotCount = desc.Annotations;

		if (annotCount == 0)
			return snew<EffectAnnotationCollection>();

		std::vector<PEffectAnnotation> list(annotCount);

		for (size_t i = 0; i < annotCount; ++i) {
			auto current = impl->dxTechnique->GetAnnotationByIndex(i);
			auto annotation = snew<EffectAnnotation>();
			annotation->impl->dxVariable = current;
			annotation->impl->dxVariable->AddRef();

			current->Release();
			current = nullptr;

			list[i] = annotation;
		}

		auto collection = snew<EffectAnnotationCollection>(list);
		return collection;
	}

	PEffectPassCollection EffectTechnique::Passes() const {
		D3DX11_TECHNIQUE_DESC desc;
		const auto hr = impl->dxTechnique->GetDesc(&desc);

		if FAILED(hr)
			throw std::runtime_error("EffectTechnique::Passes: error getting D3DX11_TECHNIQUE_DESC");

		auto passCount = desc.Passes;

		if (passCount == 0)
			return snew<EffectPassCollection>();

		std::vector<PEffectPass> list(passCount);

		for (size_t i = 0; i < passCount; ++i) {
			auto current = impl->dxTechnique->GetPassByIndex(i);

			auto pass = snew<EffectPass>();
			pass->impl->dxPass.Attach(current);

			current->Release();
			current = nullptr;

			pass->impl->dxContext = impl->dxContext;

			list[i] = pass;
		}

		auto collection = snew<EffectPassCollection>(list);
		return collection;
	}

	EffectParameter::EffectParameter() {
		impl = unew<PlatformImplementation>();
	}

	PEffectAnnotationCollection EffectParameter::Annotations() const {
		D3DX11_EFFECT_VARIABLE_DESC desc;
		const auto hr = impl->dxVariable->GetDesc(&desc);

		if FAILED(hr)
			throw std::runtime_error("EffectParameter::Annotations: error getting D3DX11_EFFECT_VARIABLE_DESC");

		auto annotCount = desc.Annotations;

		if (annotCount == 0)
			return snew<EffectAnnotationCollection>();

		std::vector<PEffectAnnotation> list(annotCount);

		for (size_t i = 0; i < annotCount; ++i) {
			auto current = impl->dxVariable->GetAnnotationByIndex(i);
			auto annotation = snew<EffectAnnotation>();
			annotation->impl->dxVariable = current;
			annotation->impl->dxVariable->AddRef();

			current->Release();
			current = nullptr;

			list[i] = annotation;
		}

		auto collection = snew<EffectAnnotationCollection>(list);
		return collection;
	}

	Int EffectParameter::ColumnCount() const {
		auto type = impl->dxVariable->GetType();
		D3DX11_EFFECT_TYPE_DESC desc{};
		const auto hr = type->GetDesc(&desc);

		type->Release();
		type = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectParameter::ColumnCount: error getting D3DX11_EFFECT_VARIABLE_DESC");

		return static_cast<Int>(desc.Columns);
	}

	Int EffectParameter::RowCount() const {
		auto type = impl->dxVariable->GetType();
		D3DX11_EFFECT_TYPE_DESC desc{};
		const auto hr = type->GetDesc(&desc);

		type->Release();
		type = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectParameter::RowCount: error getting D3DX11_EFFECT_TYPE_DESC");

		return static_cast<Int>(desc.Rows);
	}

	String EffectParameter::Semantic() const {
		auto type = impl->dxVariable->GetType();
		auto semantic = type->GetMemberSemantic(0);

		type->Release();
		type = nullptr;

		return std::string(semantic);
	}

	EffectParameterType EffectParameter::ParameterType() const {
		auto type = impl->dxVariable->GetType();
		D3DX11_EFFECT_TYPE_DESC desc{};
		const auto hr = type->GetDesc(&desc);

		type->Release();
		type = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectParameter::ParameterType: error getting D3DX11_EFFECT_TYPE_DESC");

		switch (desc.Type)
		{
		case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_BOOL:
			return EffectParameterType::Bool;
		case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_INT:
			return EffectParameterType::Int32;
		case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_FLOAT:
			return EffectParameterType::Single;
		case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_STRING:
			return EffectParameterType::String;
		case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_TEXTURE:
			return EffectParameterType::Texture;
		case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_TEXTURE1D:
			return EffectParameterType::Texture1D;
		case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_TEXTURE2D:
			return EffectParameterType::Texture2D;
		case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_TEXTURE3D:
			return EffectParameterType::Texture3D;
		case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_TEXTURECUBE:
			return EffectParameterType::TextureCube;
		case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_VOID:
			return EffectParameterType::Void;
		default:
			throw std::runtime_error("EffectParameter::ParameterType: invalid EffectParameterType.");
		}
	}

	EffectParameterClass EffectParameter::ParameterClass() const {
		auto type = impl->dxVariable->GetType();
		D3DX11_EFFECT_TYPE_DESC desc{};
		const auto hr = type->GetDesc(&desc);

		type->Release();
		type = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectParameter::ParameterClass: error getting D3DX11_EFFECT_TYPE_DESC");

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
			throw std::runtime_error("EffectParameter::ParameterClass: invalid EffectParameterClass.");
		}
	}

	String EffectParameter::Name() const {
		auto type = impl->dxVariable->GetType();
		D3DX11_EFFECT_TYPE_DESC desc{};
		const auto hr = type->GetDesc(&desc);

		type->Release();
		type = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectParameter::Name: error getting D3DX11_EFFECT_TYPE_DESC");

		return String(desc.TypeName);
	}

	sptr<EffectParameterCollection> EffectParameter::Elements() const {
		uint32_t index = 0;

		auto collection = snew<EffectParameterCollection>();

		while (true) {
			auto el = impl->dxVariable->GetElement(index);

			if (!el)
				break;

			auto efparam = snew<EffectParameter>();
			efparam->impl->dxVariable.Attach(el);

			el->Release();
			el = nullptr;
		}


		return collection;
	}

	sptr<EffectParameterCollection> EffectParameter::StructureMembers() const {
		uint32_t index = 0;

		auto collection = snew<EffectParameterCollection>();

		while (true) {
			auto member = impl->dxVariable->GetMemberByIndex(index);

			if (!member)
				break;

			auto efparam = snew<EffectParameter>();
			efparam->impl->dxVariable.Attach(member);

			member->Release();
			member = nullptr;
		}


		return collection;
	}

	bool EffectParameter::GetValueBoolean() const {
		auto scalar = impl->dxVariable->AsScalar();

		bool value;
		const auto hr = scalar->GetBool(&value);
		scalar->Release();
		scalar = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectParameter::GetValueBoolean: Unable to get boolean value.");

		return value;
	}

	std::vector<bool> EffectParameter::GetValueBooleanArray(size_t count) const {
		auto scalar = impl->dxVariable->AsScalar();
		auto arr = std::make_unique<bool[]>(count);

		const auto hr = scalar->GetBoolArray(arr.get(), 0, count);
		scalar->Release();
		scalar = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectParameter::GetValueBooleanArray: Unable to get boolean value.");

		std::vector<bool> data(count);

		for (size_t i = 0; i < count; ++i) {
			data[i] = arr[i];
		}

		return data;
	}

	Int EffectParameter::GetValueInt32() const {
		auto scalar = impl->dxVariable->AsScalar();

		Int value;
		const auto hr = scalar->GetInt(&value);
		scalar->Release();
		scalar = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectParameter::GetValueInt32: Unable to get int value.");

		return value;
	}

	std::vector<Int> EffectParameter::GetValueInt32Array(size_t count) const {
		auto scalar = impl->dxVariable->AsScalar();
		std::vector<Int> data(count);

		const auto hr = scalar->GetIntArray(data.data(), 0, count);
		scalar->Release();
		scalar = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectParameter::GetValueInt32Array: Unable to get int value.");

		return data;
	}

	Matrix EffectParameter::GetValueMatrix() const {
		auto matrix = impl->dxVariable->AsMatrix();
		float values[16];
		auto hr = matrix->GetMatrix(values);
		matrix->Release();
		matrix = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectParameter::GetValueIntMatrix: Unable to get matrix value.");

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

	std::vector<Matrix> EffectParameter::GetValueMatrixArray(size_t count) const {
		auto matrix = impl->dxVariable->AsMatrix();
		const auto elements = count * 16;
		auto arr = std::make_unique<float[]>(count * elements);

		auto hr = matrix->GetMatrixArray(arr.get(), 0, elements);
		matrix->Release();
		matrix = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectParameter::GetValuetMatrixArray: Unable to get matrix value.");

		auto index = 0;
		std::vector<Matrix> data(count);

		for (size_t i = 0; i < elements; ++i) {
			Matrix m;
			m.M11 = arr[i];
			m.M12 = arr[++i];
			m.M13 = arr[++i];
			m.M14 = arr[++i];
			m.M21 = arr[++i];
			m.M22 = arr[++i];
			m.M23 = arr[++i];
			m.M24 = arr[++i];
			m.M31 = arr[++i];
			m.M32 = arr[++i];
			m.M33 = arr[++i];
			m.M34 = arr[++i];
			m.M41 = arr[++i];
			m.M42 = arr[++i];
			m.M43 = arr[++i];
			m.M44 = arr[++i];

			data[index] = m;
			++index;
		}

		return data;
	}

	Matrix EffectParameter::GetValueMatrixTranspose() const {
		auto matrix = impl->dxVariable->AsMatrix();
		float values[16];
		auto hr = matrix->GetMatrixTranspose(values);
		matrix->Release();
		matrix = nullptr;

		if FAILED(hr)
			throw std::runtime_error("EffectParameter::GetValueIntMatrix: Unable to get matrix value.");

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
}