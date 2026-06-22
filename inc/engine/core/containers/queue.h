#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "engine/core/memory/alloc.h"
#include "engine/core/result.h"
#include "engine/core/types.h"
#include "list.h"

struct ntt_Queue
{
	ntt_List list;
};

typedef struct ntt_Queue ntt_Queue;
DEFINE_RETURN_RESULT_TYPE(ntt_Queue)

ntt_QueueResult ntt_Queue_Create(ntt_Allocator* pAllocator);

ntt_Result ntt_Queue_Enqueue(ntt_Queue* pQueue, void* pData, usize dataSize);

voidPtrResult ntt_Queue_Front(ntt_Queue* pQueue);

ntt_Result ntt_Queue_Dequeue(ntt_Queue* pQueue);

b8 ntt_Queue_IsEmpty(ntt_Queue* pQueue);

ntt_Result ntt_Queue_Clear(ntt_Queue* pQueue);

ntt_Result ntt_Queue_Destroy(ntt_Queue* pQueue);

#endif /* _QUEUE_H_ */