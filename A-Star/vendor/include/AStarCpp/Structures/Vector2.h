#pragma once
#include <cmath>
#include <unordered_map>

#include "Vector.h"

namespace as
{

/**
 * \brief A two-dimensional point in space with x and y coordinates.
 * \tparam T Coordinate type (e.g. float, int)
 */
template <typename T>
struct Vector2 : public Vector
{
	Vector2(T&& x, T&& y) : x(std::forward<T>(x)), y(std::forward<T>(y)) {}

	T getManhattanDistanceTo(const Vector2& vec) const
	{
		auto distanceX = x - vec.x;
		auto distanceY = y - vec.y;
		return ((distanceX >= 0) ? distanceX : -distanceX) +
			((distanceY >= 0) ? distanceY : -distanceY);
	}

	T getEuclideanDistanceTo(const Vector2& vec) const
	{
		auto distanceX = x - vec.x;
		auto distanceY = y - vec.y;
		return std::sqrt(distanceX * distanceX + distanceY * distanceY);
	}
	bool operator==(const Vector2& rhs) const;
	Vector2<int> operator+(const Vector2& offset) const;
	bool operator!=(const Vector2& vector2) const;

	T x;
	T y;
};


template <typename T>
bool Vector2<T>::operator==(const Vector2& rhs) const
{
	return (x == rhs.x) && (y == rhs.y);
}

template <typename T>
Vector2<int> Vector2<T>::operator+(const Vector2& offset) const
{
	return { x + offset.x, y + offset.y };
}

template <typename T>
bool Vector2<T>::operator!=(const Vector2& rhs) const
{
	return !(*this == rhs);
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


