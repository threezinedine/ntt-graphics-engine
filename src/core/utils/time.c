#include "engine/core/utils/time.h"
#include <time.h>

void ntt_GetCurrentTime(struct ntt_Time* pTime)
{
	time_t	   rawTime;
	struct tm* timeInfo;

	time(&rawTime);
	timeInfo	  = localtime(&rawTime);
	pTime->year	  = timeInfo->tm_year + 1900;
	pTime->month  = timeInfo->tm_mon + 1;
	pTime->day	  = timeInfo->tm_mday;
	pTime->hour	  = timeInfo->tm_hour;
	pTime->minute = timeInfo->tm_min;
	pTime->second = timeInfo->tm_sec;
}