#ifndef __EXTENSION_HPP__
#define __EXTENSION_HPP__

#pragma warning(disable:4996)

#include <string>
#include <fstream>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "Logger.hpp"
#include "WebSocketServer.hpp"

class Extension
{
public:
	Extension(std::string path);
	~Extension(void);

	void stop(void);

	void callExtension(char *output, const int &output_size, const char *function);

private:
	Logger *_log;
	WebSocketServer *_srv;

	boost::thread_group _threads;

	std::queue<action> _arma_actions_do;
	std::queue<action> _arma_actions_done;

	std::map<unsigned int, websocketpp::connection_hdl> _message_map;
	unsigned int _message_map_id = 0;

	boost::property_tree::ptree _property_tree;

	//settings
	int _port;
	bool _debugConsole;
	int _mode;
	std::string _sqfFileName;
	std::string _code;
};

#endif