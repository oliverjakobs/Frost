#include "ComponentList.h"

#include <stdlib.h>
#include <string.h>

struct EcsListNode
{
	EcsEntityID entity;
	void* component;
	EcsListNode* next;
};

int EcsComponentListAlloc(EcsComponentList* list, size_t elem_size, int (*cmp)(const void*, const void*))
{
	list->first = NULL;
	list->element_size = elem_size;
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
		free(list->first);
		list->first = next;
	}

	list->first = NULL;
}

static EcsListNode* EcsComponentNodeAlloc(const void* component, EcsEntityID entity, size_t size)
{
	EcsListNode* node = malloc(sizeof(EcsListNode));

	if (node)
	{
		node->entity = entity;
		node->component = malloc(size);
		memcpy(node->component, component, size);
		node->next = NULL;
	}

	return node;
}

static int EcsComponentNodeCmp(const EcsListNode* left, const EcsListNode* right, int (*cmp_func)(const void*, const void*))
{
	if (cmp_func) return cmp_func(left->component, right->component);

	return left->entity - right->entity;
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

	free(node);
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
