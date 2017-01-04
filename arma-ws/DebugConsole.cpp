#include "DebugConsole.hpp"

DebugConsole::DebugConsole(bool enabled)
{
	_enabled = enabled;

	if (_enabled) {
		AllocConsole();
		AttachConsole(GetCurrentProcessId());
		FILE *stream;
		freopen_s(&stream, "CON", "w", stdout);
		SetConsoleTitle("Debug Console for ARMA-WebSocket");

		write(" Debug Console for ARMA-WebSocket by gmb\n\n", YELLOW, BLUE);
	}
}

DebugConsole::~DebugConsole(void)
{
	if (_enabled) {
		FreeConsole();

		HWND hwnd = FindWindow(NULL, TEXT("Debug Console for ARMA-WebSocket"));

		PostMessage(hwnd, WM_CLOSE, 0, 0);
	}
}

void 
DebugConsole::write(std::string msg, unsigned char foregroundColor, unsigned char backgroundColor)
{
	if (_enabled) {
		setColor(foregroundColor, backgroundColor);
		std::cout << msg;
	}
}


void 
DebugConsole::setColor(unsigned char foreground, unsigned char background)
{
	if (_enabled)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (foreground | (background << 4)));
}