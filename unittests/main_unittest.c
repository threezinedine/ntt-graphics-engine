#include "engine/engine.h"
#include "tools/tools.h"

int			g_argc;
char**		g_argv;
b8			g_currentTestResult		  = TRUE;
char		g_currentTestMessage[256] = {0};
usize		g_currentTestMessageSize  = sizeof(g_currentTestMessage);
const char* g_currentTestCase		  = NULL;

#include "core_unittests/core_unittests.h"

/**
 * Helper for beautifying the test result message.
 *
 * @param pBuffer The buffer to store the centered content, this should be pre-allocated by the caller, and its size
 *      should be specified by the bufferSize parameter.
 * @param bufferSize The size of the buffer, this is used for preventing buffer overflow when writing to the buffer.
 * @param pSource The source content to be centered, this is the content that will be centered and written to
 *      the buffer.
 * @param sourceSize The size of the source content, this is used for calculating the padding needed for centering
 *      the content.
 * @param expectedLength The expected length of the content to be centered.
 *
 * @example
 * ```
 * // "Some tests failed! (3/5)" is 34 characters long, so the expected length is 50
 * // ===> "           Some tests failed! (3/5)           "
 * ```
 */
void centerContent(char* pBuffer, usize bufferSize, char* pSource, usize sourceSize, usize expectedLength);

int main(i32 argc, char** argv)
{
	g_argc = argc;
	g_argv = argv;

	u64 totalTestsCount	 = 0;
	u64 passedTestsCount = 0;

	RUN_TEST_SUITE(id);
	RUN_TEST_SUITE(memory);
	RUN_TEST_SUITE(string);
	RUN_TEST_SUITE(array);
	RUN_TEST_SUITE(list);
	RUN_TEST_SUITE(queue);
	RUN_TEST_SUITE(stack);
	RUN_TEST_SUITE(map);
	RUN_TEST_SUITE(object);

	const char* testResultMessage		   = "************ TEST RESULT ***********";
	i32			width					   = (i32)ntt_StrLen(testResultMessage);
	char		resultMessage[256]		   = {0};
	char		centeredResultMessage[512] = {0};

	if (totalTestsCount == passedTestsCount)
	{
		ntt_FormatMessage(
			resultMessage, sizeof(resultMessage), "All tests passed! (%llu/%llu)", passedTestsCount, totalTestsCount);

		centerContent(centeredResultMessage,
					  sizeof(centeredResultMessage),
					  resultMessage,
					  ntt_StrLen(resultMessage),
					  (usize)width - 2); // TODO: use self-implemented strlen later

		ntt_ConsoleSetColor(NTT_COLOR_GREEN);
		ntt_ConsolePrint("%s\n", testResultMessage);
		ntt_ConsolePrint("*%s*\n", centeredResultMessage);
	}
	else
	{
		ntt_FormatMessage(
			resultMessage, sizeof(resultMessage), "Some tests failed! (%llu/%llu)", passedTestsCount, totalTestsCount);

		centerContent(centeredResultMessage,
					  sizeof(centeredResultMessage),
					  resultMessage,
					  ntt_StrLen(resultMessage),
					  (usize)width - 2);

		ntt_ConsoleSetColor(NTT_COLOR_RED);
		ntt_ConsolePrint("%s\n", testResultMessage);
		ntt_ConsolePrint("*%s*\n", centeredResultMessage);
	}
	ntt_ConsolePrint("************************************\n\n");

	return 0;
}

void centerContent(char* pBuffer, usize bufferSize, char* pSource, usize sourceSize, usize expectedLength)
{
	NTT_ASSERT(pBuffer != NULL);
	NTT_ASSERT(pSource != NULL);

	i32 leftPadding	 = (i32)(expectedLength - sourceSize) / 2;
	i32 rightPadding = (i32)(expectedLength - sourceSize) - leftPadding;

	ntt_FormatMessage(pBuffer, bufferSize, "%*s%s%*s", leftPadding, "", pSource, rightPadding, "");
}