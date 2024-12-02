#ifndef XNA_CONTENT_READER_HPP
#define XNA_CONTENT_READER_HPP

#include "../common/color.hpp"
#include "../common/numerics.hpp"
#include "../csharp/binary.hpp"
#include "../csharp/type.hpp"
#include "../default.hpp"
#include "typereadermanager.hpp"
#include <any>

namespace xna {
	//A worker object that implements most of ContentManager.Load.
	class ContentReader : public BinaryReader, public std::enable_shared_from_this<ContentReader> {
	public:
		static sptr<ContentReader> Create(sptr<ContentManager> const& contentManager, sptr<Stream>& input, String const& assetName);		

		// Reads a single object from the current stream.
		template <typename T>
		auto ReadObject();

		// Reads a single object from the current stream.
		template <typename T>
		auto ReadObject(T& existingInstance);

		// Reads a single object from the current stream.
		template <typename T>
		auto ReadObject(ContentTypeReader& typeReader);

		// Reads a single object from the current stream.
		template <typename T>
		auto ReadObject(ContentTypeReader& typeReader, T& existingInstance);

		//Reads a Vector2 value from the current stream. 
		Vector2 ReadVector2();
		//Reads a Vector3 value from the current stream. 
		Vector3 ReadVector3();
		//Reads a Vector4 value from the current stream. 
		Vector4 ReadVector4();
		//Reads a Matrix value from the currently open stream.
		Matrix ReadMatrix();
		//Reads a Quaternion value from the current stream. 
		Quaternion ReadQuaternion();
		//Reads a Color value from the currently open stream.
		Color ReadColor();
		//Reads a float value from the currently open stream.
		float ReadSingle() override;
		//Reads a double value from the currently open stream.
		double ReadDouble() override;

		//Gets the name of the asset currently being read by this ContentReader.
		constexpr String AssetName() const {
			return _assetName;
		}

		//Gets the ContentManager associated with the ContentReader.
		sptr<xna::ContentManager> ContentManager() const;

		//
		// Internal methods
		//

		template <typename T>
		auto ReadAsset();

		std::vector<Byte> ReadByteBuffer(size_t size);		

	private:
		ContentReader(sptr<xna::ContentManager> const& contentManager, sptr<Stream>& input, String const& assetName, Int graphicsProfile)
			: BinaryReader(input), _contentManager(contentManager), _assetName(assetName) {}

		static sptr<Stream> PrepareStream(sptr<Stream>& input, String const& assetName, Int& graphicsProfile);

		Int ReadHeader();

		template <typename T>
		auto ReadObjectInternal(std::any& existingInstance);
		
		template <typename T>
		auto ReadObjectInternal(ContentTypeReader& typeReader, Object& existingInstance);

		template <typename T>
		auto InvokeReader(ContentTypeReader& reader, Object& existingInstance);

	private:
		sptr<xna::ContentManager> _contentManager = nullptr;
		String _assetName;
		std::vector<sptr<ContentTypeReader>> typeReaders;
		Int graphicsProfile{ 0 };
		std::vector<Byte> byteBuffer;

		static constexpr Ushort XnbVersionProfileMask = 32512;
		static constexpr Ushort XnbCompressedVersion = 32773;
		static constexpr Ushort XnbVersion = 5;
		static constexpr Int XnbVersionProfileShift = 8;
		static constexpr Char PlatformLabel = 'w';
		static constexpr Int XnbPrologueSize = 10;
		static constexpr Int XnbCompressedPrologueSize = 14;
	};

	template<typename T>
	inline auto ContentReader::ReadObjectInternal(Object& existingInstance)
	{
		const auto num = Read7BitEncodedInt();

		if (num == 0) {
			misc::ReturnDefaultOrNull<T>();
		}

		const auto index = num - 1;

		if (index >= typeReaders.size()) {
			throw csharp::InvalidOperationException("Bad Xnb");
		}		
		
		auto& reader = typeReaders[index];		
		
		return InvokeReader<T>(*reader, existingInstance);
	}

	template<typename T>
	inline auto ContentReader::InvokeReader(ContentTypeReader& reader, Object& existingInstance)
	{
		auto contentTypeReader = reinterpret_cast<ContentTypeReaderT<T>*>(&reader);
		T objB;

		if (contentTypeReader) {
			auto existingInstance1 = existingInstance.has_value() ? std::any_cast<T>(existingInstance) : T();
			objB = contentTypeReader->Read(*this, existingInstance1);
			return objB;
		}
		else {
			throw csharp::InvalidOperationException();
		}

		return misc::ReturnDefaultOrNull<T>();
	}

	template<typename T>
	inline auto ContentReader::ReadAsset()
	{
		const auto sharedResourceCount = ReadHeader();
		auto obj = ReadObject<T>();
		return obj;
	}

	template<typename T>
	inline auto ContentReader::ReadObject()
	{
		auto a = std::any();
		return ReadObjectInternal<T>(a);
	}

	template<typename T>
	inline auto ContentReader::ReadObject(T& existingInstance)
	{
		return ReadObjectInternal<T>(Object(existingInstance));
	}

	template<typename T>
	inline auto ContentReader::ReadObject(ContentTypeReader& typeReader)
	{
		auto obj = std::any();
		return ReadObjectInternal<T>(typeReader, obj);
	}

	template<typename T>
	inline auto ContentReader::ReadObject(ContentTypeReader& typeReader, T& existingInstance)
	{
		return ReadObjectInternal<T>(typeReader, std::any(existingInstance));
	}

	template<typename T>
	inline auto ContentReader::ReadObjectInternal(ContentTypeReader& typeReader, Object& existingInstance)
	{
		if (typeReader.TargetIsValueType)
			return InvokeReader<T>(typeReader, existingInstance);

		return ReadObjectInternal<T>(existingInstance);
	}	
}

#endif