#pragma once

struct Dimensions
{
	unsigned short		rows;
	unsigned short		cols;

	constexpr bool operator==(Dimensions other) const
	{
		return rows == other.rows && cols == other.cols;
	}
};