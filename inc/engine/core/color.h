#ifndef _COLOR_H_
#define _COLOR_H_

/**
 * Common predefined colors which can be used throughout the whole engine, this enum is useful in multiple systems,
 *      such as the console system, the rendering system, etc. The specific color values and their representation
 *      can be, but there's another color type like ntt_RGBColor, ntt_RGBAColor, etc. which can be used for more
 *      specific purposes, such as the rendering system, but this enum is more general and can be used in various
 *      contexts where a simple color representation is needed.
 */
enum ntt_Color
{
#define ENUM_OPTION(option)	  option,
#define ENUM_OPTION_END(name) name,
#include "color.inl"
#undef ENUM_OPTION
#undef ENUM_OPTION_END
};

const char* ntt_ColorToString(enum ntt_Color color);

#endif /* _COLOR_H_ */