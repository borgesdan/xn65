#ifndef XNA_CONTENT_READERS_DEFAULT_HPP
#define XNA_CONTENT_READERS_DEFAULT_HPP

#include "../../common/color.hpp"
#include "../../common/numerics.hpp"
#include "../../default.hpp"
#include "../reader.hpp"
#include "csharp/time.hpp"

namespace xna {
	class ObjectReader : public ContentTypeReader {
	public:
		ObjectReader() : ContentTypeReader(std::make_shared<csharp::Type>(csharp::typeof<Object>())) {}

		virtual Object Read(ContentReader& input, Object& existingInstance) override {
			throw csharp::InvalidOperationException();

			return Object();
		}
	};

	class BooleanReader : public ContentTypeReaderT<bool> {
	public:
		BooleanReader() : ContentTypeReaderT(std::make_shared<csharp::Type>(csharp::typeof<bool>())) {}

		bool Read(ContentReader& input, bool& existingInstance) override {
			return input.ReadBoolean();
		}
	};

	class ByteReader : public ContentTypeReaderT<Byte> {
	public:
		ByteReader() : ContentTypeReaderT(std::make_shared<csharp::Type>(csharp::typeof<Byte>())) {}

		Byte Read(ContentReader& input, Byte& existingInstance) override {
			const auto b = input.ReadByte();
			return b;
		}
	};

	class CharReader : public ContentTypeReaderT<Char> {
	public:
		CharReader() : ContentTypeReaderT(std::make_shared<csharp::Type>(csharp::typeof<Char>())) {}

		Char Read(ContentReader& input, Char& existingInstance) override {
			const auto b = input.ReadChar();
			return b;
		}
	};

	class ColorReader : public ContentTypeReaderT<Color> {
	public:
		ColorReader() : ContentTypeReaderT(std::make_shared<csharp::Type>(csharp::typeof<Color>())) {}

		Color Read(ContentReader& input, Color& existingInstance) override {
			const auto i = input.ReadUInt32();
			return { i };
		}
	};

	class DoubleReader : public ContentTypeReaderT<double> {
	public:
		DoubleReader() : ContentTypeReaderT(std::make_shared<csharp::Type>(csharp::typeof<double>())) {}

		double Read(ContentReader& input, double& existingInstance) override {
			return input.ReadDouble();
		}
	};

	class Int16Reader : public ContentTypeReaderT<Short> {
	public:
		Int16Reader() : ContentTypeReaderT(std::make_shared<csharp::Type>(csharp::typeof<Short>())) {}

		Short Read(ContentReader& input, Short& existingInstance) override {
			return input.ReadInt16();
		}
	};

	class Int32Reader : public ContentTypeReaderT<Int> {
	public:
		Int32Reader() : ContentTypeReaderT(std::make_shared<csharp::Type>(csharp::typeof<Int>())) {}

		Int Read(ContentReader& input, Int& existingInstance) override {
			return input.ReadInt32();
		}
	};

	class Int64Reader : public ContentTypeReaderT<Long> {
	public:
		Int64Reader() : ContentTypeReaderT(std::make_shared<csharp::Type>(csharp::typeof<Long>())) {}

		Long Read(ContentReader& input, Long& existingInstance) override {
			return input.ReadInt64();
		}
	};

	class MatrixReader : public ContentTypeReaderT<Matrix> {
	public:
		MatrixReader() : ContentTypeReaderT(std::make_shared<csharp::Type>(csharp::typeof<Matrix>())) {}

		Matrix Read(ContentReader& input, Matrix& existingInstance) override {
			return input.ReadMatrix();
		}
	};

	class PointReader : public ContentTypeReaderT<Point> {
	public:
		PointReader() : ContentTypeReaderT(std::make_shared<csharp::Type>(csharp::typeof<Point>())) {}

		Point Read(ContentReader& input, Point& existingInstance) override {
			Point point;
			point.X = input.ReadInt32();
			point.Y = input.ReadInt32();
			return point;
		}
	};

	class QuaternionReader : public ContentTypeReaderT<Quaternion> {
	public:
		QuaternionReader() : ContentTypeReaderT(std::make_shared<csharp::Type>(csharp::typeof<Quaternion>())) {}

		Quaternion Read(ContentReader& input, Quaternion& existingInstance) override {
			return input.ReadQuaternion();
		}
	};

