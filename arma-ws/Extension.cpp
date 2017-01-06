#include "Extension.hpp"

Extension::Extension(std::string path) 
{
	_log = new Logger(path);

	fs::path ini_path(path);
	ini_path /= "arma-ws.ini";

	_port = 9002;
	_debugConsole = true;
	_mode = 0;

	if (boost::filesystem::exists(ini_path)) {
		_log->write("Config file found: " + ini_path.string());
		boost::property_tree::ini_parser::read_ini(ini_path.string(), _property_tree);

		_port = _property_tree.get<int>("General.Port");
		_debugConsole = _property_tree.get<bool>("General.DebugConsole");
		_mode = _property_tree.get<int>("General.Mode");

		if (_mode == BROADCAST) {
			_sqfFileName = _property_tree.get<std::string>("Broadcast.Code");
			fs::path code_path(path);
			code_path /= _sqfFileName;

			if (boost::filesystem::exists(code_path)) {
				boost::filesystem::ifstream sqfFile;

				sqfFile.open(code_path.string(), std::ofstream::in);

				if (sqfFile.is_open()) {
					std::string str;

					while (std::getline(sqfFile, str))
					{
						_code += str;
					}

					_log->write("Code loaded from " + code_path.string());
				} else {
					_log->error("Unable to open the code file: " + code_path.string());
					_mode = COMMAND;
				}

			} else {
				_log->error("Unable to find the code file: " + code_path.string());
				_mode = COMMAND;
			}
		}
	} else {
		_log->warning("'arma-ws.ini' was not found! using default values");
	}

	_log->setDebugConsole(_debugConsole);

	try {
		_srv = new WebSocketServer(_mode, _arma_actions_do, _arma_actions_done);

		_threads.create_thread(boost::bind(&WebSocketServer::process_messages, _srv));

		_threads.create_thread(boost::bind(&WebSocketServer::run, _srv, _port));

		_threads.create_thread(boost::bind(&WebSocketServer::answer_clients, _srv));

	} catch (websocketpp::exception const & ex) {
		_log->error(ex.what());
	} catch (const std::exception& ex) {
		_log->error(ex.what());
	}

	_log->write("Port: " + std::to_string(_port));
	_log->write("Mode: " + std::to_string(_mode));

	if(_mode == BROADCAST)
		_log->write("Code: " + _sqfFileName);

	_log->write("Server is running.");
}

Extension::~Extension()
{
	_log->write("Server exited.");
}

void 
Extension::stop(void)
{
	_threads.interrupt_all();
	_threads.join_all();
	_log->write("Server stopped.");
}

void 
Extension::callExtension(char *output, const int &output_size, const char *function)
{
	boost::char_separator<char> sep(":");
	typedef boost::tokenizer<boost::char_separator<char>> t_tokanizer;

	std::string function_str(function);

	t_tokanizer	tok(function_str, sep);

	std::vector<std::string> tokens;

	for (t_tokanizer::iterator beg = tok.begin(); beg != tok.end(); ++beg) {
		tokens.push_back(*beg);
	}

	/* ping 
	   ?:
	*/
	if (tokens.size() == 1) {
		if (tokens[0] == "?") {
			// COMMAND
			if (_mode == COMMAND) {
				if (_arma_actions_do.empty()) {
					std::strcpy(output, "nothing");
				}
				else {
					action a = _arma_actions_do.front();
					_arma_actions_do.pop();

					_message_map[_message_map_id] = a.hdl;

					std::ostringstream oss;

					oss << _message_map_id << ":" << a.msg->get_payload();

					std::strcpy(output, oss.str().c_str());

					_message_map_id++;
				}
			}
			// BROADCAST
			else if (_mode == BROADCAST) {
				std::strcpy(output, _code.c_str());
			}
		}
		else {
			std::strcpy(output, "nothing");
		}
	}
	/* result of the code 
	   !:MESSAGE_ID:RETRUN_TYPE:VALUE
	*/
	else if (tokens.size() == 4) {
		if (tokens[0] == "!") {
			unsigned int id = boost::lexical_cast<unsigned int>(tokens[1]);

			std::ostringstream data;
			data << tokens[2] << ":" << tokens[3];

			_arma_actions_done.push(action(MESSAGE, _message_map[id], data.str()));

			_message_map.erase(id);
		}
	}
}

