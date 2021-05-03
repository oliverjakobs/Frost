#include "EcsList.h"

#include <stdlib.h>
#include <string.h>

struct EcsListNode
{
	EcsEntityID entity;
	void* component;
	EcsListNode* next;
};

static EcsListNode* EcsListNodeAlloc(const void* component, EcsEntityID entity, size_t size)
{
	EcsListNode* node = EcsMemAlloc(sizeof(EcsListNode));

	if (node)
	{
		node->entity = entity;
		node->component = EcsMemDup(component, size);
		node->next = NULL;
	}

	return node;
}

static void EcsListNodeFree(const EcsList* list, EcsListNode* node)
{
	if (list->free) list->free(node->component);
	EcsMemFree(node->component);
	EcsMemFree(node);
}

static int EcsListNodeCmp(const EcsListNode* left, const EcsListNode* right, EcsCmpFunc cmp)
{
	if (cmp) return cmp(left->component, right->component);

	return left->entity - right->entity;
}

int EcsListAlloc(EcsList* list, size_t elem_size, EcsFreeFunc free, EcsCmpFunc cmp)
{
	list->first = NULL;
	list->element_size = elem_size;
	list->free = free;
	list->cmp = cmp;

	return 1;
}

void EcsListFree(EcsList* list)
{
	EcsListClear(list);
	list->element_size = 0;
	list->free = NULL;
	list->cmp = NULL;
}

void EcsListClear(EcsList* list)
{
	EcsListNode* next = NULL;

	while (list->first)
	{
		next = list->first->next;
		EcsListNodeFree(list, list->first);
		list->first = next;
	}

	list->first = NULL;
}

void* EcsListInsert(EcsList* list, EcsEntityID entity, const void* component)
{
	EcsListNode* node = EcsListNodeAlloc(component, entity, list->element_size);
	if (!list->first || EcsListNodeCmp(node, list->first, list->cmp) <= 0)
	{
		node->next = list->first;
		list->first = node;
	}
	else
	{
		EcsListNode* it = list->first;
		while (it->next && EcsListNodeCmp(node, it->next, list->cmp) > 0)
			it = it->next;

		node->next = it->next;
		it->next = node;
	}

	return node;
}

void EcsListRemove(EcsList* list, EcsEntityID entity)
{
	EcsListNode* node = list->first;
	EcsListNode* prev = NULL;

	while (node && node->entity != entity)
	{
		prev = node;
		node = node->next;
	}

	if (node == NULL) return;

	if (prev)
		prev->next = node->next;
	else
		list->first = node->next;

	EcsListNodeFree(list, node);
}

void* EcsListFind(const EcsList* list, EcsEntityID entity)
{
	for (EcsListNode* it = list->first; it; it = it->next)
	{
		if (entity == it->entity)
			return it->component;
	}
	return NULL;
}

EcsEntityID EcsListNodeEntity(const EcsListNode* node) { return node->entity; }
void* EcsListNodeComponent(const EcsListNode* node) { return node->component; }
EcsListNode* EcsListNodeNext(const EcsListNode* node) { return node->next; }
