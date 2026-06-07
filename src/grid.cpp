#include <iostream>
#include <unordered_set>

#include "grid.hpp"

#pragma region statics
static inline void clear_terminal()
{
#if defined(_WIN32) || defined(_WIN64)
	std::system("cls");
#else
	std::system("clear");
#endif
}

static unsigned int rng(unsigned int max)
{
	static bool seeded = false;
	if (!seeded)
	{
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		seeded = true;
	}

	return std::rand() % max;
}
#pragma endregion

Grid::Grid(vec2 dim)
	: m_bounds(dim)
	, m_points(new GridPoint*[dim.y])
	, m_cursor(nullptr)
{
	for (unsigned short y = 0; y < m_bounds.y; ++y)
	{
		m_points[y] = new GridPoint[m_bounds.x];
		for (unsigned short x = 0; x < m_bounds.x; ++x)
		{
			m_points[y][x].position = { x, y };
		}
	}

	uint8_t bombsRequired = static_cast<unsigned int>((dim.x * dim.y) * 0.2);
	std::unordered_set<vec2> bombPositions;
	while (bombPositions.size() < bombsRequired)
	{
		vec2 pt = { static_cast<uint16_t>(rng(m_bounds.x)), static_cast<uint16_t>(rng(m_bounds.y)) };
		if (!bombPositions.count(pt))
		{
			bombPositions.insert(pt);
			m_points[pt.y][pt.x].isBomb = true;
		}
	}

	for (unsigned short y = 0; y < m_bounds.y; ++y)
	{
		for (unsigned short x = 0; x < m_bounds.x; ++x)
		{
			GridPoint& current = m_points[y][x];
			if (current.isBomb) continue;
			std::array<GridPoint*, 8> surrounds = GetSurroundingPoints(current.position);

			uint8_t count = 0;
			for (uint8_t i = 0; i < 8; ++i)
			{
				if (surrounds[i] != nullptr && surrounds[i]->isBomb) ++count;
			}

			current.bombProx = count;
		}
	}
}

Grid::~Grid() noexcept
{
	for (unsigned short i = 0; i < m_bounds.y; ++i)
	{
		delete[] m_points[i];
	}

	delete[] m_points;
}

Grid::Grid(Grid&& other) noexcept
	: m_bounds(other.m_bounds)
	, m_cursor(other.m_cursor)
	, m_points(other.m_points)
{
	other.m_points = nullptr;
	other.m_cursor = nullptr;
	other.m_bounds = { 0, 0 };
}

Grid& Grid::operator=(Grid&& other) noexcept
{
	if (this != &other)
	{
		for (unsigned short i = 0; i < m_bounds.y; ++i)
		{
			delete[] m_points[i];
		}
		delete[] m_points;

		m_bounds = other.m_bounds;
		m_cursor = other.m_cursor;
		m_points = other.m_points;

		other.m_points = nullptr;
		other.m_cursor = nullptr;
		other.m_bounds = { 0, 0 };
	}

	return *this;
}

void Grid::Display() const
{
	clear_terminal();

	if (HitBombOnMove())
	{
		DisplayAllCleared();
		std::cout << "GAME OVER!";
		const_cast<Cursor*>(m_cursor)->m_threadActive = false;
		return;
	}

	for (unsigned short y = 0; y < m_bounds.y; ++y)
	{
		for (unsigned short x = 0; x < m_bounds.x; ++x)
		{
			GridPoint& pt = m_points[y][x];

			std::cout << GetGridSquare(pt);
		}
		std::cout << std::endl;
	}

	if (IsGameWon())
	{
		std::cout << "GAME WON!";
		const_cast<Cursor*>(m_cursor)->m_threadActive = false;
	}
}

void Grid::GiveCursor(const Cursor* cursor)
{
	m_cursor = cursor;
	Display();
}

