#ifndef __DEBUGCONSOLE_HPP__
#define __DEBUGCONSOLE_HPP__

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <iostream>
#include <string>

#ifdef _MSC_VER
	#pragma warning(disable:4996)

	#define BLACK			0
	#define BLUE			1
	#define GREEN			2
	#define CYAN			3
	#define RED				4
	#define MAGENTA			5
	#define BROWN			6
	#define LIGHTGRAY		7
	#define DARKGRAY		8
	#define LIGHTBLUE		9
	#define LIGHTGREEN		10
	#define LIGHTCYAN		11
	#define LIGHTRED		12
	#define LIGHTMAGENTA	13
	#define YELLOW			14
	#define WHITE			15
#endif

class DebugConsole
{
public:
	DebugConsole(bool enabled);
	~DebugConsole(void);

	void write(std::string msg, unsigned char foregroundColor = WHITE, unsigned char backgroundColor = BLACK);

private:
	bool _enabled;
	void setColor(unsigned char foreground, unsigned char background);
};

#endif