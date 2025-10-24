#ifndef XNA_FRAMEWORK_BOUNDINGVOLUME_HPP
#define XNA_FRAMEWORK_BOUNDINGVOLUME_HPP

namespace Xna {
	//Describes the intersection between a plane and a bounding volume.
	enum class PlaneIntersectionType {
		//There is no intersection, and the bounding volume is in the positive half-space of the Plane.
		Front,
		//There is no intersection, and the bounding volume is in the negative half - space of the Plane.
		Back,
		//The Plane is intersected.
		Intersecting,
	};

	//Indicates the extent to which bounding volumes intersect or contain one another. 
	enum class ContainmentType {
		//Indicates there is no overlap between the bounding volumes.
		Disjoint,
		//Indicates that one bounding volume completely contains the other.
		Contains,
		//Indicates that the bounding volumes partially overlap.
		Intersects,
	};
}

#endif