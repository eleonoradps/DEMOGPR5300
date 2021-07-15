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
		//glm::vec3 position = glm::vec3(0, 0, 0); // mountain position

		std::vector<Mesh> meshes;
		std::vector<Material> materials;
		Model(const std::string& filename);

		Mesh GetMesh(unsigned i);

		void Update(const Shader& shader);

		void SetModelMatrix(glm::vec3 position = glm::vec3(0, 0, 0));

		private:
		glm::mat4 _model = glm::mat4(1.0f);
		glm::mat4 _inv_model = glm::mat4(1.0f);

		void ParseMaterial(const tinyobj::material_t& material);
		void ParseMesh(const tinyobj::shape_t& shape, const tinyobj::attrib_t& attrib);
	};
}