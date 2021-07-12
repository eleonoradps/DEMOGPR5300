#pragma once

#include <tiny_obj_loader.h>

#include <vector>
#include "mesh.h"
#include "material.h"
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace gl {
	class Shader; //prototype

	class Model {
	public:
		std::vector<Mesh> meshes;
		std::vector<Material> materials;
		Model(const std::string& filename);

		Mesh GetMesh(unsigned i);

		void Update(const Shader& shader);

	private:
		void ParseMaterial(const tinyobj::material_t& material);
		void ParseMesh(const tinyobj::shape_t& shape, const tinyobj::attrib_t& attrib);
	};
}