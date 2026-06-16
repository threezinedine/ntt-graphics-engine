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

ntt_StackResult ntt_StackCreate(ntt_Allocator* pAllocator);

ntt_Result ntt_StackPush(ntt_Stack* pStack, void* pData, usize dataSize);

voidPtrResult ntt_StackTop(ntt_Stack* pStack);

ntt_Result ntt_StackPop(ntt_Stack* pStack);

b8 ntt_StackIsEmpty(ntt_Stack* pStack);

ntt_Result ntt_StackClear(ntt_Stack* pStack);

ntt_Result ntt_StackDestroy(ntt_Stack* pStack);

#endif /* _STACK_H_ */