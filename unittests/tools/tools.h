#ifndef _TOOLS_H_
#define _TOOLS_H_

#include "engine/engine.h"

/**
 * Usage:
 * ```c
 * TEST_CASE("Test case name")
 * {
 *   // Arrange
 *   // Act
 *   // Assert
 *   ASSERT(3 == 4);
 * }
 * ```
 */

/**
 * Store the information of a test case
 */
struct ntt_TestCase
{
	const char* name;
	void (*testFunc)();
};

typedef struct ntt_TestCase ntt_TestCase;

#define TEST_CASE(name) void test_##name()

#define TEST_ASSERT(condition)                                                                                         \
	do                                                                                                                 \
	{                                                                                                                  \
		if (!(condition))                                                                                              \
		{                                                                                                              \
			g_currentTestResult = FALSE;                                                                               \
			ntt_FormatMessage(g_currentTestMessage, g_currentTestMessageSize, "Assertion failed: %s", #condition);     \
			return;                                                                                                    \
		}                                                                                                              \
	} while (0)

#define TEST_ASSERT_M(condition, message, ...)                                                                         \
	do                                                                                                                 \
	{                                                                                                                  \
		if (!(condition))                                                                                              \
		{                                                                                                              \
			g_currentTestResult = FALSE;                                                                               \
			ntt_FormatMessage(g_currentTestMessage, g_currentTestMessageSize, message, ##__VA_ARGS__);                 \
			return;                                                                                                    \
		}                                                                                                              \
	} while (0)

#define TEST_SUITE_DEFINE(testSuit, beforeEach, afterEach, ...)                                                        \
	void run_##testSuit##_tests(u64* pTestsCount, u64* pTestsPassed)                                                   \
	{                                                                                                                  \
		ntt_TestCase testCases[] = {__VA_ARGS__};                                                                      \
		u64			 testsCount	 = (u64)(sizeof(testCases) / sizeof(ntt_TestCase));                                    \
		*pTestsCount			 = *pTestsCount + testsCount;                                                          \
		u64 testsPassed			 = 0;                                                                                  \
		for (u64 i = 0; i < testsCount; i++)                                                                           \
		{                                                                                                              \
			beforeEach();                                                                                              \
			g_currentTestResult = TRUE;                                                                                \
			ntt_Memset(g_currentTestMessage, 0, g_currentTestMessageSize);                                             \
			g_currentTestMessageSize = sizeof(g_currentTestMessage);                                                   \
			ntt_ConsoleSetColor(NTT_COLOR_GREEN);                                                                      \
			ntt_ConsolePrint("##########################\n");                                                          \
			ntt_ConsolePrint("[ ==> ] Running test: %s\n\t", testCases[i].name);                                       \
			testCases[i].testFunc();                                                                                   \
			if (g_currentTestResult)                                                                                   \
			{                                                                                                          \
				testsPassed++;                                                                                         \
				ntt_ConsolePrint("[PASS]\n");                                                                          \
			}                                                                                                          \
			else                                                                                                       \
			{                                                                                                          \
				ntt_ConsoleSetColor(NTT_COLOR_RED);                                                                    \
				ntt_ConsolePrint("[FAIL] %s\n", g_currentTestMessage);                                                 \
			}                                                                                                          \
			ntt_ConsoleSetColor(NTT_COLOR_GREEN);                                                                      \
			ntt_ConsolePrint("##########################\n\n");                                                        \
			afterEach();                                                                                               \
		}                                                                                                              \
		*pTestsPassed = *pTestsPassed + testsPassed;                                                                   \
	}

#define RUN_TEST_SUITE(name)                                                                                           \
	do                                                                                                                 \
	{                                                                                                                  \
		run_##name##_tests(&totalTestsCount, &passedTestsCount);                                                       \
	} while (0)

#define TEST_CASE_DECLARE(testCase) {.name = #testCase, .testFunc = test_##testCase}

#endif /* _TOOLS_H_ */