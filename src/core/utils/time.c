#include "engine/core/utils/time.h"
#include <time.h>

void ntt_GetCurrentTime(struct ntt_Time* pTime)
{
	time_t	   rawTime;
	time(&rawTime);
	struct tm timeInfo;
	localtime_s(&timeInfo, &rawTime);

	pTime->year	  = (u16)timeInfo.tm_year + 1900;
	pTime->month  = (u8)timeInfo.tm_mon + 1;
	pTime->day	  = (u8)timeInfo.tm_mday;
	pTime->hour	  = (u8)timeInfo.tm_hour;
	pTime->minute = (u8)timeInfo.tm_min;
	pTime->second = (u8)timeInfo.tm_sec;
}