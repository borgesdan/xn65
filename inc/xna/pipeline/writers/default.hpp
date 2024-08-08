#ifndef XNA_PIPELINE_WRITERS_DEFAULT_HPP
#define XNA_PIPELINE_WRITERS_DEFAULT_HPP

#include "../writer.hpp"
#include "../../common/collision.hpp"
#include "../../common/curve.hpp"

namespace xna {
	class BooleanWriter : public BuiltinTypeWriter<bool> {
	public:
		void Write(ContentWriter& output, bool& value) override {
			output.Write(value);
		}
	};

	class BoundingBoxWriter : public BuiltinTypeWriter<BoundingBox> {
	public:
		void Write(ContentWriter& output, BoundingBox& value) override {
			output.Write(value.Min);
			output.Write(value.Max);
		}
	};

	class BoundingFrustumWriter : public BuiltinTypeWriter<BoundingFrustum> {
	public:
		void Write(ContentWriter& output, BoundingFrustum& value) override {
			output.Write(value.GetMatrix());
		}
	};

	class BoundingSphereWriter : public BuiltinTypeWriter<BoundingSphere> {
	public:
		void Write(ContentWriter& output, BoundingSphere& value) override {
			output.Write(value.Center);
			output.Write(value.Radius);
		}
	};

	class ByteWriter : public BuiltinTypeWriter<Byte> {
	public:
		void Write(ContentWriter& output, Byte& value) override {
			output.Write(value);
		}
	};

	class CharWriter : public BuiltinTypeWriter<Char> {
	public:
		void Write(ContentWriter& output, Char& value) override {
			output.Write(value);
		}
	};

	class ColorWriter : public BuiltinTypeWriter<Color> {
	public:
		void Write(ContentWriter& output, Color& value) override {
			output.Write(value);
		}
	};

	class CurveWriter : public BuiltinTypeWriter<Curve> {
	public:
		void Write(ContentWriter& output, Curve& value) override {
			output.Write(static_cast<Int>(value.PreLoop));
			output.Write(static_cast<Int>(value.PostLoop));
			output.Write(static_cast<Int>(value.Keys.Count()));
			
			for (size_t i = 0; i < value.Keys.Count(); ++i) {
				const auto& key = value.Keys[i];

				output.Write(key.Position);
				output.Write(key.Value);
				output.Write(key.TangentIn);
				output.Write(key.TangentOut);
				output.Write(static_cast<Int>(key.Continuity));
			}
		}

		bool CanDeserializeIntoExistingObject() const override {
			return true;
		}

	};

	class DoubleWriter : public BuiltinTypeWriter<double> {
	public:
		void Write(ContentWriter& output, double& value) override {
			output.Write(value);
		}
	};
	
	class Int16Writer : public BuiltinTypeWriter<Short> {
	public:
		void Write(ContentWriter& output, Short& value) override {
			output.Write(value);
		}
	};
	
	class Int32Writer : public BuiltinTypeWriter<Int> {
	public:
		void Write(ContentWriter& output, Int& value) override {
			output.Write(value);
		}
	};
	
	class Int64Writer : public BuiltinTypeWriter<Long> {
	public:
		void Write(ContentWriter& output, Long& value) override {
			output.Write(value);
		}
	};
	
	class MatrixWriter : public BuiltinTypeWriter<Matrix> {
	public:
		void Write(ContentWriter& output, Matrix& value) override {
			output.Write(value);
		}
	};
	
	class PlaneWriter : public BuiltinTypeWriter<Plane> {
	public:
		void Write(ContentWriter& output, Plane& value) override {
			output.Write(value.Normal);
			output.Write(value.D);
		}
	};
	
	class PointWriter : public BuiltinTypeWriter<Point> {
	public:
		void Write(ContentWriter& output, Point& value) override {
			output.Write(value.X);
			output.Write(value.Y);
		}
	};
	
	class QuaternionWriter : public BuiltinTypeWriter<Quaternion> {
	public:
		void Write(ContentWriter& output, Quaternion& value) override {
			output.Write(value);
		}
	};
	
	class RayWriter : public BuiltinTypeWriter<Ray> {
	public:
		void Write(ContentWriter& output, Ray& value) override {
			output.Write(value.Position);
			output.Write(value.Direction);
		}
	};
	
	class RectangleWriter : public BuiltinTypeWriter<Rectangle> {
	public:
		void Write(ContentWriter& output, Rectangle& value) override {
			output.Write(value.X);
			output.Write(value.Y);
			output.Write(value.Width);
			output.Write(value.Height);
		}
	};
	
	class SByteWriter : public BuiltinTypeWriter<Sbyte> {
	public:
		void Write(ContentWriter& output, Sbyte& value) override {
			output.Write(value);
		}
	};
	
	class SingleWriter : public BuiltinTypeWriter<float> {
	public:
		void Write(ContentWriter& output, float& value) override {
			output.Write(value);
		}
	};
	
	class StringWriter : public BuiltinTypeWriter<String> {
	public:
		void Write(ContentWriter& output, String& value) override {
			output.Write(value);
		}
	};
	
	class UInt16Writer : public BuiltinTypeWriter<Ushort> {
	public:
		void Write(ContentWriter& output, Ushort& value) override {
			output.Write(value);
		}
	};
	
	class UInt32Writer : public BuiltinTypeWriter<Uint> {
	public:
		void Write(ContentWriter& output, Uint& value) override {
			output.Write(value);
		}
	};
	
	class UInt64Writer : public BuiltinTypeWriter<Ulong> {
	public:
		void Write(ContentWriter& output, Ulong& value) override {
			output.Write(value);
		}
	};
	
	class Vector2Writer : public BuiltinTypeWriter<Vector2> {
	public:
		void Write(ContentWriter& output, Vector2& value) override {
			output.Write(value);
		}
	};
	
	class Vector3Writer : public BuiltinTypeWriter<Vector3> {
	public:
		void Write(ContentWriter& output, Vector3& value) override {
			output.Write(value);
		}
	};
	
	class Vector4Writer : public BuiltinTypeWriter<Vector4> {
	public:
		void Write(ContentWriter& output, Vector4& value) override {
			output.Write(value);
		}
	};
	
}

#endif