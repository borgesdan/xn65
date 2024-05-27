#include "platform-dx/init.hpp"
#include "csharp/type.hpp"
#include "graphics/readers/texture2D.hpp"
#include "content/typereadermanager.hpp"
#include "content/defaultreaders.hpp"
#include "platform-dx/implementations.hpp"

namespace xna {
	void Platform::Init() {
		PlatformInit::Init();
	}

	void PlatformInit::InitRegisteredTypes()
	{
		insertRegisteredReader<ObjectReader>("ObjecReader");
		insertRegisteredReader<BooleanReader>("BooleanReader");
		insertRegisteredReader<ByteReader>("ByteReader");
		insertRegisteredReader<CharReader>("CharReader");
		insertRegisteredReader<ColorReader>("ColorReader");
		insertRegisteredReader<DoubleReader>("DoubleReader");
		insertRegisteredReader<Int16Reader>("Int16Reader");
		insertRegisteredReader<Int32Reader>("Int32Reader");
		insertRegisteredReader<Int64Reader>("Int64Reader");
		insertRegisteredReader<MatrixReader>("MatrixReader");
		insertRegisteredReader<PointReader>("PointReader");
		insertRegisteredReader<QuaternionReader>("QuaternionReader");
		insertRegisteredReader<RectangleReader>("RectangleReader");
		insertRegisteredReader<SByteReader>("SByteReader");
		insertRegisteredReader<SingleReader>("SingleReader");
		insertRegisteredReader<TimeSpanReader>("TimeSpanReader");
		insertRegisteredReader<UInt16Reader>("UInt16Reader");
		insertRegisteredReader<UInt32Reader>("UInt32Reader");
		insertRegisteredReader<UInt64Reader>("UInt64Reader");
		insertRegisteredReader<Vector2Reader>("Vector2Reader");
		insertRegisteredReader<Vector3Reader>("Vector3Reader");
		insertRegisteredReader<Vector4Reader>("Vector4Reader");
		insertRegisteredReader<Texture2DReader>("Texture2DReader");
	}

	void PlatformInit::InitActivadors()
	{
		insertActivadorReader<ObjectReader>();		
		insertActivadorReader<BooleanReader>();
		insertActivadorReader<ByteReader>();
		insertActivadorReader<CharReader>();
		insertActivadorReader<ColorReader>();
		insertActivadorReader<DoubleReader>();
		insertActivadorReader<Int16Reader>();
		insertActivadorReader<Int32Reader>();
		insertActivadorReader<Int64Reader>();
		insertActivadorReader<MatrixReader>();
		insertActivadorReader<PointReader>();
		insertActivadorReader<QuaternionReader>();
		insertActivadorReader<RectangleReader>();
		insertActivadorReader<SByteReader>();
		insertActivadorReader<SingleReader>();
		insertActivadorReader<TimeSpanReader>();
		insertActivadorReader<UInt16Reader>();
		insertActivadorReader<UInt32Reader>();
		insertActivadorReader<UInt64Reader>();
		insertActivadorReader<Vector2Reader>();
		insertActivadorReader<Vector3Reader>();
		insertActivadorReader<Vector4Reader>();
		insertActivadorReader<Texture2DReader>();
	}
}