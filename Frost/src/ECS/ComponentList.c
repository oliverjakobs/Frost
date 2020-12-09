#include "ComponentList.h"

#include <stdlib.h>
#include <string.h>

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
	// TODO
}

static EcsComponentNode* EcsComponentNodeAlloc(const void* component, EcsEntityID entity, size_t size)
{
	EcsComponentNode* node = malloc(sizeof(EcsComponentNode));

	if (node)
	{
		node->entity = entity;
		node->component = malloc(size);
		memcpy(node->component, component, size);
		node->next = NULL;
	}

	return node;
}

static int EcsComponentNodeCmp(EcsComponentNode* left, EcsComponentNode* right, int (*cmp_func)(const void*, const void*))
{
	if (cmp_func) return cmp_func(left->component, right->component);

	return left->entity - right->entity;
}

void* EcsComponentListInsert(EcsComponentList* list, EcsEntityID entity, void* component)
{
	EcsComponentNode* node = EcsComponentNodeAlloc(component, entity, list->element_size);
	if (!list->first || EcsComponentNodeCmp(node, list->first, list->cmp_func) <= 0)
	{
		node->next = list->first;
		list->first = node;
	}
	else
	{
		EcsComponentNode* it = list->first;
		while (it->next && EcsComponentNodeCmp(node, it->next, list->cmp_func) > 0)
			it = it->next;

		node->next = it->next;
		it->next = node;
	}

	return node;
}

void EcsComponentListRemove(EcsComponentList* list, EcsEntityID entity)
{
	// TODO
}

void* EcsComponentListFind(const EcsComponentList* list, EcsEntityID entity)
{
	for (EcsComponentNode* it = list->first; it; it = it->next)
	{
		if (entity == it->entity)
			return it->component;
	}
	return NULL;
}

EcsEntityID EcsComponentNodeEntity(const EcsComponentNode* node) { return node->entity; }
void* EcsComponentNodeComponent(const EcsComponentNode* node) { return node->component; }
EcsComponentNode* EcsComponentNodeNext(const EcsComponentNode* node) { return node->next; }
