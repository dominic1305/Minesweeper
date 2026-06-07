#include <iostream>
#include <conio.h>
#include <queue>

#include "cursor.hpp"

Cursor::Cursor(vec2 dim, std::variant<Grid*> display)
	: m_bounds(dim)
	, m_position({ 0, 0 })
	, m_display(display)
	, m_pressedChar('\x0')
	, m_threadActive(true)
{
	m_thread = std::jthread{ KeyboardListener, this };
}

void Cursor::Resize(vec2 new_bounds, vec2 new_pos)
{
	m_bounds = new_bounds;
	m_position = new_pos;
}

void Cursor::UpdateDisplay(std::variant<Grid*> display)
{
	m_display = display;
}

void Cursor::KeyboardListener(Cursor* const cursor)
{
	while (cursor->m_threadActive)
	{
		if (!_kbhit()) continue;
		cursor->m_pressedChar = _getch();

		switch (cursor->m_pressedChar)
		{
			case 'w':
			case 'W':
			{
				if (cursor->m_position.y > 0)
				{
					--cursor->m_position.y;
				}
				break;
			}
			case 'a':
			case 'A':
			{
				if (cursor->m_position.x > 0)
				{
					--cursor->m_position.x;
				}
				break;
			}
			case 's':
			case 'S':
			{
				if (cursor->m_position.y < cursor->m_bounds.y - 1)
				{
					++cursor->m_position.y;
				}
				break;
			}
			case 'd':
			case 'D':
			{
				if (cursor->m_position.x < cursor->m_bounds.x - 1)
				{
					++cursor->m_position.x;
				}
				break;
			}

			case 'f':
			case 'F':
			{
				std::visit([=](auto grid) {
					if constexpr (std::is_same_v<decltype(grid), Grid*>)
					{
						auto& pt = grid->m_points[cursor->m_position.y][cursor->m_position.x];

						if (pt.state == State::CLEARED) return; // Cannot flag cleared space

						pt.state = (pt.state == State::HIDDEN) ? State::FLAGGED : State::HIDDEN;
					}
				}, cursor->m_display);

				break;
			}

			case ' ':
			{
				std::visit([=](auto grid) {
					if constexpr (std::is_same_v<decltype(grid), Grid*>)
					{
						std::queue<vec2> toProcess;
						toProcess.push(cursor->m_position); // Starting position

						while (!toProcess.empty())
						{
							vec2 pos = toProcess.front();
							toProcess.pop();

							GridPoint& pt = (*grid)[pos];

							if (pt.state == State::CLEARED) continue;
							pt.state = State::CLEARED;

							if (pt.bombProx != 0) continue; // Bombs are nearby, don't extend further

							std::array<GridPoint*, 8> surround = grid->GetSurroundingPoints(pt.position);
							for (GridPoint* point : surround)
							{
								if (point != nullptr && point->state != State::CLEARED)
								{
									toProcess.push(point->position);
								}
							}
						}
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