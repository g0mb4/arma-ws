#include "Logger.hpp"

Logger::Logger(std::string path)
{
	fs::path log_path(path);

	log_path /= "arma-ws.txt";

	_con = new DebugConsole(false);

	_file.open(log_path.string(), std::ofstream::out);

	if (!_file.is_open()) {
		_con->write("Unable to open the file");
	}
}

Logger::~Logger(void)
{
	_file.close();
}

void 
Logger::setDebugConsole(bool b)
{
	_con = new DebugConsole(b);
}

void 
Logger::write(std::string msg)
{
	_file << "[INFO] " << msg << "\n";

	_con->write("[INFO] " + msg + "\n\r");
}

void 
Logger::warning(std::string msg)
{
	_file << "[WARNING] " << msg << "\n";

	_con->write("[WARNING] " + msg + "\n\r");
}

void 
Logger::error(std::string msg)
{
	_file << "[ERROR] " << msg << "\n";

	_con->write("[ERROR] " + msg + "\n\r");
}
