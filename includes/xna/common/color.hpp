#ifndef XNA_COMMON_COLOR_HPP
#define XNA_COMMON_COLOR_HPP

#include "../default.hpp"
#include "packedvalue.hpp"

namespace xna {
	//Represents a four-component color using red, green, blue, and alpha data. 
	struct Color : public IPackedVector, public IPackedVectorT<Uint> {
		constexpr Color() = default;

		constexpr Color(Uint packedValue)
			: _packedValue(packedValue) {}

		constexpr Color(Int r, Int g, Int b, Int a = 255U) {
			if (((r | g | b | a) & -256) != 0)
			{
				r = ClampToByte32(r);
				g = ClampToByte32(g);
				b = ClampToByte32(b);
				a = ClampToByte32(a);
			}

			g <<= 8;
			b <<= 16;
			a <<= 24;

			_packedValue = static_cast<Uint>(r | g | b | a);
		}

		Color(float r, float g, float b, float a = 1.0F);
		Color(Vector3 const& vector);
		Color(Vector4 const& vector);

		virtual void PackFromVector4(Vector4 const& vector) override;

		//Converts a non-premultipled alpha color to a color that contains premultiplied alpha.
		static Color FromNonPremultiplied(Vector4 const& vector);

		//Converts a non-premultipled alpha color to a color that contains premultiplied alpha.
		static Color FromNonPremultiplied(Int r, Int g, Int b, Int a);

		//Gets a three-component vector representation for this object.
		constexpr Vector3 ToVector3() const {
			Vector3 vector3;
			const auto byteMax = static_cast<Uint>(ByteMaxValue);
			vector3.X = PackUtils::UnpackUNorm(byteMax, _packedValue);
			vector3.Y = PackUtils::UnpackUNorm(byteMax, _packedValue >> 8);
			vector3.Z = PackUtils::UnpackUNorm(byteMax, _packedValue >> 16);
			return vector3;
		}

		//Gets a four-component vector representation for this object.
		constexpr virtual Vector4 ToVector4() const	override {
			Vector4 vector4;
			const auto byteMax = static_cast<Uint>(ByteMaxValue);
			vector4.X = PackUtils::UnpackUNorm(byteMax, _packedValue);
			vector4.Y = PackUtils::UnpackUNorm(byteMax, _packedValue >> 8);
			vector4.Z = PackUtils::UnpackUNorm(byteMax, _packedValue >> 16);
			vector4.W = PackUtils::UnpackUNorm(byteMax, _packedValue >> 24);
			return vector4;
		}

		//Gets or sets the red component value of this Color. 
		constexpr Byte R() const {
			return static_cast<Byte>(_packedValue);
		}

		//Gets or sets the red component value of this Color. 
		constexpr void R(Byte value) {
			_packedValue = _packedValue & (UintMaxValue - 255) | static_cast<Uint>(value);
		}

		//Gets or sets the green component value of this Color. 
		constexpr Byte G() const {
			return static_cast<Byte>(_packedValue >> 8);
		}

		//Gets or sets the green component value of this Color. 
		constexpr void G(Byte value) {
			_packedValue = (static_cast<Int>(_packedValue) & -65281 | static_cast<Int>(value) << 8);
		}

		//Gets or sets the blue component value of this Color. 
		constexpr Byte B() const {
			return static_cast<Byte>(_packedValue >> 16);
		}

		//Gets or sets the blue component value of this Color. 
		constexpr void B(Byte value) {
			_packedValue = (static_cast<Int>(_packedValue) & -16711681 | static_cast<Int>(value) << 16);
		}

		//Gets or sets the alpha component value. 
		constexpr Byte A() const {
			return static_cast<Byte>(_packedValue >> 24);
		}

		//Gets or sets the alpha component value. 
		constexpr void A(Byte value) {
			_packedValue = (static_cast<Int>(_packedValue) & 16777215 | static_cast<Int>(value) << 24);
		}

