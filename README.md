## arma-ws v0.4

This is a simple addon, that will launch a WebSocket server, recieves/sends data between an external WS app and the ARMA engine.

This mod is basically a .DLL/.SO file.

## Modes

- COMMAND (different commands for clients):
 1. client sends commands to the server
 2. ARMA gets the commands from the server
 3. ARMA executes the commands
 4. ARMA sends the result back to the server
 5. server sends the result to the client
 6. goto 1.

- BROADCAST (same commands for all clients):
 1. server reads the commands at the start
 2. ARMA gets the commands from the server
 3. ARMA executes the commands
 4. ARMA sends the result back to the server
 5. server sends the result to all clients
 6. goto 3.

## Installation

- copy "example/@arma-ws" directory to your ARMA root, then add "@arma-ws" to the "-mod" launch parameter
- include (and/or modify) the content of the "example/sqf/<mode>" directory in your mission

check if it's working:
- launch the "example/html/<mode>/arma-ws.html" and you can see your player's coordinates at live

## Configuration

modify the 'arma-ws.ini'

## Motivation

This is my first external lib for ARMA and I think it's fun to connect ARMA to the outer world.

## Compilation

- get "boost" (http://www.boost.org/users/history/version_1_63_0.html)

set BOOST_ROOT enviromental variable
e.g.:
```
Windows:
set BOOST_ROOT=n:\prog\C++\prog\boost_1_63_0

Linux:
export BOOST_ROOT=/home/gmb/prog/c++/boost_1_63_0
```
set BOOST_LIBRARYDIR enviromental variable
e.g.:
```
Windows:
set BOOST_LIBRARYDIR=n:\prog\C++\prog\boost_1_63_0\lib32-msvc-14.0

Linux:
export BOOST_LIBRARYDIR=/home/gmb/prog/c++/boost_1_63_0/stage/lib
```

- get "websocketpp" (https://github.com/zaphoyd/websocketpp)

set WEBSOCKETPP_ROOT enviromental variable
e.g.:
```
Windows:
set WEBSOCKETPP_ROOT=n:\prog\C++\prog\websocketpp

Linux:
export WEBSOCKETPP_ROOT=/home/gmb/prog/c++/websocketpp
```

in linux (bash) type (I'll work on it):
```
alias g++="g++ --std=c++0x"
```

then
```
cd build
cmake ..

Windows:
open the generated arma-ws.sln

Linux:
make
```

## Known errors

- crashes when the game is paused (use -noPause and Alt+Tab NOT Esc)

## Thanks to

- the developers of boost (http://www.boost.org)
- zaphoyd for the source of websocketpp (https://github.com/zaphoyd/websocketpp)
- AsYetUnited for the source of extDB2 (https://github.com/AsYetUntitled/extDB2)
- killzonekid for the beautiful tutorials (http://killzonekid.com)

## License

Use, modify at your will, just please mention me at the end of your README :)
