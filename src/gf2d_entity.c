#include <stdlib.h>
#include <string.h>
#include "gfc_list.h"
#include "simple_logger.h"
#include "gf2d_sprite.h"
#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_entity.h"
#include "simple_logger.h"

typedef struct
{
	Entity *entity_list;
	Uint32  entity_max;
}EntityManager;

static EntityManager gf2d_entity_manager = { 0 };

void gf2d_entity_manager_close()
{
	if (gf2d_entity_manager.entity_list != NULL)
	{
		free(gf2d_entity_manager.entity_list);
	}
	memset(&gf2d_entity_manager, 0, sizeof(EntityManager));
}

void gf2d_entity_manager_init(Uint32 entity_max)
{
	gf2d_entity_manager.entity_list = (Entity*)gfc_allocate_array(sizeof(Entity), entity_max);
	if (!gf2d_entity_manager.entity_list)
	{
		slog("failed to allocate entity list");
		return;
	}
	gf2d_entity_manager.entity_max = entity_max;
	slog("allocated entity list of size %u", sizeof(gf2d_entity_manager.entity_list));
	atexit(gf2d_entity_manager_close);
}

Entity *gf2d_entity_new()
{
	int i;
	for (i = 0; i < gf2d_entity_manager.entity_max; i++)
	{
		slog("the _inuse flage for this entity is %u", gf2d_entity_manager.entity_list[i]._inuse);
		if (gf2d_entity_manager.entity_list[i]._inuse)continue;
		//. found a free entity
		memset(&gf2d_entity_manager.entity_list[i], 0, sizeof(Entity));
		gf2d_entity_manager.entity_list[i]._inuse = 1;
		return &gf2d_entity_manager.entity_list[i];
	}
	slog("request for entity failed: all full up");
	return NULL;
}

void gf2d_entity_free(Entity *self)
{
	if (!self)
	{
		slog("self pointer is not valid");
		return;
	}
	self->_inuse = 0;
	gf2d_sprite_free(self->sprite);
}
