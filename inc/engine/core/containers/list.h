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

typedef b8 (*ntt_ListElementPredicate)(void* pElement, usize elementSize, void* pUserData, usize userDataSize);
typedef ntt_Result (*ntt_ListNodeDestructor)(void* pData, usize dataSize);

/**
 */
struct ntt_List
{
	struct ntt_ListNode*   pHead;
	struct ntt_ListNode*   pTail;
	usize				   length;
	ntt_Allocator*		   pAllocator;
	ntt_ListNodeDestructor nodeDestructor;
};

typedef struct ntt_ListNode ntt_ListNode;
typedef struct ntt_List		ntt_List;
DEFINE_RETURN_RESULT_TYPE(ntt_List)

ntt_ListResult ntt_List_Create(ntt_Allocator* pAllocator, ntt_ListNodeDestructor nodeDestructor);

ntt_Result ntt_List_Append(ntt_List* pList, void* pData, usize dataSize);

ntt_Result ntt_List_HeadAppend(ntt_List* pList, void* pData, usize dataSize);

ntt_Result ntt_List_Insert(ntt_List* pList, usize index, void* pData, usize dataSize);

ntt_Result ntt_List_Remove(ntt_List* pList, usize index);

voidPtrResult ntt_List_Get(ntt_List* pList, usize index);

b8 ntt_List_Contains(ntt_List* pList, ntt_ListElementPredicate predicate, void* pUserData, usize userDataSize);

ntt_ListNode*
ntt_List_FindNode(ntt_List* pList, ntt_ListElementPredicate predicate, void* pUserData, usize userDataSize);

ntt_Result ntt_List_InsertAfterNode(ntt_List* pList, ntt_ListNode* pNode, void* pData, usize dataSize);

ntt_Result ntt_List_InsertBeforeNode(ntt_List* pList, ntt_ListNode* pNode, void* pData, usize dataSize);

ntt_Result ntt_List_RemoveNode(ntt_List* pList, ntt_ListNode* pNode);

ntt_Result ntt_List_Clear(ntt_List* pList);

ntt_Result ntt_List_Destroy(ntt_List* pList);

#endif /* _LIST_H_ */