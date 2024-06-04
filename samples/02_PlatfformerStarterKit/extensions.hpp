#ifndef PLATFORMSTARTERKIT_EXTENSIONS_HPP
#define PLATFORMSTARTERKIT_EXTENSIONS_HPP

#include "xna/xna.hpp"

namespace PlatformerStarterKit {
	struct RectangleExtensions {
        static xna::Vector2 GetIntersectionDepth(xna::Rectangle const& rectA, xna::Rectangle const& rectB) {            
            const auto halfWidthA = rectA.Width / 2.0f;
            const auto halfHeightA = rectA.Height / 2.0f;
            const auto halfWidthB = rectB.Width / 2.0f;
            const auto halfHeightB = rectB.Height / 2.0f;
            
            const auto centerA = xna::Vector2(rectA.Left() + halfWidthA, rectA.Top() + halfHeightA);
            const auto centerB = xna::Vector2(rectB.Left() + halfWidthB, rectB.Top() + halfHeightB);
            
            const auto distanceX = centerA.X - centerB.X;
            const auto distanceY = centerA.Y - centerB.Y;
            const auto minDistanceX = halfWidthA + halfWidthB;
            const auto minDistanceY = halfHeightA + halfHeightB;
            
            if (std::abs(distanceX) >= minDistanceX || std::abs(distanceY) >= minDistanceY)
                return xna::Vector2::Zero();
                        
            const auto depthX = distanceX > 0 ? minDistanceX - distanceX : -minDistanceX - distanceX;
            const auto depthY = distanceY > 0 ? minDistanceY - distanceY : -minDistanceY - distanceY;
            return xna::Vector2(depthX, depthY);
        }

		static constexpr xna::Vector2 GetBottomCenter(xna::Rectangle const& rect) {
			return xna::Vector2(rect.X + rect.Width / 2.0f, rect.Bottom());
		}
	};
}

#endif