#pragma once
#include <cmath>
#include <unordered_map>

#include "Vector.h"

namespace as
{

/**
 * \brief A three-dimensional point in space with x, y, z coordinates.
 * \tparam T Coordinate type (e.g. float, int)
 */
template <typename T>
struct Vector3 : public Vector
{
	float getDistanceTo(const Vector3& vec) const
	{
		return std::sqrt(std::pow((x - vec.x), 2) + std::pow((y - vec.y), 2) 
			+ std::pow((z - vec.z), 2));
	}

	T x;
	T y;
	T z;
};

using Vector3i = Vector3<int>;
using Vector3f = Vector3<float>;

} // namespace as
