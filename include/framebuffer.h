#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

#include "shader.h"

namespace gl {
	class Framebuffer
	{
	public:
		unsigned int fbo;

		// FBO framebuffer object
		unsigned int texColorBuffer;

		// RBO renderbuffer object
		unsigned int rbo;

		unsigned int quadVAO;
		unsigned int quadVBO;

		Framebuffer();

		void Draw() const;

		void Bind() const;

		void Unbind() const;

		unsigned int GetColorBuffer();
	};
}