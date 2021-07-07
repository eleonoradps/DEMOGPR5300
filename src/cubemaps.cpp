#include "cubemaps.h"

namespace gl {
	Cubemaps::Cubemaps() {
		// Skybox
		std::array<float, 108> skybox_vertices
		{
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f

		};

		// Generate vao skybox
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// Generate vbo skybox
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		// Transfer data into gpu
		glBufferData(GL_ARRAY_BUFFER, sizeof(skybox_vertices), &skybox_vertices, GL_STATIC_DRAW);

		// Location in skybox.vert
		glEnableVertexAttribArray(0);

		// Linking datas of skybox vertices already in gpu and how to use them 
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

		// Saying we're not using vao anymore
		glBindVertexArray(0);

		std::string path = "../";
		std::vector<std::string> faces{
			path + "data/textures/Skybox/right.jpg",
			path + "data/textures/Skybox/left.jpg",
			path + "data/textures/Skybox/top.jpg",
			path + "data/textures/Skybox/bottom.jpg",
			path + "data/textures/Skybox/front.jpg",
			path + "data/textures/Skybox/back.jpg"
		};

		//Texture skybox
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		for (unsigned int i = 0; i < faces.size(); i++)
		{
			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(
					GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0,
					GL_RGB,
					width,
					height,
					0,
					GL_RGB,
					GL_UNSIGNED_BYTE,
					data);
				stbi_image_free(data);
			}
			else
			{
				std::cout << "skybox texture fail to load \n";
				stbi_image_free(data);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	void Cubemaps::Bind(unsigned int i) const
	{
		glBindVertexArray(vao);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	}
} // namespace gl