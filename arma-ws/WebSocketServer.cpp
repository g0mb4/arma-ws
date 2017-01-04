#include "WebSocketServer.hpp"

WebSocketServer::WebSocketServer(std::queue<action>& arma_actions_do, std::queue<action>& arma_actions_done)
	: _arma_actions_do(arma_actions_do), _arma_actions_done(arma_actions_done)
{
	_server.clear_access_channels(websocketpp::log::alevel::all);
	_server.clear_access_channels(websocketpp::log::elevel::all);
	_server.set_access_channels(websocketpp::log::elevel::rerror | websocketpp::log::elevel::fatal);

	_server.init_asio();

	_server.set_open_handler(bind(&WebSocketServer::on_open, this, ::_1));
	_server.set_close_handler(bind(&WebSocketServer::on_close, this, ::_1));
	_server.set_message_handler(bind(&WebSocketServer::on_message, this, ::_1, ::_2));
}

WebSocketServer::~WebSocketServer(void)
{
	_server.stop_listening();
	_server.stop();
}

void
WebSocketServer::run(uint16_t port)
{
	_server.listen(port);

	_server.start_accept();

	try {
		_server.run();
	}
	catch (const std::exception & e) {
		throw e;
		/* TODO : make it nicer */
	}
}

void
WebSocketServer::on_open(connection_hdl hdl)
{
	unique_lock<mutex> lock(_action_lock);
	_server_actions.push(action(SUBSCRIBE, hdl));
	lock.unlock();
	_action_cond.notify_one();
}

void
WebSocketServer::on_close(connection_hdl hdl)
{
	unique_lock<mutex> lock(_action_lock);
	_server_actions.push(action(UNSUBSCRIBE, hdl));
	lock.unlock();
	_action_cond.notify_one();
}

void
WebSocketServer::on_message(connection_hdl hdl, server::message_ptr msg)
{
	unique_lock<mutex> lock(_action_lock);
	_server_actions.push(action(MESSAGE, hdl, msg));
	lock.unlock();
	_action_cond.notify_one();
}

void 
WebSocketServer::answer_clients(void) 
{
	for (;;) {
		while (!_arma_actions_done.empty()) {

			try {
				action a = _arma_actions_done.front();
				_arma_actions_done.pop();

				_server.send(a.hdl, a.msg_str, websocketpp::frame::opcode::text);
			}
			catch (...) { 
				/* TODO : make it nicer */
			}
		}
	}
}

void 
WebSocketServer::process_messages(void)
{
	for(;;) {
		unique_lock<mutex> lock(_action_lock);

		while (_server_actions.empty()) {
			_action_cond.wait(lock);
		}

		action a = _server_actions.front();
		_server_actions.pop();

		lock.unlock();

		if (a.type == SUBSCRIBE) {
			unique_lock<mutex> con_lock(_connection_lock);
			_connections.insert(a.hdl);
		}
		else if (a.type == UNSUBSCRIBE) {
			unique_lock<mutex> con_lock(_connection_lock);
			_connections.erase(a.hdl);
		}
		else if (a.type == MESSAGE) {
			unique_lock<mutex> con_lock(_connection_lock);
			con_list::iterator it;
			bool found = false;

			int i = 0;
			for(it = _connections.begin(); it != _connections.end(); ++it) {

				/* TODO : prevent client to set up an action if another action of his is in the queue */
				_arma_actions_do.push(a);
			}
		}
		else {
			// undefined.
		}
	}
}
