#ifndef __GF2D_ENTITY_H__
#define __GF2D_ENTITY_H__

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

typedef enum
{
	ES_Idle = 0,
	ES_Dying = 1,
	ES_Dead = 2
}EntityState;

typedef struct Entity_S
{
	Uint8        _inuse; /**<Flag to keep track if the instance is in use**/
	Vector2D     position; /**<This is where we keep track of the position**/
	Vector2D     velocity; /**<The speed that entity is moving at**/
	Vector2D     acceleration; /**<The additive that is being applied to the velocity**/
	Vector2D     rotation; /**<The rotation value of the entity**/
	Vector2D     scale;   /**<How big the entity is**/
	EntityState state; /**<What state that entity is in**/
	void(*think)(struct Entity_S* self);
	void(*update)(struct Entity_S* self);
	void(*touch)(struct Entity_S* self, struct Entity_S* other);
	Sprite       *sprite;/**<The sprite being used**/
	float        speed; /**<The Speed of an object*/
	float		 health; /**<The HP of your Ent*/

}Entity;
/**
* @brief initializes the entity subsystem
* @param entity_max maximum number of simultaneous entities you wish to support
*/
void gf2d_entity_manager_init(Uint32 entity_max);
/**
* @brief get an empty entity from the system
* @return NULL on out of space or a pointer to an entity otherwise
*/
Entity *gf2d_entity_new();
/**
* @brief free an active entity
* @param self the entity to free
*/
void gf2d_entity_free(Entity *self);
void gf2d_update_all_entities();
void update_entity(Entity *e);
void gf2d_entity_sync_position(Entity *e);
void gf2d_set_entity_bounding_box(Entity *e);
Entity * gf2d_entity_init(char * sprite, Bool isEnvironment, int startFrame, int endFrame);
int gf2d_entity_manager_get_size();
Entity * gf2d_entity_manager_get_entity(int n);


#endif