#include "engine/core/utils/console.h"
#include <stdarg.h>
#include <stdio.h>

#if NTT_PLATFORM_WINDOWS
#include <Windows.h>
#endif /* NTT_PLATFORM_WINDOWS */

void ntt_ConsoleSetColor(enum ntt_Color color)
{
#if NTT_PLATFORM_UNIX
	switch (color)
	{
	case NTT_COLOR_BLACK:
		printf("\033[30m");
		break;
	case NTT_COLOR_RED:
		printf("\033[31m");
		break;
	case NTT_COLOR_GREEN:
		printf("\033[32m");
		break;
	case NTT_COLOR_YELLOW:
		printf("\033[33m");
		break;
	case NTT_COLOR_BLUE:
		printf("\033[34m");
		break;
	case NTT_COLOR_MAGENTA:
		printf("\033[35m");
		break;
	case NTT_COLOR_CYAN:
		printf("\033[36m");
		break;
	case NTT_COLOR_WHITE:
		printf("\033[37m");
		break;
	case NTT_COLOR_GRAY:
		printf("\033[90m");
		break;
	default:
		break;
	}
#elif NTT_PLATFORM_WINDOWS
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	int icolor;

	switch (color) {
	case NTT_COLOR_BLACK:
		icolor = 0;
		break;
	case NTT_COLOR_RED:
		icolor = FOREGROUND_RED;
		break;
	case NTT_COLOR_GREEN:
		icolor = FOREGROUND_GREEN;
		break;
	case NTT_COLOR_YELLOW:
		icolor = FOREGROUND_RED | FOREGROUND_GREEN;
		break;
	case NTT_COLOR_BLUE:
		icolor = FOREGROUND_BLUE;
		break;
	case NTT_COLOR_MAGENTA:
		icolor = FOREGROUND_RED | FOREGROUND_BLUE;
		break;
	case NTT_COLOR_CYAN:
		icolor = FOREGROUND_GREEN | FOREGROUND_BLUE;
		break;
	case NTT_COLOR_WHITE:
		icolor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		break;
	case NTT_COLOR_GRAY:
		icolor = FOREGROUND_INTENSITY;
		break;
	default:
		icolor = 7; // Default color (light gray)
		break;
	}

	SetConsoleTextAttribute(hConsole, (WORD)icolor);
#else
#error "Unknown platform."
#endif
}

void ntt_ConsoleResetColor()
{
#if NTT_PLATFORM_UNIX
	printf("\033[0m");
#elif NTT_PLATFORM_WINDOWS
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 7); // 7 is the default color (light gray)
#else
#error "Unknown platform."
#endif
}

void ntt_ConsolePrint(const char* message, ...)
{
	va_list args;
	va_start(args, message);
	vprintf(message, args);
	va_end(args);
}

void ntt_FormatMessage(char* pBuffer, usize bufferSize, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vsnprintf(pBuffer, bufferSize, format, args);
	va_end(args);
}