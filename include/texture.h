#pragma once
#include <string>
#include <fstream>
#include <glad/glad.h>
namespace gl {
	class Texture {
	public:
		unsigned int id;

		Texture() = default;
		Texture(const std::string& file_name);
			
		void Bind(unsigned int i = 0) const;

		void UnBind() const;

	protected:
		void IsError(const char* file, int line);
	};
} // End namespace gl.