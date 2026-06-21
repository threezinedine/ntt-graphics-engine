#ifndef _LOGGING_UNITTEST_H_
#define _LOGGING_UNITTEST_H_

#include "engine/engine.h"
#include "tools/tools.h"
#include <string.h>

static ntt_LoggingMessage s_testMessage;
static const char*		  s_fileName   = "test_file.cpp";
static const char*		  s_logMessage = "This is a test log message.";

void logging_run_before_each_test()
{
	s_testMessage.type	= LOGGING_TYPE_CORE;
	s_testMessage.level = LOGGING_LEVEL_INFO;
	ntt_GetCurrentTime(&s_testMessage.timestamp);
	memcpy(s_testMessage.file, s_fileName, strlen(s_fileName) + 1);
	s_testMessage.line = 42;
	memcpy(s_testMessage.message, s_logMessage, strlen(s_logMessage) + 1);
	memset(s_testMessage.finalMessage, 0, sizeof(s_testMessage.finalMessage));
}

void logging_run_after_each_test()
{
}

TEST_CASE(FormatMessage_Level)
{
	ntt_LoggingMessage_FormatMessage(&s_testMessage, "[%(level)]");
	TEST_ASSERT(strcmp(s_testMessage.finalMessage, "[INFO]") == 0);
}

TEST_CASE(FormatMessage_Type)
{
	ntt_LoggingMessage_FormatMessage(&s_testMessage, "[%(type)");
	TEST_ASSERT(strcmp(s_testMessage.finalMessage, "[CORE") == 0);
}

TEST_CASE(FormatMessage_File)
{
	ntt_LoggingMessage_FormatMessage(&s_testMessage, "%(file):%(line)");
	TEST_ASSERT(strcmp(s_testMessage.finalMessage, "test_file.cpp:42") == 0);
}

TEST_SUITE_DEFINE(logging,
				  logging_run_before_each_test,
				  logging_run_after_each_test,
				  TEST_CASE_DECLARE(FormatMessage_Level),
				  TEST_CASE_DECLARE(FormatMessage_File),
				  TEST_CASE_DECLARE(FormatMessage_Type))

#endif /* _LOGGING_UNITTEST_H_ */