#include "render.hpp"

std::string Colour::operator()() const
{
	return "38;2;"
		+ std::to_string(static_cast<int>(fg.red))
		+ ";"
		+ std::to_string(static_cast<int>(fg.green))
		+ ";"
		+ std::to_string(static_cast<int>(fg.blue))
		+ ";48;2;"
		+ std::to_string(static_cast<int>(bg.red))
		+ ";"
		+ std::to_string(static_cast<int>(bg.green))
		+ ";"
		+ std::to_string(static_cast<int>(bg.blue));
}

std::string Renderer::operator()(const std::string& str) const
{
	return "\x1b[" + colour() + "m" + str + "\x1b[0m";
}