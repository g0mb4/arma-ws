## arma-ws v0.2

This is a simple addon, that will launch a WebSocket server, recieves/sends data between an external WS app and the ARMA engine.

## Installation
- copy "example/@arma-ws" directory to your ARMA root, then add "@arma-ws" to the "-mod" launch parameter
- include (and/or modify) the content of the "example/sqf" directory in your mission

check if it's working: launch the "example/html/arma-ws.html" and you can see your player's coordinates at live

## Motivation

This is my first external lib for ARMA and I think it's fun to connect ARMA to the outer world.

## Compilation

- get "boost" (http://www.boost.org/users/history/version_1_63_0.html)
- get "websocketpp" (https://github.com/zaphoyd/websocketpp)

use the attached solution files (currently working only on Windows, compiled with VS2015)

## Known errors

- crashes when the game is paused (use -noPause and Alt+Tab NOT Esc)

## Thanks to

- the developers of "boost"
- the developer of "websocketpp"
- AsYetUnited for the source of extDB2 (https://github.com/AsYetUntitled/extDB2)
- killzonekid fot the beautiful tutorials (http://killzonekid.com)

## License

Use, modify at your will, just please mention me at the end of your README :)
