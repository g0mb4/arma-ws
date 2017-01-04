#include "Extension.hpp"

Extension::Extension(std::string path) 
{
	_log = new Logger(path);

	fs::path ini_path(path);
	ini_path /= "arma-ws.ini";

	if (boost::filesystem::exists(ini_path)) {
		boost::property_tree::ini_parser::read_ini(ini_path.string(), _property_tree);

		_port = _property_tree.get<int>("General.Port");
		_debugConsole = _property_tree.get<bool>("General.DebugConsole");

		_log->write("Config file found: " + ini_path.string());
	}
	else {
		_log->warning("'arma-ws.ini' was not found! using default values");

		int _port = 9002;
		bool _debugConsole = true;
	}

	_log->setDebugConsole(_debugConsole);

	try {
		_srv = new WebSocketServer(_arma_actions_do, _arma_actions_done);

		_threads.create_thread(boost::bind(&WebSocketServer::process_messages, _srv));

		_threads.create_thread(boost::bind(&WebSocketServer::run, _srv, _port));

		_threads.create_thread(boost::bind(&WebSocketServer::answer_clients, _srv));

	} catch (websocketpp::exception const & ex) {
		_log->error(ex.what());
	} catch (const std::exception& ex) {
		_log->error(ex.what());
	}

	_log->write("Server is running.");
}

Extension::~Extension()
{
	/* TODO: something */
}

void 
Extension::stop(void)
{
	_threads.interrupt_all();
	_threads.join_all();
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
		else {
			std::strcpy(output, "nothing");
		}
	}
	/* result of the code 
	   !:ID:TYPE:VALUE
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

