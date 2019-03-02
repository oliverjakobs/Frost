#pragma once

#include <vector>

typedef unsigned char uint8;
typedef unsigned int uint32;

typedef std::vector<std::pair<uint32, uint32>> Entity;
typedef std::pair<uint32, Entity> IndexedEntity;

typedef void* EntityHandle;
#define NULL_ENTITY_HANDLE nullptr

enum ECSFlag
{
	ECS_FLAG_NONE = 0,
	ECS_FLAG_OPTIONAL = 1
};