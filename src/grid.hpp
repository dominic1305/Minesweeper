#pragma once

#include <string>
#include <optional>

#include "vec2.hpp"
#include "render.hpp"
#include "cursor.hpp"

enum class State : uint8_t
{
	HIDDEN,
	CLEARED,
	FLAGGED,
};

struct GridPoint
{
	vec2					position = {};
	bool					isBomb = false;
	uint8_t					bombProx = 0;
	State					state = State::HIDDEN;
};

class Grid
{
private:

	friend class Cursor;

public:

								Grid(vec2 dim);

								~Grid() noexcept;

								Grid(const Grid&)		= delete;
	Grid&						operator=(const Grid&)	= delete;

								Grid(Grid&& other) noexcept;
	Grid&						operator=(Grid&& other) noexcept;

	void						Display() const;

	void						GiveCursor(const Cursor* cursor);

private:

	std::string					GetGridSquare(const GridPoint point) const;

	std::array<GridPoint*, 8>	GetSurroundingPoints(vec2 pos) const;

	GridPoint&					operator[](vec2 vec) const;

	bool						IsGameWon() const;

	inline bool					HitBombOnMove() const;

	void						DisplayAllCleared() const;

	const rgb					m_numColours[9]
	{
		0x7B7B7B,
		0x0026F9,
		0x0D811A,
		0xFC0012,
		0x000E81,
		0x830005,
		0x0B8284,
		0x830D81,
		0x757575
	};

	vec2						m_bounds;
	const Cursor*				m_cursor;
	GridPoint**					m_points;

};