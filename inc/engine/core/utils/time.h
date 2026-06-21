#ifndef _NTT_TIME_H
#define _NTT_TIME_H

#include "engine/core/result_type.h"
#include "engine/core/types.h"

/**
 * The custom time structure for the engine where as the standard time information will be saved.
 */
struct ntt_Time
{
	u16 year;	///< Format like dddd (example: 2024)
	u8	month;	///< Format like mm (example: 01, 02, ..., 12)
	u8	day;	///< Format like dd (example: 01, 02, ..., 31)
	u8	hour;	///< Format like hh (example: 00, 01, ..., 23)
	u8	minute; ///< Format like mm (example: 00, 01, ..., 59)
	u8	second; ///< Format like ss (example: 00, 01, ..., 59)
};

typedef struct ntt_Time ntt_Time;

/**
 * Get the current time information and save it into the provided time structure. The behavior of this function depends
 * on the specific time implementation, but it generally retrieves the current time information from the system and
 *      saves it into the provided time structure for later use.
 *
 * @param pTime The pointer to the time structure where the current time information will be saved. This parameter can
 *      not be NULL or trigger an assertion failure.
 */
ntt_Result ntt_GetCurrentTime(ntt_Time* pTime);

#endif /* _NTT_TIME_H */