std::string Grid::GetGridSquare(const GridPoint point) const
{
	switch (point.state)
	{
		case State::HIDDEN:
		{
			Renderer render{ 0xFFFFFF, 0x7B7B7B };

			if (m_cursor != nullptr && m_cursor->m_position == point.position)
			{
				render.colour.bg = render.colour.bg.hex - 0x202020;
			}

			return render("[ ]");
		}

		case State::CLEARED:
		{
			if (point.isBomb)
			{
				return Renderer{ 0x000000, 0xFF0000 }("[x]");
			}

			Renderer border{ 0xBDBDBD, 0x7B7B7B };
			Renderer centre{ m_numColours[point.bombProx], 0x7B7B7B};

			if (m_cursor != nullptr && m_cursor->m_position == point.position)
			{
				border.colour.bg = border.colour.bg.hex - 0x202020;
				centre.colour.bg = centre.colour.bg.hex - 0x202020;
				if (point.bombProx == 0) centre.colour.fg = centre.colour.fg.hex - 0x202020;
			}

			return border("[") + centre(std::to_string(point.bombProx)) + border("]");
		}

		case State::FLAGGED:
		{
			Renderer border{ 0xFFFFFF, 0x7B7B7B };
			Renderer centre{ 0xFD0013, 0x7B7B7B };

			if (m_cursor != nullptr && m_cursor->m_position == point.position)
			{
				border.colour.bg = border.colour.bg.hex - 0x202020;
				centre.colour.bg = centre.colour.bg.hex - 0x202020;
			}

			return border("[") + centre(">") + border("]");
		}

		default:
		{
			throw std::exception("Invalid state");
		}
	}
}

std::array<GridPoint*, 8> Grid::GetSurroundingPoints(vec2 pos) const
{
	auto isValid = [this](uint16_t y, uint16_t x) { return (x >= 0 && y >= 0) && (x < m_bounds.x && y < m_bounds.y); };

	std::array<GridPoint*, 8> points
	{
		isValid(pos.y-1, pos.x-1)	? &m_points[pos.y-1][pos.x-1]	: nullptr,
		isValid(pos.y-1, pos.x)		? &m_points[pos.y-1][pos.x]		: nullptr,
		isValid(pos.y-1, pos.x+1)	? &m_points[pos.y-1][pos.x+1]	: nullptr,

		isValid(pos.y, pos.x-1)		? &m_points[pos.y][pos.x-1]		: nullptr,
		// middle not included
		isValid(pos.y, pos.x+1)		? &m_points[pos.y][pos.x+1]		: nullptr,

		isValid(pos.y+1, pos.x-1)	? &m_points[pos.y+1][pos.x-1]	: nullptr,
		isValid(pos.y+1, pos.x)		? &m_points[pos.y+1][pos.x]		: nullptr,
		isValid(pos.y+1, pos.x+1)	? &m_points[pos.y+1][pos.x+1]	: nullptr
	};

	return points;
}

GridPoint& Grid::operator[](vec2 vec) const
{
	return m_points[vec.y][vec.x];
}

bool Grid::IsGameWon() const
{
	uint16_t flaggedBombs = 0;
	uint16_t clearedSpaces = 0;

	for (unsigned short y = 0; y < m_bounds.y; ++y)
	{
		for (unsigned short x = 0; x < m_bounds.x; ++x)
		{
			GridPoint& current = m_points[y][x];

			if (current.isBomb && current.state == State::FLAGGED)
			{
				++flaggedBombs;
			}
			else if (!current.isBomb && current.state == State::CLEARED)
			{
				++clearedSpaces;
			}
		}
	}

	return (flaggedBombs + clearedSpaces) == (m_bounds.x * m_bounds.y);
}

inline bool Grid::HitBombOnMove() const
{
	return (*this)[m_cursor->m_position].isBomb && m_cursor->m_pressedChar == ' ';
}

void Grid::DisplayAllCleared() const
{
	for (unsigned short y = 0; y < m_bounds.y; ++y)
	{
		for (unsigned short x = 0; x < m_bounds.x; ++x)
		{
			GridPoint& pt = m_points[y][x];
			pt.state = State::CLEARED;

			std::cout << GetGridSquare(pt);
		}
		std::cout << std::endl;
	}
}