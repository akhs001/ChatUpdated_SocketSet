#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <SDL.h>

namespace Utils {

	void draw();
	void lineBreak(int count);
	std::string space(int count);
	void clearConsole();
	void TypeWrite(const std::string& text, unsigned int delay);
	void Log(const std::string message);
	std::vector<std::string> Split(const std::string str, const char delimiter);
}
