#ifndef ENTITY_H
#define ENTITY_H

#include <stdint.h>

typedef int32_t EntityID;
#define NULL_ENTITY (-1)

void EntityResetIDCounter();
EntityID EntityGetNextID();


#endif /* !ENTITY_H */
