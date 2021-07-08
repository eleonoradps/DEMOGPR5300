#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace gl {
	Texture::Texture(const std::string& file_name)
	{
		int width, height, nrChannels;
			stbi_set_flip_vertically_on_load(true);
			unsigned char* dataDiffuse = stbi_load(
				file_name.c_str(),
				&width,
				&height,
				&nrChannels,
				0);
			assert(dataDiffuse);
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
			if (nrChannels == 1)
			{
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RED,
					width,
					height,
					0,
					GL_RED,
					GL_UNSIGNED_BYTE,
					dataDiffuse);
			}
			if (nrChannels == 3)
			{
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RGB,
					width,
					height,
					0,
					GL_RGB,
					GL_UNSIGNED_BYTE,
					dataDiffuse);
			}
			if (nrChannels == 4)
			{
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					GL_RGBA,
					width,
					height,
					0,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					dataDiffuse);
			}
			glTexParameteri(
				GL_TEXTURE_2D,
				GL_TEXTURE_WRAP_S,
				GL_MIRRORED_REPEAT);
			glTexParameteri(
				GL_TEXTURE_2D,
				GL_TEXTURE_WRAP_T,
				GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::Bind(unsigned int i) const
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void Texture::UnBind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::IsError(const char* file, int line)
	{
		auto error_code = glGetError();
		if (error_code != GL_NO_ERROR)
		{
			throw std::runtime_error(
				std::to_string(error_code) +
				" in file: " + file +
				" at line: " + std::to_string(line));
		}
	}
} // End namespace gl