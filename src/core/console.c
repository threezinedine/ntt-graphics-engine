#include "engine/core/console.h"
#include <stdarg.h>
#include <stdio.h>

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
#error "Windows platform is not supported yet."
#else
#error "Unknown platform."
#endif
}

void ntt_ConsoleResetColor()
{
#if NTT_PLATFORM_UNIX
	printf("\033[0m");
#elif NTT_PLATFORM_WINDOWS
#error "Windows platform is not supported yet."
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