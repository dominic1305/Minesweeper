#pragma once

#include <string>

#include "dimensions.hpp"
#include "render.hpp"
#include "cursor.hpp"

enum class State : uint8_t
{
	HIDDEN,
	CLEARED,
	FLAGGED,
};

class Grid
{
private:

	friend class Cursor;

	struct GridPoint
	{
		Dimensions			position	= {};
		bool				isBomb		= false;
		uint8_t				bombProx	= 0;
		State				state		= State::HIDDEN;
	};

public:

							Grid(Dimensions dim);

							~Grid() noexcept;

							Grid(const Grid&)		= delete;
	Grid&					operator=(const Grid&)	= delete;

							Grid(Grid&&)			= delete;
	Grid&					operator=(Grid&&)		= delete;

	void					Display() const;

	void					GiveCursor(const Cursor* cursor);

	std::string				GetGridSquare(const GridPoint point) const;

private:

	const rgb				m_numColours[8]
	{
		0x0026F9,
		0x0D811A,
		0xFC0012,
		0x000E81,
		0x830005,
		0x0B8284,
		0x830D81,
		0x757575
	};

	Dimensions				m_bounds;
	const Cursor*			m_cursor;
	GridPoint**				m_points;

};