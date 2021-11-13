#pragma once
#include <stdexcept>

#include "Structures/Vector.h"
#include <type_traits>

namespace as
{

/**
 * \brief A basic structure representing a single point inside the A* algorithm.
 * \tparam Vec The type of which is Node. It can only be one of the Vectors.
 */
template <typename Vec, class = std::enable_if_t<std::is_base_of_v<Vector, Vec>>>
struct Node
{
public:
	explicit Node(Vec&& position) : mPosition(std::move(position)) {}

	bool isTraversable() const;
	bool setTraversable(bool isTraversable);
	const Vec& getPosition() const;

	float getCostTo(const Node& node) const;
	bool operator==(const Node& rhs) const;

private:
	Vec mPosition;
	bool mIsTraversable = true;
};

template <typename Vec, class T0>
bool Node<Vec, T0>::isTraversable() const
{
	return mIsTraversable;
}

template <typename Vec, class T0>
bool Node<Vec, T0>::setTraversable(bool isTraversable)
{
	mIsTraversable = isTraversable;
}

template <typename Vec, class T0>
const Vec& Node<Vec, T0>::getPosition() const
{
	return mPosition;
}

template <typename Vec, class T0>
float Node<Vec, T0>::getCostTo(const Node& node) const
{
	return mPosition.getDistanceTo(node.getPosition());
}

template <typename Vec, class T0>
bool Node<Vec, T0>::operator==(const Node& rhs) const
{
	return getPosition() == rhs.getPosition();
}
}

namespace std
{
	template <typename T>
	struct hash<as::Node<T>>
	{
		std::size_t operator()(const as::Node<T>& node) const noexcept
		{
			return ((hash<T>()(node.getPosition())));
		}
	};

}