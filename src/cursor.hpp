#pragma once

#include <thread>
#include <variant>

#include "dimensions.hpp"
#include "grid.hpp"

class Cursor
{
private:

	friend class Grid;

public:

						Cursor(Dimensions dim, std::variant<Grid*> display);

						Cursor(const Cursor&) = delete;
	Cursor&				operator=(const Cursor&) = delete;

						Cursor(Cursor&& other) noexcept;
	Cursor&				operator=(Cursor&& other) noexcept;

private:

	static void			KeyboardListener(std::stop_token stoken, Cursor* const cursor);

	Dimensions			m_bounds;
	Dimensions			m_position;
	std::jthread		m_thread;

	std::variant<Grid*>	m_display;

};