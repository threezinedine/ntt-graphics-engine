#ifndef _DISPLAY_DRIVER_TYPE_H_
#define _DISPLAY_DRIVER_TYPE_H_

enum ntt_DisplayDriverType
{
#define ENUM_OPTION(option)		NTT_DISPLAY_DRIVER_##option,
#define ENUM_OPTION_END(option) NTT_DISPLAY_DRIVER_##option
#include "display_driver_type.inl"
#undef ENUM_OPTION
#undef ENUM_OPTION_END
};

typedef enum ntt_DisplayDriverType ntt_DisplayDriverType;

const char* ntt_DisplayDriverType_ToString(ntt_DisplayDriverType type);

#endif /* _DISPLAY_DRIVER_TYPE_H_ */