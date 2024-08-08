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
}

#endif