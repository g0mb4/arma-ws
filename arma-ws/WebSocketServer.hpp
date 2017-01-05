#ifndef __WSSERVER_HPP__
#define __WSSERVER_HPP__

#ifdef _MSC_VER
	#define _WEBSOCKETPP_NOEXCEPT_
	#define _WEBSOCKETPP_CPP11_CHRONO_
	#define BOOST_ASIO_DISABLE_STD_CHRONO
	#pragma warning(disable:4996)
#endif

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <iostream>
#include <set>

#include <websocketpp/common/thread.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;
typedef server::connection_ptr connection_ptr;

using websocketpp::connection_hdl;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

using websocketpp::lib::thread;
using websocketpp::lib::mutex;
using websocketpp::lib::unique_lock;
using websocketpp::lib::condition_variable;

enum action_type {
	SUBSCRIBE,
	UNSUBSCRIBE,
	MESSAGE
};

struct action {
	action(action_type t, connection_hdl h) : type(t), hdl(h) {}
	action(action_type t, connection_hdl h, server::message_ptr m)
		: type(t), hdl(h), msg(m) {}
	action(action_type t, connection_hdl h, std::string m)
		: type(t), hdl(h), msg_str(m) {}

	action_type type;
	websocketpp::connection_hdl hdl;
	server::message_ptr msg;
	std::string msg_str;
};

class WebSocketServer {

public:
	WebSocketServer(std::queue<action>& _arma_actions_do, std::queue<action>& _arma_actions_done);
	~WebSocketServer(void);

	void run(uint16_t port);

	void on_open(connection_hdl hdl);
	void on_close(connection_hdl hdl);
	void on_message(connection_hdl hdl, server::message_ptr msg);

	void process_messages(void);

	void answer_clients(void);

private:
	typedef std::set<connection_hdl, std::owner_less<connection_hdl>> con_list;

	server _server;
	con_list _connections;

	std::queue<action> _server_actions;
	std::queue<action> &_arma_actions_do;
	std::queue<action> &_arma_actions_done;
	mutex _action_lock;
	mutex _connection_lock;
	condition_variable _action_cond;

	void _handle_server_messages(action a);
};

#endif