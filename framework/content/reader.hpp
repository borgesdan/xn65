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
	class ContentReader : public BinaryReader, public std::enable_shared_from_this<ContentReader>{
	public:
		static sptr<ContentReader> Create(ContentManager* contentManager, sptr<Stream>& input, String const& assetName);
		
		template <typename T>
		sptr<T> ReadAsset();
		
		template <typename T>
		T ReadObject();

		template <typename T>
		T ReadObject(T existingInstance);

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
			: BinaryReader(input), _contentManager(contentManager), _assetName(assetName){}

		static sptr<Stream> PrepareStream(sptr<Stream>& input, String const& assetName, Int& graphicsProfile);

		Int ReadHeader();

		template <typename T>
		T ReadObjectInternal(std::any& existingInstance, xna_error_nullarg);
		
		template <typename T>
		T InvokeReader(ContentTypeReader& reader, std::any& existingInstance, xna_error_nullarg);

	private:
		ContentManager* _contentManager = nullptr;
		String _assetName;
		std::vector<sptr<ContentTypeReader>> typeReaders;
		Int graphicsProfile{ 0 };
	};

	template<typename T>
	inline T ContentReader::ReadObjectInternal(std::any& existingInstance, xna_error_ptr_arg)
	{
		const auto num = Read7BitEncodedInt();

		if (num == 0) {
			return T();
		}
			
		const auto index = num - 1;

		if (index >= typeReaders.size()) {
			xna_error_apply(err, XnaErrorCode::ARGUMENT_OUT_OF_RANGE);
			return T();
		}

		return InvokeReader(typeReaders[index], existingInstance);
	}
	
	template<typename T>
	inline T ContentReader::InvokeReader(ContentTypeReader& reader, std::any& existingInstance, xna_error_ptr_arg)
	{
		auto contentTypeReader = reinterpret_cast<ContentTypeReaderT<T>*>(&reader);
		T objB = T();

		if (contentTypeReader) {
			T existingInstance1 = existingInstance.has_value() ? std::any_cast<T>(existingInstance) : T();
			objB = contentTypeReader->Read(*this, existingInstance1);
		}

		return T();
	}
	
	template<typename T>	
	inline sptr<T> ContentReader::ReadAsset()
	{
		const auto sharedResourceCount = ReadHeader();
		T obj = ReadObject<T>();
		//this.ReadSharedResources(sharedResourceCount);
		return obj;
	}

	template<typename T>
	inline T ContentReader::ReadObject()
	{
		return ReadObjectInternal<T>(nullptr);
	}

	template<typename T>
	inline T ContentReader::ReadObject(T existingInstance)
	{
		return ReadObjectInternal<T>(std::any(existingInstance));
	}	
}

#endif