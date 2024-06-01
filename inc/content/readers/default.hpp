#ifndef XNA_CONTENT_READERS_DEFAULT_HPP
#define XNA_CONTENT_READERS_DEFAULT_HPP

#include "content/reader.hpp"
#include "default.hpp"
#include "common/color.hpp"
#include "common/numerics.hpp"
#include "csharp/timespan.hpp"

namespace xna {
	class ObjectReader : public ContentTypeReaderT<Object> {
	public:
		ObjectReader() : ContentTypeReaderT(typeof<Object>()) {}

		virtual Object Read(ContentReader& input, Object& existingInstance) override {
			return Object();
		}
	};

	class BooleanReader : public ContentTypeReaderT<bool> {
	public:
		BooleanReader() : ContentTypeReaderT(typeof<bool>()) {}

		bool Read(ContentReader& input, bool& existingInstance) override {
			return input.ReadBoolean();
		}
	};

	class ByteReader : public ContentTypeReaderT<Byte> {
	public:
		ByteReader() : ContentTypeReaderT(typeof<Byte>()) {}

		Byte Read(ContentReader& input, Byte& existingInstance) override {
			const auto b = input.ReadByte();
			return b;
		}
	};

	class CharReader : public ContentTypeReaderT<Char> {
	public:
		CharReader() : ContentTypeReaderT(typeof<Char>()) {}

		Char Read(ContentReader& input, Char& existingInstance) override {
			const auto b = input.ReadChar();
			return b;
		}
	};

	class ColorReader : public ContentTypeReaderT<Color> {
	public:
		ColorReader() : ContentTypeReaderT(typeof<Color>()) {}

		Color Read(ContentReader& input, Color& existingInstance) override {
			const auto i = input.ReadUInt32();
			return { i };
		}
	};

	class DoubleReader : public ContentTypeReaderT<double> {
	public:
		DoubleReader() : ContentTypeReaderT(typeof<double>()) {}

		double Read(ContentReader& input, double& existingInstance) override {
			return input.ReadDouble();
		}
	};

	class Int16Reader : public ContentTypeReaderT<Short> {
	public:
		Int16Reader() : ContentTypeReaderT(typeof<Short>()) {}

		Short Read(ContentReader& input, Short& existingInstance) override {
			return input.ReadInt16();
		}
	};

	class Int32Reader : public ContentTypeReaderT<Int> {
	public:
		Int32Reader() : ContentTypeReaderT(typeof<Int>()) {}

		Int Read(ContentReader& input, Int& existingInstance) override {
			return input.ReadInt32();
		}
	};

	class Int64Reader : public ContentTypeReaderT<Long> {
	public:
		Int64Reader() : ContentTypeReaderT(typeof<Long>()) {}

		Long Read(ContentReader& input, Long& existingInstance) override {
			return input.ReadInt64();
		}
	};

	class MatrixReader : public ContentTypeReaderT<Matrix> {
	public:
		MatrixReader() : ContentTypeReaderT(typeof<Matrix>()) {}

		Matrix Read(ContentReader& input, Matrix& existingInstance) override {
			return input.ReadMatrix();
		}
	};

	class PointReader : public ContentTypeReaderT<Point> {
	public:
		PointReader() : ContentTypeReaderT(typeof<Point>()) {}

		Point Read(ContentReader& input, Point& existingInstance) override {
			Point point;
			point.X = input.ReadInt32();
			point.Y = input.ReadInt32();
			return point;
		}
	};

	class QuaternionReader : public ContentTypeReaderT<Quaternion> {
	public:
		QuaternionReader() : ContentTypeReaderT(typeof<Quaternion>()) {}

		Quaternion Read(ContentReader& input, Quaternion& existingInstance) override {
			return input.ReadQuaternion();
		}
	};

	class RectangleReader : public ContentTypeReaderT<Rectangle> {
	public:
		RectangleReader() : ContentTypeReaderT(typeof<Rectangle>()) {}

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
		SByteReader() : ContentTypeReaderT(typeof<Sbyte>()) {}

		Sbyte Read(ContentReader& input, Sbyte& existingInstance) override {
			return input.ReadSByte();
		}
	};

	class SingleReader : public ContentTypeReaderT<float> {
	public:
		SingleReader() : ContentTypeReaderT(typeof<float>()) {}

		float Read(ContentReader& input, float& existingInstance) override {
			return input.ReadSingle();
		}
	};

	class TimeSpanReader : public ContentTypeReaderT<TimeSpan> {
	public:
		TimeSpanReader() : ContentTypeReaderT(typeof<TimeSpan>()) {}

		TimeSpan Read(ContentReader& input, TimeSpan& existingInstance) override {
			return TimeSpan::FromTicks(input.ReadInt64());
		}
	};

	class UInt16Reader : public ContentTypeReaderT<Ushort> {
	public:
		UInt16Reader() : ContentTypeReaderT(typeof<Ushort>()) {}

		Ushort Read(ContentReader& input, Ushort& existingInstance) override {
			return input.ReadUInt16();
		}
	};

	class UInt32Reader : public ContentTypeReaderT<Uint> {
	public:
		UInt32Reader() : ContentTypeReaderT(typeof<Uint>()) {}

		Uint Read(ContentReader& input, Uint& existingInstance) override {
			return input.ReadUInt32();
		}
	};

	class UInt64Reader : public ContentTypeReaderT<Ulong> {
	public:
		UInt64Reader() : ContentTypeReaderT(typeof<Ulong>()) {}

		Ulong Read(ContentReader& input, Ulong& existingInstance) override {
			return input.ReadUInt64();
		}
	};

	class Vector2Reader : public ContentTypeReaderT<Vector2> {
	public:
		Vector2Reader() : ContentTypeReaderT(typeof<Vector2>()) {}

		Vector2 Read(ContentReader& input, Vector2& existingInstance) override {
			return input.ReadVector2();
		}
	};

	class Vector3Reader : public ContentTypeReaderT<Vector3> {
	public:
		Vector3Reader() : ContentTypeReaderT(typeof<Vector3>()) {}

		Vector3 Read(ContentReader& input, Vector3& existingInstance) override {
			return input.ReadVector3();
		}
	};

	class Vector4Reader : public ContentTypeReaderT<Vector4> {
	public:
		Vector4Reader() : ContentTypeReaderT(typeof<Vector4>()) {}

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
			elementReader = manager->GetTypeReader(typeof<T>());
		}

	private:
		PContentTypeReader elementReader = nullptr;
	};
}

#endif