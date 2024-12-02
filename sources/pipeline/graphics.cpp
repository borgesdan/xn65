#include "pipeline/graphics.hpp"
#include "misc.hpp"
#include <memory>

namespace xna {	

	void xna::BitmapContent::Copy(BitmapContent const& sourceBitmap, Rectangle const& sourceRegion, BitmapContent& destinationBitmap, Rectangle const& destinationRegion)
	{
		BitmapContent::ValidateCopyArguments(sourceBitmap, sourceRegion, destinationBitmap, destinationRegion);

		if (sourceBitmap.TryCopyTo(destinationBitmap, sourceRegion, destinationRegion) || destinationBitmap.TryCopyFrom(sourceBitmap, sourceRegion, destinationRegion))
			return;

		auto bitmapContent1 = misc::reinterpret_make_shared<BitmapContent, PixelBitmapContent<Vector4>>(sourceBitmap.Width(), sourceBitmap.Height());
		auto rectangle1 = Rectangle(0, 0, bitmapContent1->Width(), bitmapContent1->Height());		

		if (sourceBitmap.TryCopyTo(*bitmapContent1, sourceRegion, rectangle1) && destinationBitmap.TryCopyFrom(*bitmapContent1, rectangle1, destinationRegion))
			return;

		auto bitmapContent2 = misc::reinterpret_make_shared<BitmapContent, PixelBitmapContent<Vector4>>(sourceBitmap.Width(), sourceBitmap.Height());
		auto bitmapContent3 = misc::reinterpret_make_shared<BitmapContent, PixelBitmapContent<Vector4>>(destinationBitmap.Width(), destinationBitmap.Height());

		auto rectangle2 = Rectangle(0, 0, sourceBitmap.Width(), sourceBitmap.Height());
		auto rectangle3 = Rectangle(0, 0, destinationBitmap.Width(), destinationBitmap.Height());		

		if (!sourceBitmap.TryCopyTo(*bitmapContent2, rectangle2, rectangle2) || !destinationBitmap.TryCopyTo(*bitmapContent3, rectangle3, rectangle3)
			|| !bitmapContent3->TryCopyFrom(*bitmapContent2, sourceRegion, destinationRegion) || !destinationBitmap.TryCopyFrom(*bitmapContent3, rectangle3, rectangle3))
		{
			throw csharp::InvalidOperationException();
		}
	}

	bool xna::BitmapContent::InteropCopy(BitmapContent const& sourceBitmap, Rectangle const& sourceRegion, BitmapContent& destinationBitmap, Rectangle const& destinationRegion)
	{
		BitmapContent::ValidateCopyArguments(sourceBitmap, sourceRegion, destinationBitmap, destinationRegion);

		bool flag = false;
		SurfaceFormat format1;
		SurfaceFormat format2;

		if (destinationBitmap.TryGetFormat(format1) && sourceBitmap.TryGetFormat(format2))
		{
			if (format2 != format1 && (format2 == SurfaceFormat::NormalizedByte2
				|| format2 == SurfaceFormat::NormalizedByte4 
				|| format1 == SurfaceFormat::NormalizedByte2 
				|| format1 == SurfaceFormat::NormalizedByte4 
				|| format2 == SurfaceFormat::Rg32 
				|| format2 == SurfaceFormat::Single 
				|| format2 == SurfaceFormat::Vector2 
				|| format2 == SurfaceFormat::HalfSingle 
				|| format2 == SurfaceFormat::HalfVector2))
				return false;

			auto pixelData1 = sourceBitmap.GetPixelData();
			auto pixelData2 = destinationBitmap.GetPixelData();
			try
			{
				//TODO: ImageProcessor?
				//ImageProcessor.Convert(sourceBitmap.Width, sourceBitmap.Height, format2, new Rectangle ? (sourceRegion), pixelData1, destinationBitmap.Width, destinationBitmap.Height, format1, new Rectangle ? (destinationRegion), pixelData2);
				destinationBitmap.SetPixelData(pixelData2);
				flag = true;
			}
			catch(std::exception& ex)
			{
				flag = false;
			}
		}

		return flag;
	}

	void xna::BitmapContent::ValidateCopyArguments(BitmapContent const& sourceBitmap, Rectangle const& sourceRegion,
		BitmapContent const& destinationBitmap, Rectangle const& destinationRegion) {

		if (sourceRegion.Left() < 0 || sourceRegion.Top() < 0 || sourceRegion.Width < 0 || sourceRegion.Height < 0 || sourceRegion.Right() > sourceBitmap.Width() || sourceRegion.Bottom() > sourceBitmap.Height())
			throw csharp::InvalidOperationException();

		if (destinationRegion.Left() < 0 || destinationRegion.Top() < 0 || destinationRegion.Width < 0 || destinationRegion.Height < 0 || destinationRegion.Right() > destinationBitmap.Width() || destinationRegion.Bottom() > destinationBitmap.Height())
			throw csharp::InvalidOperationException();
	}
}