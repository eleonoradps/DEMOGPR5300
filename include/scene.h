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


		/*void Bind(unsigned int i = 0) const
		{
			glBindVertexArray(vao);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		}*/
	};
}