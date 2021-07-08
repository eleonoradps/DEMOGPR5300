#pragma once

#include <string>
#include < vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stb_image.h>

#include "shader.h"

namespace gl
{
	class Scene
	{
	public:
		unsigned int textureID;
		unsigned int vao;
		unsigned int vbo;

		int width, height, nrChannels;

		Scene() {}
	};
}