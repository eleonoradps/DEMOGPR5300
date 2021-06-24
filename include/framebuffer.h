#pragma once

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

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

		Framebuffer()
		{
			float quadVertices[24] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
				// positions   // texCoords
				-1.0f,  1.0f,  0.0f, 1.0f,
				-1.0f, -1.0f,  0.0f, 0.0f,
				1.0f, -1.0f,  1.0f, 0.0f,

				-1.0f,  1.0f,  0.0f, 1.0f,
				1.0f, -1.0f,  1.0f, 0.0f,
				1.0f,  1.0f,  1.0f, 1.0f
			};

			glGenFramebuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);

			glGenTextures(1, &texColorBuffer);
			glBindTexture(GL_TEXTURE_2D, texColorBuffer);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGB,
				1024,
				720,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(
				GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D,
				texColorBuffer,
				0);

			// RBO renderbuffer object
			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1024, 720);
			glFramebufferRenderbuffer(
				GL_FRAMEBUFFER,
				GL_DEPTH_STENCIL_ATTACHMENT,
				GL_RENDERBUFFER,
				rbo);

			// rbo error
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// QuadVAO
			glGenVertexArrays(1, &quadVAO);
			glBindVertexArray(quadVAO);

			// QuadVBO
			glGenBuffers(1, &quadVBO);
			glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
			glBufferData(
				GL_ARRAY_BUFFER, 
				sizeof(quadVertices), 
				&quadVertices,
				GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(
				0,
				2,
				GL_FLOAT,
				GL_FALSE,
				4 * sizeof(float),
				(void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(
				1,
				2,
				GL_FLOAT,
				GL_FALSE,
				4 * sizeof(float),
				(void*)(2 * sizeof(float)));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
		}

		void Draw() const
		{
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		void Bind() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		}

		void Unbind() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		unsigned int GetColorBuffer()
		{
			return texColorBuffer;
		}
	};
}