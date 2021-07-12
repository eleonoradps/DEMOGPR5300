#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

#include "shader.h"

namespace gl {
	class ShadowMap {
	public:
		unsigned int depthMapFBO;
		unsigned int depthMap;

		// 1024 is resolution of depth map
		const unsigned int SHADOW_WIDTH = 1024;
		const unsigned int SHADOW_HEIGHT = 1024;

		ShadowMap()
		{
			// FRamebuffer object for rendering depth map
			glGenFramebuffers(1, &depthMapFBO);

			// Create 2D texture that will be used as framebuffer's depth buffer
			glGenTextures(1, &depthMap);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_DEPTH_COMPONENT,
				SHADOW_WIDTH,
				SHADOW_HEIGHT,
				0,
				GL_DEPTH_COMPONENT,
				GL_FLOAT,
				NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			// Attach the depth texture as framebuffer's depth buffer
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glFramebufferTexture2D(
				GL_FRAMEBUFFER,
				GL_DEPTH_ATTACHMENT,
				GL_TEXTURE_2D,
				depthMap,
				0);
			glDrawBuffers(GL_NONE, 0);
			glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);


		}
	};
} // End namespace gl.