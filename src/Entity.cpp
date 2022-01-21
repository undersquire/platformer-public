#include "Entity.h"

Entity::Entity(Backend *backend, EntityType type) noexcept : backend(backend), type(type)
{
}