		//Gets or sets the current color as a packed value.
		virtual constexpr Uint PackedValue() const override {
			return _packedValue;
		}

		//Gets or sets the current color as a packed value.
		virtual constexpr void PackedValue(Uint const& value) override {
			_packedValue = value;
		}

		//Linearly interpolate a color.
		static Color Lerp(Color const& value1, Color const& value2, float amount);

		//Multiply each color component by the scale factor.
		static constexpr Color Multiply(Color const& value, float scale) {
			const Uint r = value.R();
			const Uint g = value.G();
			const Uint b = value.B();
			const Uint a = value.A();

			scale *= 65536.0f;

			const Uint num5 = scale >= 0.0F ? (scale <= 16777215.0F ? static_cast<Uint>(scale) : 16777215U) : 0U;

			Uint r1 = r * num5 >> 16;
			Uint g1 = g * num5 >> 16;
			Uint b1 = b * num5 >> 16;
			Uint a1 = a * num5 >> 16;

			if (r1 > ByteMaxValue)
				r1 = ByteMaxValue;

			if (g1 > ByteMaxValue)
				g1 = ByteMaxValue;

			if (b1 > ByteMaxValue)
				b1 = ByteMaxValue;

			if (a1 > ByteMaxValue)
				a1 = ByteMaxValue;

			const auto r2 = static_cast<Int>(r1);
			const auto g2 = static_cast<Int>(g1);
			const auto b2 = static_cast<Int>(b1);
			const auto a2 = static_cast<Int>(a1);

			Color color;
			color._packedValue = static_cast<Uint>(r2 | g2 << 8 | b2 << 16 | a2 << 24);
			return color;
		}

		constexpr bool operator==(Color const& other) const {
			return _packedValue == other._packedValue;
		}

		friend constexpr Color operator*(Color const& value, float const& scale) {
			return Color::Multiply(value, scale);
		}

		constexpr operator Uint() const { 
			return _packedValue; 
		}

	private:
		Uint _packedValue{ 0 };

		static constexpr Int ClampToByte32(Int value) noexcept {
			if (value < 0)
				return 0;

			return value > ByteMaxValue ? ByteMaxValue : value;
		}

		static Uint PackHelper(float vectorX, float vectorY, float vectorZ, float vectorW);
	};

