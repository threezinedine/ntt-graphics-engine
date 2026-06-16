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

ntt_QueueResult ntt_QueueCreate(ntt_Allocator* pAllocator);

ntt_Result ntt_QueueEnqueue(ntt_Queue* pQueue, void* pData, usize dataSize);

voidPtrResult ntt_QueueFront(ntt_Queue* pQueue);

ntt_Result ntt_QueueDequeue(ntt_Queue* pQueue);

b8 ntt_QueueIsEmpty(ntt_Queue* pQueue);

ntt_Result ntt_QueueClear(ntt_Queue* pQueue);

ntt_Result ntt_QueueDestroy(ntt_Queue* pQueue);

#endif /* _QUEUE_H_ */