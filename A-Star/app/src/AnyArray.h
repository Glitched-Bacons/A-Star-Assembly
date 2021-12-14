#pragma once
#include <vector>


class AnyArray
{
public:
	AnyArray() = default;
	AnyArray(int* array1D, int width, int height);
	AnyArray(std::vector<int> array1D, int width);
	AnyArray(std::vector<std::vector<int>> array2D);

	operator std::vector<std::vector<int>>() const;

private:
	std::vector<std::vector<int>> mArray2D;
};
