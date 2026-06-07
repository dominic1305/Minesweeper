#include <iostream>

#include "render.hpp"
#include "grid.hpp"
#include "cursor.hpp"

int main()
{
	Grid grid({ 10, 10 });
	Cursor cursor({ 10, 10 }, &grid);

	grid.GiveCursor(&cursor);

	grid.Display();

	std::this_thread::sleep_for(std::chrono::seconds(60));

	return 0;
}