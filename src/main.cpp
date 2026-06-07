#include <iostream>

#include "render.hpp"
#include "grid.hpp"
#include "cursor.hpp"

int main()
{
	Grid grid({ 10, 10 });
	Cursor cursor({ 10, 10 }, &grid);

	grid.GiveCursor(&cursor);

	return 0;
}