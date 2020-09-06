#ifndef ECS_ENTITY_H
#define ECS_ENTITY_H

#include <stdint.h>

typedef int32_t EcsEntityID;
#define ECS_NULL_ENTITY (-1)

void EcsEntityResetIDCounter();
EcsEntityID EcsEntityGetNextID();


#endif /* !ECS_ENTITY_H */
