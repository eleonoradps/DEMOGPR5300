#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"

namespace gl
{
	// Container object for holding all state relevant for a single
	// game object entity.
	class GameObject
	{
	public:
		//object state
		glm::vec2 position;
		glm::vec2 size;
		glm::vec2 velocity;

		glm::vec3 color;
		float rotation;
		bool isSolid;
		bool isDestroyed;

		//render state
		Texture sprite;

		GameObject();
		GameObject(
			glm::vec2 pos,
			glm::vec2 size,
			Texture sprite,
			glm::vec3 color = glm::vec3(1.0f),
			glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
	};
}