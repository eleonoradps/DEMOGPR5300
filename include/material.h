#pragma once
#include <memory>
#include "texture.h"

namespace gl {
	class Material {
	public:
		Texture color;
		Texture specular;
		Texture normal;
		float specular_pow;
		glm::vec3 specular_vec;
	};
}