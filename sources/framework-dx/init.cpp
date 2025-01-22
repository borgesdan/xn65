#include "csharp/type.hpp"
#include "xna/content/readers/graphics.hpp"
#include "xna/content/readers/audio.hpp"
#include "xna/content/typereadermanager.hpp"
#include "xna/content/readers/default.hpp"
#include "xna-dx/framework.hpp"

namespace xna {
	void Platform::Init() {		
		Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
		if (FAILED(initialize))
		{
			MessageBox(nullptr, "Ocorreu um erro ao chamar Microsoft::WRL::Wrappers::RoInitializeWrapper.", "XN65", MB_OK);
		}
		
		HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
		if (FAILED(hr))
		{
			MessageBox(nullptr, "Ocorreu um erro ao chamar CoInitializeEx.", "XN65", MB_OK);
		}

		PlatformInit::Init();
	}

	void PlatformInit::InitRegisteredTypes()
	{
		insertRegisteredReader<ObjectReader>("Microsoft.Xna.Framework.Content.ObjectReader");
		insertRegisteredReader<BooleanReader>("Microsoft.Xna.Framework.Content.BooleanReader");
		insertRegisteredReader<ByteReader>("Microsoft.Xna.Framework.Content.ByteReader");
		insertRegisteredReader<CharReader>("Microsoft.Xna.Framework.Content.CharReader");
		insertRegisteredReader<ColorReader>("Microsoft.Xna.Framework.Content.ColorReader");
		insertRegisteredReader<DoubleReader>("Microsoft.Xna.Framework.Content.DoubleReader");
		insertRegisteredReader<Int16Reader>("Microsoft.Xna.Framework.Content.Int16Reader");
		insertRegisteredReader<Int32Reader>("Microsoft.Xna.Framework.Content.Int32Reader");
		insertRegisteredReader<Int64Reader>("Microsoft.Xna.Framework.Content.Int64Reader");
		insertRegisteredReader<MatrixReader>("Microsoft.Xna.Framework.Content.MatrixReader");
		insertRegisteredReader<PointReader>("Microsoft.Xna.Framework.Content.PointReader");
		insertRegisteredReader<QuaternionReader>("Microsoft.Xna.Framework.Content.QuaternionReader");
		insertRegisteredReader<RectangleReader>("Microsoft.Xna.Framework.Content.RectangleReader");
		insertRegisteredReader<SByteReader>("Microsoft.Xna.Framework.Content.SByteReader");
		insertRegisteredReader<SingleReader>("Microsoft.Xna.Framework.Content.SingleReader");
		insertRegisteredReader<TimeSpanReader>("Microsoft.Xna.Framework.Content.TimeSpanReader");
		insertRegisteredReader<UInt16Reader>("Microsoft.Xna.Framework.Content.UInt16Reader");
		insertRegisteredReader<UInt32Reader>("Microsoft.Xna.Framework.Content.UInt32Reader");
		insertRegisteredReader<UInt64Reader>("Microsoft.Xna.Framework.Content.UInt64Reader");
		insertRegisteredReader<Vector2Reader>("Microsoft.Xna.Framework.Content.Vector2Reader");
		insertRegisteredReader<Vector3Reader>("Microsoft.Xna.Framework.Content.Vector3Reader");
		insertRegisteredReader<Vector4Reader>("Microsoft.Xna.Framework.Content.Vector4Reader");
		insertRegisteredReader<Texture2DReader>("Microsoft.Xna.Framework.Content.Texture2DReader");
		insertRegisteredReader<SoundEffectReader>("Microsoft.Xna.Framework.Content.SoundEffectReader");
		insertRegisteredReader<SpriteFontReader>("Microsoft.Xna.Framework.Content.SpriteFontReader");
		insertRegisteredReader<ListReader<Rectangle>>("Microsoft.Xna.Framework.Content.ListReader`1[[Microsoft.Xna.Framework.Rectangle");
		insertRegisteredReader<ListReader<bool>>("Microsoft.Xna.Framework.Content.ListReader`1[[System.Boolean");
		insertRegisteredReader<ListReader<Byte>>("Microsoft.Xna.Framework.Content.ListReader`1[[System.Byte");		
		insertRegisteredReader<ListReader<double>>("Microsoft.Xna.Framework.Content.ListReader`1[[System.Double");
		insertRegisteredReader<ListReader<Short>>("Microsoft.Xna.Framework.Content.ListReader`1[[System.Int16");
		insertRegisteredReader<ListReader<Int>>("Microsoft.Xna.Framework.Content.ListReader`1[[System.Int32");
		insertRegisteredReader<ListReader<Long>>("Microsoft.Xna.Framework.Content.ListReader`1[[System.Int64");
		insertRegisteredReader<ListReader<Char>>("Microsoft.Xna.Framework.Content.ListReader`1[[System.Char");
		insertRegisteredReader<ListReader<Vector3>>("Microsoft.Xna.Framework.Content.ListReader`1[[Microsoft.Xna.Framework.Vector3");
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
		insertActivadorReader<SoundEffectReader>();
		insertActivadorReader<SpriteFontReader>();				
		insertActivadorReader<ListReader<Rectangle>>();
		insertActivadorReader<ListReader<bool>>();
		insertActivadorReader<ListReader<Byte>>();
		insertActivadorReader<ListReader<double>>();
		insertActivadorReader<ListReader<Short>>();
		insertActivadorReader<ListReader<Int>>();
		insertActivadorReader<ListReader<Long>>();
		insertActivadorReader<ListReader<Char>>();	
		insertActivadorReader<ListReader<Vector3>>();
	}
}