#ifndef _LIST_H_
#define _LIST_H_

#include "engine/core/memory/alloc.h"
#include "engine/core/result.h"
#include "engine/core/types.h"

struct ntt_List;

struct ntt_ListNode
{
	void*				 pData;
	usize				 dataSize;
	struct ntt_ListNode* pNext;
	struct ntt_ListNode* pPrev;
	struct ntt_List*	 pOwner;
};

/**
 */
struct ntt_List
{
	struct ntt_ListNode* pHead;
	struct ntt_ListNode* pTail;
	usize				 length;
	ntt_Allocator*		 pAllocator;
};

typedef struct ntt_ListNode ntt_ListNode;
typedef struct ntt_List		ntt_List;
DEFINE_RETURN_RESULT_TYPE(ntt_List)

typedef b8 (*ntt_ListElementPredicate)(void* pElement);

ntt_ListResult ntt_ListCreate(ntt_Allocator* pAllocator);

ntt_Result ntt_ListAppend(ntt_List* pList, void* pData, usize dataSize);

ntt_Result ntt_ListHeadAppend(ntt_List* pList, void* pData, usize dataSize);

ntt_Result ntt_ListInsert(ntt_List* pList, usize index, void* pData, usize dataSize);

ntt_Result ntt_ListRemove(ntt_List* pList, usize index);

voidPtrResult ntt_ListGet(ntt_List* pList, usize index);

b8 ntt_ListContains(ntt_List* pList, ntt_ListElementPredicate predicate);

ntt_Result ntt_ListInsertAfterNode(ntt_List* pList, ntt_ListNode* pNode, void* pData, usize dataSize);

ntt_Result ntt_ListInsertBeforeNode(ntt_List* pList, ntt_ListNode* pNode, void* pData, usize dataSize);

ntt_Result ntt_ListRemoveNode(ntt_List* pList, ntt_ListNode* pNode);

ntt_Result ntt_ListClear(ntt_List* pList);

ntt_Result ntt_ListDestroy(ntt_List* pList);

#endif /* _LIST_H_ */