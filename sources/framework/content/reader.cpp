#include "xna/content/reader.hpp"
#include "xna/content/manager.hpp"
#include "xna/content/typereadermanager.hpp"

namespace xna {
	std::shared_ptr<ContentReader> ContentReader::Create(std::shared_ptr<xna::ContentManager> const& contentManager, std::shared_ptr<csharp::Stream>& input, String const& assetName)
	{
		Int graphicsProfile = 0;
		input = ContentReader::PrepareStream(input, assetName, graphicsProfile);
		return std::shared_ptr<ContentReader>(new ContentReader(contentManager, input, assetName, graphicsProfile));
	}

	std::shared_ptr<ContentManager> ContentReader::ContentManager() const {
		return _contentManager;
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

	std::vector<Byte> ContentReader::ReadByteBuffer(size_t size)
	{
		if (byteBuffer.empty() || byteBuffer.size() < size)
		{
			byteBuffer.resize(size);
		}
		
		Int num = 0;
		for (size_t index = 0; index < size; index += num)
		{			
			num = Read(byteBuffer.data(), byteBuffer.size(), index, size - index);
			if (num == 0) {
				throw std::runtime_error("ContentReader::ReadByteBuffer: Bad xbn.");
			}
		}

		return byteBuffer;
	}

	std::shared_ptr<csharp::Stream> ContentReader::PrepareStream(std::shared_ptr<csharp::Stream>& input, String const& assetName, Int& graphicsProfile)
	{
		BinaryReader binaryReader = BinaryReader(input);

		if (binaryReader.ReadByte() != 'X' || binaryReader.ReadByte() != 'N' || binaryReader.ReadByte() != 'B')
			throw std::runtime_error("ContentReader::PrepareStream: Bad xbn platform.");

		Int num1 = 0;
		auto wbyte = binaryReader.ReadByte();

		if(wbyte != 'w')
			throw std::runtime_error("ContentReader::PrepareStream: Bad xbn file.");

		num1 = binaryReader.ReadUInt16();			

		graphicsProfile = (num1 & XnbVersionProfileMask) >> XnbVersionProfileShift;
		bool flag = false;

		switch (num1 & -32513)
		{
		case XnbVersion:
			flag = false;
			break;
		case XnbCompressedVersion:
			flag = true;
			break;
		default:
			throw std::runtime_error("ContentReader::PrepareStream: Bad xbn version.");
		}

		const auto num2 = binaryReader.ReadInt32();

		if ((static_cast<Long>(num2) - 10) > input->Length() - input->Position())
			throw std::runtime_error("ContentReader::PrepareStream: Bad xbn size.");

		if (!flag)
			return input;

		const Int compressedTodo = num2 - 14;
		const auto decompressedTodo = binaryReader.ReadInt32();

		throw std::runtime_error("ContentReader::PrepareStream: LzxDecoder not implemented.");
	}

	Int ContentReader::ReadHeader() {
		auto _this = shared_from_this();
		typeReaders = ContentTypeReaderManager::ReadTypeManifest(this->Read7BitEncodedInt(), _this);
		auto length = this->Read7BitEncodedInt();		

		return length;

	}
}
