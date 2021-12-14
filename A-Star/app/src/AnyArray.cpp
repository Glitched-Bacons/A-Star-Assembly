#include "AnyArray.h"

#include <utility>

AnyArray::AnyArray(int* array1D, int width, int height)
{
	mArray2D.resize(height);
	for(int row = 0; row < height; ++row)
	{
		mArray2D[row].resize(width);
		for(int col = 0; col < width; ++col)
		{
			mArray2D[row][col] = array1D[row * width + col];
		}
	}
}

AnyArray::AnyArray(std::vector<int> array1D, int width)
{
	mArray2D.reserve(array1D.size() / width);
	int height = array1D.size() / width;

	for(int row = 0; row < height; ++row)
	{
		mArray2D.emplace_back(array1D.begin() + row * width, array1D.begin() + (row + 1) * width);
	}
}

AnyArray::AnyArray(std::vector<std::vector<int>> array2D)
	: mArray2D(std::move(array2D))
{
	
}

AnyArray::operator std::vector<std::vector<int>>() const
{
	return mArray2D;
}
