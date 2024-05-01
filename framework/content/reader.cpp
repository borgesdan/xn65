#include "reader.hpp"
#include "manager.hpp"
#include "lzx/decoderstream.hpp"

namespace xna {
	sptr<ContentReader> ContentReader::Create(ContentManager* contentManager, Stream* input, String const& assetName)
	{
		return sptr<ContentReader>();
	}

	sptr<Stream> ContentReader::PrepareStream(sptr<Stream>& input, String const* assetName, Int& graphicsProfile)
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
		return Int();
	}
}
