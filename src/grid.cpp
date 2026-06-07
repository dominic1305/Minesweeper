#include <iostream>

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
#pragma endregion

Grid::Grid(Dimensions dim)
	: m_bounds(dim)
	, m_points(new GridPoint* [dim.cols])
	, m_cursor(nullptr)
{
	for (unsigned short y = 0; y < dim.cols; ++y)
	{
		m_points[y] = new GridPoint[dim.rows];

		for (unsigned short x = 0; x < dim.rows; ++x)
		{
			m_points[y][x] = { x, y };
		}
	}
}

Grid::~Grid() noexcept
{
	for (unsigned short i = 0; i < m_bounds.cols; ++i)
	{
		delete[] m_points[i];
	}

	delete[] m_points;
}

void Grid::Display() const
{
	clear_terminal();

	for (unsigned short y = 0; y < m_bounds.cols; ++y)
	{
		for (unsigned short x = 0; x < m_bounds.rows; ++x)
		{
			std::cout << GetGridSquare(m_points[y][x]);
		}
		std::cout << std::endl;
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
			Renderer render{ 0xBDBDBD, 0x7B7B7B };

			if (m_cursor != nullptr && m_cursor->m_position == point.position)
			{
				render.colour.bg = render.colour.bg.hex - 0x202020;
			}

			return render("[ ]");
			break;
		}

		case State::CLEARED:
		{
			throw std::exception("Not Implemented");
			break;
		}

		case State::FLAGGED:
		{
			Renderer border = { 0xBDBDBD, 0x7B7B7B };
			Renderer centre = { 0xFD0013, 0x7B7B7B };

			if (m_cursor != nullptr && m_cursor->m_position == point.position)
			{
				border.colour.bg = border.colour.bg.hex - 0x202020;
				centre.colour.bg = centre.colour.bg.hex - 0x202020;
			}

			return border("[") + centre(">") + border("]");

			break;
		}

		default:
		{
			throw std::exception("Invalid state");
		}
	}
}