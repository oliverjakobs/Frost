#include "ComponentList.h"

#include "Ecs.h"

#include <stdlib.h>
#include <string.h>

struct EcsListNode
{
	EcsEntityID entity;
	void* component;
	EcsListNode* next;
};

static EcsListNode* EcsComponentNodeAlloc(const void* component, EcsEntityID entity, size_t size)
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

static void EcsComponentNodeFree(const EcsComponentList* list, EcsListNode* node)
{
	if (list->free_func) list->free_func(node->component);
	EcsMemFree(node->component);
	EcsMemFree(node);
}

static int EcsComponentNodeCmp(const EcsListNode* left, const EcsListNode* right, int (*cmp_func)(const void*, const void*))
{
	if (cmp_func) return cmp_func(left->component, right->component);

	return left->entity - right->entity;
}

int EcsComponentListAlloc(EcsComponentList* list, size_t elem_size, void (*free)(void*), int (*cmp)(const void*, const void*))
{
	list->first = NULL;
	list->element_size = elem_size;
	list->free_func = free;
	list->cmp_func = cmp;

	return 1;
}

void EcsComponentListFree(EcsComponentList* list)
{
	EcsComponentListClear(list);
	list->element_size = 0;
	list->cmp_func = NULL;
}

void EcsComponentListClear(EcsComponentList* list)
{
	EcsListNode* next = NULL;

	while (list->first)
	{
		next = list->first->next;
		EcsComponentNodeFree(list, list->first);
		list->first = next;
	}

	list->first = NULL;
}

void* EcsComponentListInsert(EcsComponentList* list, EcsEntityID entity, void* component)
{
	EcsListNode* node = EcsComponentNodeAlloc(component, entity, list->element_size);
	if (!list->first || EcsComponentNodeCmp(node, list->first, list->cmp_func) <= 0)
	{
		node->next = list->first;
		list->first = node;
	}
	else
	{
		EcsListNode* it = list->first;
		while (it->next && EcsComponentNodeCmp(node, it->next, list->cmp_func) > 0)
			it = it->next;

		node->next = it->next;
		it->next = node;
	}

	return node;
}

void EcsComponentListRemove(EcsComponentList* list, EcsEntityID entity)
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

	EcsComponentNodeFree(list, node);
}

void* EcsComponentListFind(const EcsComponentList* list, EcsEntityID entity)
{
	for (EcsListNode* it = list->first; it; it = it->next)
	{
		if (entity == it->entity)
			return it->component;
	}
	return NULL;
}

EcsEntityID EcsComponentNodeEntity(const EcsListNode* node) { return node->entity; }
void* EcsComponentNodeComponent(const EcsListNode* node) { return node->component; }
EcsListNode* EcsComponentNodeNext(const EcsListNode* node) { return node->next; }
