#pragma once

struct vec2
{
	unsigned short		x;
	unsigned short		y;

	constexpr bool		operator==(vec2 other) const
	{
		return x == other.x && y == other.y;
	}
};

template<>
struct std::hash<vec2>
{
	size_t operator()(const vec2 dim) const noexcept
	{
		size_t h1 = std::hash<unsigned short>{}(dim.x);
		size_t h2 = std::hash<unsigned short>{}(dim.y);
		return h1 ^ (h2 << 1);
	}
};