	class RectangleReader : public ContentTypeReaderT<Rectangle> {
	public:
		RectangleReader() : ContentTypeReaderT(std::make_shared<csharp::Type>(csharp::typeof<Rectangle>())) {}

		Rectangle Read(ContentReader& input, Rectangle& existingInstance) override {
			Rectangle rectangle;
			rectangle.X = input.ReadInt32();
			rectangle.Y = input.ReadInt32();
			rectangle.Width = input.ReadInt32();
			rectangle.Height = input.ReadInt32();
			return rectangle;
		}
	};

	class SByteReader : public ContentTypeReaderT<Sbyte> {
	public:
		SByteReader() : ContentTypeReaderT(std::make_shared<csharp::Type>(csharp::typeof<Sbyte>())) {}

		Sbyte Read(ContentReader& input, Sbyte& existingInstance) override {
			return input.ReadSByte();
		}
	};

	class SingleReader : public ContentTypeReaderT<float> {
	public:
		SingleReader() : ContentTypeReaderT(std::make_shared<csharp::Type>(csharp::typeof<float>())) {}

		float Read(ContentReader& input, float& existingInstance) override {
			return input.ReadSingle();
		}
	};

	class TimeSpanReader : public ContentTypeReaderT<csharp::TimeSpan> {
	public:
		TimeSpanReader() : ContentTypeReaderT(std::make_shared<csharp::Type>(csharp::typeof<csharp::TimeSpan>())) {}

		csharp::TimeSpan Read(ContentReader& input, csharp::TimeSpan& existingInstance) override {
			return csharp::TimeSpan::FromTicks(input.ReadInt64());
		}
	};

	class UInt16Reader : public ContentTypeReaderT<Ushort> {
	public:
		UInt16Reader() : ContentTypeReaderT(std::make_shared<csharp::Type>(csharp::typeof<Ushort>())) {}

		Ushort Read(ContentReader& input, Ushort& existingInstance) override {
			return input.ReadUInt16();
		}
	};

	class UInt32Reader : public ContentTypeReaderT<Uint> {
	public:
		UInt32Reader() : ContentTypeReaderT(std::make_shared<csharp::Type>(csharp::typeof<Uint>())) {}

		Uint Read(ContentReader& input, Uint& existingInstance) override {
			return input.ReadUInt32();
		}
	};

	class UInt64Reader : public ContentTypeReaderT<Ulong> {
	public:
		UInt64Reader() : ContentTypeReaderT(std::make_shared<csharp::Type>(csharp::typeof<Ulong>())) {}

		Ulong Read(ContentReader& input, Ulong& existingInstance) override {
			return input.ReadUInt64();
		}
	};

	class Vector2Reader : public ContentTypeReaderT<Vector2> {
	public:
		Vector2Reader() : ContentTypeReaderT(std::make_shared<csharp::Type>(csharp::typeof<Vector2>())) {}

		Vector2 Read(ContentReader& input, Vector2& existingInstance) override {
			return input.ReadVector2();
		}
	};

	class Vector3Reader : public ContentTypeReaderT<Vector3> {
	public:
		Vector3Reader() : ContentTypeReaderT(std::make_shared<csharp::Type>(csharp::typeof<Vector3>())) {}

		Vector3 Read(ContentReader& input, Vector3& existingInstance) override {
			return input.ReadVector3();
		}
	};

	class Vector4Reader : public ContentTypeReaderT<Vector4> {
	public:
		Vector4Reader() : ContentTypeReaderT(std::make_shared<csharp::Type>(csharp::typeof<Vector4>())) {}

		Vector4 Read(ContentReader& input, Vector4& existingInstance) override {
			return input.ReadVector4();
		}
	};	

	template <typename T>
	class ListReader : public ContentTypeReaderT<std::vector<T>> {
	public:
		ListReader(){}

		std::vector<T> Read(ContentReader& input, std::vector<T>& existingInstance) override {
			auto num = input.ReadInt32();

			auto& objList = existingInstance;

			while (num-- > 0) {
				auto obj = input.ReadObject<T>(*elementReader);
				objList.push_back(obj);
			}
			return objList;
		}		

		void Initialize(sptr<ContentTypeReaderManager> const& manager) override {
			auto type = std::make_shared<csharp::Type>(csharp::typeof<T>());
			elementReader = manager->GetTypeReader(type);
		}

	private:
		PContentTypeReader elementReader = nullptr;
	};
}

#endif