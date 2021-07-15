#include "game_object.h"

namespace gl
{
	GameObject::GameObject() :
		position(0.0f, 0.0f),
		size(1.0f, 1.0f),
		velocity(0.0f),
		color(1.0f),
		rotation(1.0f),
		sprite(),
		isSolid(false),
		isDestroyed(false) {}

	GameObject::GameObject(
		glm::vec2 pos,
		glm::vec2 size, 
		Texture sprite, 
		glm::vec3 color, 
		glm::vec2 velocity) :

		position(pos),
		size(size),
		velocity(velocity),
		color(color),
		rotation(0.0f),
		sprite(sprite),
		isSolid(false),
		isDestroyed(false) {}

}