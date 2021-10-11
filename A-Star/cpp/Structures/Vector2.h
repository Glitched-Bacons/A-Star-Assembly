#pragma once
#include <cmath>
#include <unordered_map>

#include "Structures/Vector.h"

namespace as
{

/**
 * \brief A two-dimensional point in space with x and y coordinates.
 * \tparam T Coordinate type (e.g. float, int)
 */
template <typename T>
struct Vector2 : public Vector
{
	Vector2(T&& x, T&& y) : x(std::move(x)), y(std::move(y)) {}

	float getDistanceTo(const Vector2& vec) const
	{
		return std::sqrt(std::pow((x - vec.x), 2) + std::pow((y - vec.y), 2));
	}

	bool operator==(const Vector2& rhs) const;
	
	T x;
	T y;
};


template <typename T>
bool Vector2<T>::operator==(const Vector2& rhs) const
{
	return (x == rhs.x) && (y == rhs.y);
}

using Vector2i = Vector2<int>;
using Vector2f = Vector2<float>;


} // namespace as

namespace std
{
	template <typename T>
	struct hash<as::Vector2<T>>
	{
		std::size_t operator()(const as::Vector2<T>& k) const noexcept
		{
			return ((hash<T>()(k.x)
				^ (hash<T>()(k.y) << 1)) >> 1);
		}
	};

}


