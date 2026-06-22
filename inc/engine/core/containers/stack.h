#ifndef _STACK_H_
#define _STACK_H_

#include "engine/core/memory/alloc.h"
#include "engine/core/result.h"
#include "engine/core/types.h"
#include "list.h"

struct ntt_Stack
{
	ntt_List list;
};

typedef struct ntt_Stack ntt_Stack;
DEFINE_RETURN_RESULT_TYPE(ntt_Stack)

ntt_StackResult ntt_Stack_Create(ntt_Allocator* pAllocator);

ntt_Result ntt_Stack_Push(ntt_Stack* pStack, void* pData, usize dataSize);

voidPtrResult ntt_Stack_Top(ntt_Stack* pStack);

ntt_Result ntt_Stack_Pop(ntt_Stack* pStack);

b8 ntt_Stack_IsEmpty(ntt_Stack* pStack);

ntt_Result ntt_Stack_Clear(ntt_Stack* pStack);

ntt_Result ntt_Stack_Destroy(ntt_Stack* pStack);

#endif /* _STACK_H_ */