#include "reader.hpp"
#include "manager.hpp"
#include "lzx/decoderstream.hpp"
#include "typereadermanager.hpp"

namespace xna {
	sptr<ContentReader> ContentReader::Create(ContentManager* contentManager, sptr<Stream>& input, String const& assetName)
	{
		Int graphicsProfile = 0;
		input = ContentReader::PrepareStream(input, assetName, graphicsProfile);
		return std::shared_ptr<ContentReader>(new ContentReader(contentManager, input, assetName, graphicsProfile));
	}

	Vector2 ContentReader::ReadVector2()
	{
		Vector2 vector2;
		vector2.X = ReadSingle();
		vector2.Y = ReadSingle();
		return vector2;
	}

	Vector3 ContentReader::ReadVector3()
	{
		Vector3 vector3;
		vector3.X = ReadSingle();
		vector3.Y = ReadSingle();
		vector3.Z = ReadSingle();
		return vector3;
	}

	Vector4 ContentReader::ReadVector4()
	{
		Vector4 vector4;
		vector4.X = ReadSingle();
		vector4.Y = ReadSingle();
		vector4.Z = ReadSingle();
		vector4.W = ReadSingle();
		return vector4;
	}

	Matrix ContentReader::ReadMatrix()
	{
		Matrix matrix;
		matrix.M11 = ReadSingle();
		matrix.M12 = ReadSingle();
		matrix.M13 = ReadSingle();
		matrix.M14 = ReadSingle();
		matrix.M21 = ReadSingle();
		matrix.M22 = ReadSingle();
		matrix.M23 = ReadSingle();
		matrix.M24 = ReadSingle();
		matrix.M31 = ReadSingle();
		matrix.M32 = ReadSingle();
		matrix.M33 = ReadSingle();
		matrix.M34 = ReadSingle();
		matrix.M41 = ReadSingle();
		matrix.M42 = ReadSingle();
		matrix.M43 = ReadSingle();
		matrix.M44 = ReadSingle();
		return matrix;
	}

	Quaternion ContentReader::ReadQuaternion()
	{
		Quaternion quaternion;
		quaternion.X = ReadSingle();
		quaternion.Y = ReadSingle();
		quaternion.Z = ReadSingle();
		quaternion.W = ReadSingle();
		return quaternion;
	}

	Color ContentReader::ReadColor()
	{
		const auto packedValue = ReadUInt32();
		return Color(packedValue);
	}

	float ContentReader::ReadSingle()
	{
		const auto int32 = ReadUInt32();
		return *(float*)&int32;
	}

	double ContentReader::ReadDouble()
	{
		const auto int64 = ReadUInt64();
		return *(double*)&int64;
	}

	sptr<Stream> ContentReader::PrepareStream(sptr<Stream>& input, String const& assetName, Int& graphicsProfile)
	{
		BinaryReader binaryReader = BinaryReader(input);

		if (binaryReader.ReadByte() != 'X' || binaryReader.ReadByte() != 'N' || binaryReader.ReadByte() != 'B')
			return nullptr;

		Int num1 = 0;
		if (binaryReader.ReadByte() == 'w')
			num1 = binaryReader.ReadUInt16();
		else
			return nullptr;

		graphicsProfile = (num1 & 32512) >> 8;
		bool flag = false;

		switch (num1 & -32513)
		{
		case 5:
			flag = false;
			break;
		case 32773:
			flag = true;
			break;
		default:
			return nullptr;
		}

		const auto num2 = binaryReader.ReadInt32();

		if ((num2 - 10) > input->Length() - input->Position())
			return nullptr;

		if (!flag)
			return input;

		const Int compressedTodo = num2 - 14;
		const auto decompressedTodo = binaryReader.ReadInt32();

		auto lzxStream = New<LzxDecoderStream>(input, compressedTodo, decompressedTodo);

		return reinterpret_pointer_cast<Stream>(lzxStream);
	}

	Int ContentReader::ReadHeader() {
		auto _this = shared_from_this();
		typeReaders = ContentTypeReaderManager::ReadTypeManifest(this->Read7BitEncodedInt(), _this);
		auto length = this->Read7BitEncodedInt();

		if (length > 0)
		{
			//TODO: length > 0
		}

		return length;

	}
}
