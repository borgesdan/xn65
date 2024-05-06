#ifndef XNA_CONTENT_READER_HPP
#define XNA_CONTENT_READER_HPP

#include "../common/color.hpp"
#include "../common/matrix.hpp"
#include "../common/quaternion.hpp"
#include "../common/vectors.hpp"
#include "../csharp/binary.hpp"
#include "../csharp/type.hpp"
#include "../default.hpp"
#include "typereadermanager.hpp"
#include <any>

namespace xna {
	class ContentReader : public BinaryReader, public std::enable_shared_from_this<ContentReader> {
	public:
		static sptr<ContentReader> Create(ContentManager* contentManager, sptr<Stream>& input, String const& assetName);

		template <typename T>
		sptr<T> ReadAsset();

		template <typename T>
		sptr<T> ReadObject();

		template <typename T>
		sptr<T> ReadObject(T existingInstance);

		Vector2 ReadVector2();
		Vector3 ReadVector3();
		Vector4 ReadVector4();
		Matrix ReadMatrix();
		Quaternion ReadQuaternion();
		Color ReadColor();
		float ReadSingle();
		double ReadDouble();

		std::vector<Byte> ReadByteBuffer(size_t size, xna_error_nullarg);

	private:
		ContentReader(ContentManager* contentManager, sptr<Stream>& input, String const& assetName, Int graphicsProfile)
			: BinaryReader(input), _contentManager(contentManager), _assetName(assetName) {}

		static sptr<Stream> PrepareStream(sptr<Stream>& input, String const& assetName, Int& graphicsProfile);

		Int ReadHeader();

		template <typename T>
		sptr<T> ReadObjectInternal(std::any& existingInstance, xna_error_nullarg);

		template <typename T>
		sptr<T> InvokeReader(ContentTypeReader& reader, std::any& existingInstance, xna_error_nullarg);

	private:
		ContentManager* _contentManager = nullptr;
		String _assetName;
		std::vector<sptr<ContentTypeReader>> typeReaders;
		Int graphicsProfile{ 0 };

		static constexpr Ushort XnbVersionProfileMask = 32512;
		static constexpr Ushort XnbCompressedVersion = 32773;
		static constexpr Ushort XnbVersion = 5;
		static constexpr Int XnbVersionProfileShift = 8;
	};

	template<typename T>
	inline sptr<T> ContentReader::ReadObjectInternal(std::any& existingInstance, xna_error_ptr_arg)
	{
		const auto num = Read7BitEncodedInt();

		if (num == 0) {
			return New<T>();
		}

		const auto index = num - 1;

		if (index >= typeReaders.size()) {
			xna_error_apply(err, XnaErrorCode::ARGUMENT_OUT_OF_RANGE);
			return New<T>();
		}

		auto reader = typeReaders[index];
		return InvokeReader<T>(*reader, existingInstance, err);
	}

	template<typename T>
	inline sptr<T> ContentReader::InvokeReader(ContentTypeReader& reader, std::any& existingInstance, xna_error_ptr_arg)
	{
		auto contentTypeReader = reinterpret_cast<ContentTypeReaderT<T>*>(&reader);
		sptr<T> objB = nullptr;

		if (contentTypeReader) {
			auto existingInstance1 = existingInstance.has_value() ? std::any_cast<sptr<T>>(existingInstance) : nullptr;
			objB = contentTypeReader->Read(*this, *existingInstance1);
			return objB;
		}

		return New<T>();
	}

	template<typename T>
	inline sptr<T> ContentReader::ReadAsset()
	{
		const auto sharedResourceCount = ReadHeader();
		auto obj = ReadObject<T>();
		//this.ReadSharedResources(sharedResourceCount);
		return obj;
	}

	template<typename T>
	inline sptr<T> ContentReader::ReadObject()
	{
		auto a = std::any();
		return ReadObjectInternal<T>(a);
	}

	template<typename T>
	inline sptr<T> ContentReader::ReadObject(T existingInstance)
	{
		return ReadObjectInternal<T>(std::any(existingInstance));
	}
}

#endif