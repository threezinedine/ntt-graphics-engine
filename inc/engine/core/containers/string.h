#ifndef _STRING_H_
#define _STRING_H_

#include "engine/core/result.h"
#include "engine/core/types.h"

#define NTT_SHORT_STRING_MAX_LENGTH 64

/**
 * An interface for working with the entire string as a single unit. There're 2 modes for storing the
 *      string: `short` vs `long`. If the length of the string is less than MAX_SHORT_STRING_LENGTH,
 *      the string is stored in the pBuffer, otherwise, it is stored in the pLongBuffer. This is an
 *      optimization technique called short string.
 * @note The string view uses the global malloc for memory management.
 */
struct ntt_String
{
	b8 isShortString; // This field is used for indicating whether the string is a short string or a long string, this
					  // is used for determining which buffer to use when accessing the string data.
	/**
	 * With the short string optimization, if the length < MAX_SHORT_STRING_LENGTH,
	 *      the string is stored in the pBuffer, otherwise, it is stored in the pLongBuffer.
	 */
	char pBuffer[NTT_SHORT_STRING_MAX_LENGTH];
	/**
	 * If the length of the string is greater than or equal to MAX_SHORT_STRING_LENGTH, the string
	 *      is stored in the pLongBuffer.
	 */
	char* pLongBuffer;
	usize length;
};

typedef struct ntt_String ntt_String;

/**
 * Be passed as the input arguments
 */
struct ntt_StringView
{
	const char* pBuffer;
	usize		length;
};

typedef struct ntt_StringView ntt_StringView;

DEFINE_RETURN_RESULT_TYPE(ntt_String)

/**
 * Creates a new string view from the given C-style string. The length of the string view is determined
 *      by the length of the C-style string, excluding the null terminator. The ownership of the c-string
 *      is not transferred to the string view.
 */
ntt_StringResult ntt_StringFromCString(const char* cString);

/**
 * Creates a new string view from the given format string and its corresponding arguments. The length of the
 *     string view is determined by the length of the formatted string, excluding the null terminator
 *     The ownership of the format string and its arguments is not transferred to the string view.
 */
ntt_StringResult ntt_StringFromFormat(const char* format, ...);

/**
 * Returns the number of the characters in the string view (not including the null terminator)
 */
usize ntt_StringLength(const ntt_String* stringView);

ntt_Result ntt_StringDestroy(ntt_String* stringView);

ntt_StringView ntt_stringToView(ntt_String* pStr);

#endif /* _STRING_H_ */