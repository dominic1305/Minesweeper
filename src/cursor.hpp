#pragma once

#include <thread>
#include <variant>

#include "vec2.hpp"
#include "grid.hpp"

class Cursor
{
private:

	friend class Grid;

public:

						Cursor(vec2 dim, std::variant<Grid*> display);

						Cursor(const Cursor&)		= delete;
	Cursor&				operator=(const Cursor&)	= delete;

						Cursor(Cursor&&)			= delete;
	Cursor&				operator=(Cursor&&)			= delete;

	void				Resize(vec2 new_bounds, vec2 new_pos);

	void				UpdateDisplay(std::variant<Grid*> display);

private:

	static void			KeyboardListener(Cursor* const cursor);

	vec2				m_bounds;
	vec2				m_position;
	std::jthread		m_thread;
	char				m_pressedChar;
	std::atomic_bool	m_threadActive;

	std::variant<Grid*>	m_display;

};