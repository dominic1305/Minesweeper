#include <iostream>
#include <conio.h>

#include "cursor.hpp"

Cursor::Cursor(Dimensions dim, std::variant<Grid*> display)
	: m_bounds(dim)
	, m_position({ 0, 0 })
	, m_display(display)
{
	m_thread = std::jthread{ KeyboardListener, this };
}

Cursor::Cursor(Cursor&& other) noexcept
	: m_bounds(std::move(other.m_bounds))
	, m_position(std::move(other.m_position))
	, m_thread(std::move(other.m_thread))
{
}

Cursor& Cursor::operator=(Cursor&& other) noexcept
{
	if (this != &other)
	{
		m_bounds = std::move(other.m_bounds);
		m_position = std::move(other.m_position);
		m_thread = std::move(other.m_thread);
	}

	return *this;
}

void Cursor::KeyboardListener(std::stop_token stoken, Cursor* const cursor)
{
	while (!stoken.stop_requested())
	{
		char found = _getch();

		switch (found)
		{
			case 'w':
			case 'W':
			{
				if (cursor->m_position.cols > 0)
				{
					--cursor->m_position.cols;
				}
				break;
			}
			case 'a':
			case 'A':
			{
				if (cursor->m_position.rows > 0)
				{
					--cursor->m_position.rows;
				}
				break;
			}
			case 's':
			case 'S':
			{
				if (cursor->m_position.cols < cursor->m_bounds.cols - 1)
				{
					++cursor->m_position.cols;
				}
				break;
			}
			case 'd':
			case 'D':
			{
				if (cursor->m_position.rows < cursor->m_bounds.rows - 1)
				{
					++cursor->m_position.rows;
				}
				break;
			}

			case 'f':
			case 'F':
			{
				std::visit([=](auto grid) {
					if constexpr (std::is_same_v<decltype(grid), Grid*>)
					{
						auto& pt = grid->m_points[cursor->m_position.cols][cursor->m_position.rows];

						if (pt.state == State::CLEARED) return; // Cannot flag cleared space

						pt.state = (pt.state == State::HIDDEN) ? State::FLAGGED : State::HIDDEN;
					}
				}, cursor->m_display);

				break;
			}

			case '\r': // Enter
			{
				

				break;
			}
		}

		struct Visitor
		{
			Cursor* cursor;

			void operator()(Grid* grid) const
			{
				grid->Display();
			}
		};

		std::visit(Visitor{ cursor }, cursor->m_display);
	}
}