	struct Colors {
		static constexpr Color Transparent{ Color(0U) };
		static constexpr Color AliceBlue{ Color(4294965488U) };
		static constexpr Color AntiqueWhite{ Color(4292340730U) };
		static constexpr Color Aqua{ Color(4294967040U) };
		static constexpr Color Aquamarine{ Color(4292149119U) };
		static constexpr Color Azure{ Color(4294967280U) };
		static constexpr Color Beige{ Color(4292670965U) };
		static constexpr Color Bisque{ Color(4291093759U) };
		static constexpr Color Black{ Color(4278190080U) };
		static constexpr Color BlanchedAlmond{ Color(4291685375U) };
		static constexpr Color Blue{ Color(4294901760U) };
		static constexpr Color BlueViolet{ Color(4293012362U) };
		static constexpr Color Brown{ Color(4280953509U) };
		static constexpr Color BurlyWood{ Color(4287084766U) };
		static constexpr Color CadetBlue{ Color(4288716383U) };
		static constexpr Color Chartreuse{ Color(4278255487U) };
		static constexpr Color Chocolate{ Color(4280183250U) };
		static constexpr Color Coral{ Color(4283465727U) };
		static constexpr Color CornflowerBlue{ Color(4293760356U) };
		static constexpr Color Cornsilk{ Color(4292671743U) };
		static constexpr Color Crimson{ Color(4282127580U) };
		static constexpr Color Cyan{ Color(4294967040U) };
		static constexpr Color DarkBlue{ Color(4287299584U) };
		static constexpr Color DarkCyan{ Color(4287335168U) };
		static constexpr Color DarkGoldenrod{ Color(4278945464U) };
		static constexpr Color DarkGray{ Color(4289309097U) };
		static constexpr Color DarkGreen{ Color(4278215680U) };
		static constexpr Color DarkKhaki{ Color(4285249469U) };
		static constexpr Color DarkMagenta{ Color(4287299723U) };
		static constexpr Color DarkOliveGreen{ Color(4281297749U) };
		static constexpr Color DarkOrange{ Color(4278226175U) };
		static constexpr Color DarkOrchid{ Color(4291572377U) };
		static constexpr Color DarkRed{ Color(4278190219U) };
		static constexpr Color DarkSalmon{ Color(4286224105U) };
		static constexpr Color DarkSeaGreen{ Color(4287347855U) };
		static constexpr Color DarkSlateBlue{ Color(4287315272U) };
		static constexpr Color DarkSlateGray{ Color(4283387695U) };
		static constexpr Color DarkTurquoise{ Color(4291939840U) };
		static constexpr Color DarkViolet{ Color(4292018324U) };
		static constexpr Color DeepPink{ Color(4287829247U) };
		static constexpr Color DeepSkyBlue{ Color(4294950656U) };
		static constexpr Color DimGray{ Color(4285098345U) };
		static constexpr Color DodgerBlue{ Color(4294938654U) };
		static constexpr Color Firebrick{ Color(4280427186U) };
		static constexpr Color FloralWhite{ Color(4293982975U) };
		static constexpr Color ForestGreen{ Color(4280453922U) };
		static constexpr Color Fuchsia{ Color(4294902015U) };
		static constexpr Color Gainsboro{ Color(4292664540U) };
		static constexpr Color GhostWhite{ Color(4294965496U) };
		static constexpr Color Gold{ Color(4278245375U) };
		static constexpr Color Goldenrod{ Color(4280329690U) };
		static constexpr Color Gray{ Color(4286611584U) };
		static constexpr Color Green{ Color(4278222848U) };
		static constexpr Color GreenYellow{ Color(4281335725U) };
		static constexpr Color Honeydew{ Color(4293984240U) };
		static constexpr Color HotPink{ Color(4290013695U) };
		static constexpr Color IndianRed{ Color(4284243149U) };
		static constexpr Color Indigo{ Color(4286709835U) };
		static constexpr Color Ivory{ Color(4293984255U) };
		static constexpr Color Khaki{ Color(4287424240U) };
		static constexpr Color Lavender{ Color(4294633190U) };
		static constexpr Color LavenderBlush{ Color(4294308095U) };
		static constexpr Color LawnGreen{ Color(4278254716U) };
		static constexpr Color LemonChiffon{ Color(4291689215U) };
		static constexpr Color LightBlue{ Color(4293318829U) };
		static constexpr Color LightCoral{ Color(4286611696U) };
		static constexpr Color LightCyan{ Color(4294967264U) };
		static constexpr Color LightGoldenrodYellow{ Color(4292016890U) };
		static constexpr Color LightGreen{ Color(4287688336U) };
		static constexpr Color LightGray{ Color(4292072403U) };
		static constexpr Color LightPink{ Color(4290885375U) };
		static constexpr Color LightSalmon{ Color(4286226687U) };
		static constexpr Color LightSeaGreen{ Color(4289376800U) };
		static constexpr Color LightSkyBlue{ Color(4294626951U) };
		static constexpr Color LightSlateGray{ Color(4288252023U) };
		static constexpr Color LightSteelBlue{ Color(4292789424U) };
		static constexpr Color LightYellow{ Color(4292935679U) };
		static constexpr Color Lime{ Color(4278255360U) };
		static constexpr Color LimeGreen{ Color(4281519410U) };
		static constexpr Color Linen{ Color(4293325050U) };
		static constexpr Color Magenta{ Color(4294902015U) };
		static constexpr Color Maroon{ Color(4278190208U) };
		static constexpr Color MediumAquamarine{ Color(4289383782U) };
		static constexpr Color MediumBlue{ Color(4291624960U) };
		static constexpr Color MediumOrchid{ Color(4292040122U) };
		static constexpr Color MediumPurple{ Color(4292571283U) };
		static constexpr Color MediumSeaGreen{ Color(4285641532U) };
		static constexpr Color MediumSlateBlue{ Color(4293814395U) };
		static constexpr Color MediumSpringGreen{ Color(4288346624U) };
		static constexpr Color MediumTurquoise{ Color(4291613000U) };
		static constexpr Color MediumVioletRed{ Color(4286911943U) };
		static constexpr Color MidnightBlue{ Color(4285536537U) };
		static constexpr Color MintCream{ Color(4294639605U) };
		static constexpr Color MistyRose{ Color(4292994303U) };
		static constexpr Color Moccasin{ Color(4290110719U) };
		static constexpr Color NavajoWhite{ Color(4289584895U) };
		static constexpr Color Navy{ Color(4286578688U) };
		static constexpr Color OldLace{ Color(4293326333U) };
		static constexpr Color Olive{ Color(4278222976U) };
		static constexpr Color OliveDrab{ Color(4280520299U) };
		static constexpr Color Orange{ Color(4278232575U) };
		static constexpr Color OrangeRed{ Color(4278207999U) };
		static constexpr Color Orchid{ Color(4292243674U) };
		static constexpr Color PaleGoldenrod{ Color(4289390830U) };
		static constexpr Color PaleGreen{ Color(4288215960U) };
		static constexpr Color PaleTurquoise{ Color(4293848751U) };
		static constexpr Color PaleVioletRed{ Color(4287852763U) };
		static constexpr Color PapayaWhip{ Color(4292210687U) };
		static constexpr Color PeachPuff{ Color(4290370303U) };
		static constexpr Color Peru{ Color(4282353101U) };
		static constexpr Color Pink{ Color(4291543295U) };
		static constexpr Color Plum{ Color(4292714717U) };
		static constexpr Color PowderBlue{ Color(4293320880U) };
		static constexpr Color Purple{ Color(4286578816U) };
		static constexpr Color Red{ Color(4278190335U) };
		static constexpr Color RosyBrown{ Color(4287598524U) };
		static constexpr Color RoyalBlue{ Color(4292962625U) };
		static constexpr Color SaddleBrown{ Color(4279453067U) };
		static constexpr Color Salmon{ Color(4285694202U) };
		static constexpr Color SandyBrown{ Color(4284523764U) };
		static constexpr Color SeaGreen{ Color(4283927342U) };
		static constexpr Color SeaShell{ Color(4293850623U) };
		static constexpr Color Sienna{ Color(4281160352U) };
		static constexpr Color Silver{ Color(4290822336U) };
		static constexpr Color SkyBlue{ Color(4293643911U) };
		static constexpr Color SlateBlue{ Color(4291648106U) };
		static constexpr Color SlateGray{ Color(4287660144U) };
		static constexpr Color Snow{ Color(4294638335U) };
		static constexpr Color SpringGreen{ Color(4286578432U) };
		static constexpr Color SteelBlue{ Color(4290019910U) };
		static constexpr Color Tan{ Color(4287411410U) };
		static constexpr Color Teal{ Color(4286611456U) };
		static constexpr Color Thistle{ Color(4292394968U) };
		static constexpr Color Tomato{ Color(4282868735U) };
		static constexpr Color Turquoise{ Color(4291878976U) };
		static constexpr Color Violet{ Color(4293821166U) };
		static constexpr Color Wheat{ Color(4289978101U) };
		static constexpr Color White{ Color(UintMaxValue) };
		static constexpr Color WhiteSmoke{ Color(4294309365U) };
		static constexpr Color Yellow{ Color(4278255615U) };
		static constexpr Color YellowGreen{ Color(4281519514U) };

	private:
		constexpr Colors() = default;
		constexpr Colors(Colors&&) = default;
		constexpr Colors(const Colors&) = default;
	};
}

#endif