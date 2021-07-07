#pragma once

#include <string>
#include <vector>
#include <array>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stb_image.h>
//#include <GLFW/glfw3.h>

#include "shader.h"

namespace gl 
{
	class Cubemaps 
	{
	public:
		unsigned int textureID;
		unsigned int vao;
		unsigned int vbo;

        int width, height, nrChannels;

		Cubemaps();

		void Bind(unsigned int i = 0) const;
	};
}