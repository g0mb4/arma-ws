#ifndef __LOGGER_HPP__
#define __LOGGER_HPP__

#include <iostream>
#include <string>
#include <fstream>

#include <boost/filesystem/fstream.hpp>

#include "DebugConsole.hpp"

#pragma warning(disable:4996)

namespace fs = boost::filesystem;

class Logger {
public :
	Logger(std::string path);
	~Logger(void);

	void setDebugConsole(bool b);

	void write(std::string msg);

	void warning(std::string msg);

	void error(std::string msg);

private :
	fs::ofstream _file;
	DebugConsole *_con;
};

#endif