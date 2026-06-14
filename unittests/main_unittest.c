#include "engine/engine.h"
#include "tools/tools.h"
#include <string.h> // TODO: use self-implemented string functions later

int	   g_argc;
char** g_argv;
b8	   g_currentTestResult		 = TRUE;
char   g_currentTestMessage[256] = {0};
usize  g_currentTestMessageSize	 = sizeof(g_currentTestMessage);

#include "core_unittests/core_unittests.h"

int main(i32 argc, char** argv)
{
	g_argc = argc;
	g_argv = argv;

	u64 totalTestsCount	 = 0;
	u64 passedTestsCount = 0;

	RUN_TEST_SUITE(id_tests);

	const char* testResultMessage = "*********** TEST RESULT ***********";
	i32			width			  = (i32)strlen(testResultMessage); // TODO: use self-implemented strlen later

	if (totalTestsCount == passedTestsCount)
	{
		ntt_ConsoleSetColor(NTT_COLOR_GREEN);
		ntt_ConsolePrint("%s\n", testResultMessage);
		ntt_ConsolePrint("*        All tests passed!        *\n");
	}
	else
	{
		char resultMessage[256];
		ntt_FormatMessage(
			resultMessage, sizeof(resultMessage), "Some tests failed! (%llu/%llu)", passedTestsCount, totalTestsCount);

		i32 leftPadding = (width - 2 - (i32)strlen(resultMessage)) / 2; // TODO: use self-implemented strlen later
		i32 rightPadding =
			width - 3 - leftPadding - (i32)strlen(resultMessage); // TODO: use self-implemented strlen later

		ntt_ConsoleSetColor(NTT_COLOR_RED);
		ntt_ConsolePrint("%s\n", testResultMessage);
		ntt_ConsolePrint("*%*s", leftPadding + (i32)strlen(resultMessage), resultMessage);
		ntt_ConsolePrint("%*s*\n", rightPadding + 1, ""); // +1 for the '*' at the end
	}
	ntt_ConsolePrint("***********************************\n\n");

	return 0;
}