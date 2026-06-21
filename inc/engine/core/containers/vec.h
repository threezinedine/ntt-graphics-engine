#ifndef _VEC_H_
#define _VEC_H_

#include "engine/core/types.h"

#define VEC2_DECLARE(type, abbrev)                                                                                     \
	typedef union {                                                                                                    \
		struct                                                                                                         \
		{                                                                                                              \
			type x;                                                                                                    \
			type y;                                                                                                    \
		};                                                                                                             \
		type data[2];                                                                                                  \
	} ntt_Vec2##abbrev;                                                                                                \
	typedef ntt_Vec2##abbrev ntt_Vec2##abbrev;                                                                         \
	ntt_Vec2##abbrev		 ntt_Vec2##abbrev##_Add(ntt_Vec2##abbrev a, ntt_Vec2##abbrev b);                           \
	ntt_Vec2##abbrev		 ntt_Vec2##abbrev##_Sub(ntt_Vec2##abbrev a, ntt_Vec2##abbrev b);                           \
	ntt_Vec2##abbrev		 ntt_Vec2##abbrev##_Mul(ntt_Vec2##abbrev a, ntt_Vec2##abbrev b);                           \
	ntt_Vec2##abbrev		 ntt_Vec2##abbrev##_Div(ntt_Vec2##abbrev a, ntt_Vec2##abbrev b);

VEC2_DECLARE(f32, f)
VEC2_DECLARE(f64, d)
VEC2_DECLARE(i32, i)
VEC2_DECLARE(u32, u)

#define VEC3_DECLARE(type, abbrev)                                                                                     \
	typedef union {                                                                                                    \
		struct                                                                                                         \
		{                                                                                                              \
			type x;                                                                                                    \
			type y;                                                                                                    \
			type z;                                                                                                    \
		};                                                                                                             \
		type data[3];                                                                                                  \
	} ntt_Vec3##abbrev;                                                                                                \
	typedef ntt_Vec3##abbrev ntt_Vec3##abbrev;                                                                         \
	ntt_Vec3##abbrev		 ntt_Vec3##abbrev##_Add(ntt_Vec3##abbrev a, ntt_Vec3##abbrev b);                           \
	ntt_Vec3##abbrev		 ntt_Vec3##abbrev##_Sub(ntt_Vec3##abbrev a, ntt_Vec3##abbrev b);                           \
	ntt_Vec3##abbrev		 ntt_Vec3##abbrev##_Mul(ntt_Vec3##abbrev a, ntt_Vec3##abbrev b);                           \
	ntt_Vec3##abbrev		 ntt_Vec3##abbrev##_Div(ntt_Vec3##abbrev a, ntt_Vec3##abbrev b);

VEC3_DECLARE(f32, f)
VEC3_DECLARE(f64, d)
VEC3_DECLARE(i32, i)
VEC3_DECLARE(u32, u)

#define VEC4_DECLARE(type, abbrev)                                                                                     \
	typedef union {                                                                                                    \
		struct                                                                                                         \
		{                                                                                                              \
			type x;                                                                                                    \
			type y;                                                                                                    \
			type z;                                                                                                    \
			type w;                                                                                                    \
		};                                                                                                             \
		struct                                                                                                         \
		{                                                                                                              \
			type r;                                                                                                    \
			type g;                                                                                                    \
			type b;                                                                                                    \
			type a;                                                                                                    \
		};                                                                                                             \
		type data[4];                                                                                                  \
	} ntt_Vec4##abbrev;                                                                                                \
	typedef ntt_Vec4##abbrev ntt_Vec4##abbrev;                                                                         \
	ntt_Vec4##abbrev		 ntt_Vec4##abbrev##_Add(ntt_Vec4##abbrev a, ntt_Vec4##abbrev b);                           \
	ntt_Vec4##abbrev		 ntt_Vec4##abbrev##_Sub(ntt_Vec4##abbrev a, ntt_Vec4##abbrev b);                           \
	ntt_Vec4##abbrev		 ntt_Vec4##abbrev##_Mul(ntt_Vec4##abbrev a, ntt_Vec4##abbrev b);                           \
	ntt_Vec4##abbrev		 ntt_Vec4##abbrev##_Div(ntt_Vec4##abbrev a, ntt_Vec4##abbrev b);

VEC4_DECLARE(f32, f)
VEC4_DECLARE(f64, d)
VEC4_DECLARE(i32, i)
VEC4_DECLARE(u32, u)

#endif /* _VEC_H_ */