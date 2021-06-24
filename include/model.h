#pragma once
#include <vector>
#include "mesh.h"
#include "material.h"
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace gl {
	class Model {
	public:
		std::vector<Mesh> meshes;
		std::vector<Material> materials;
		Model(const std::string& filename)
		{
			tinyobj::ObjReader reader;
			if (!reader.ParseFromFile(filename))
			{
				throw std::runtime_error("Cannot load file: " + filename);
			}
			auto& attrib = reader.GetAttrib();
			auto& shapes = reader.GetShapes();
			auto& materials = reader.GetMaterials();
			for (const auto& material : materials)
			{
				ParseMaterial(material);
			}
			for (const auto& shape : shapes)
			{
				ParseMesh(shape, attrib);
			}
		}

	private:
		void ParseMaterial(const tinyobj::material_t& material)
		{
			Material mat{};
			std::string path = "../data/textures/";
			mat.color = Texture(path + material.diffuse_texname);
			mat.specular = Texture(path + material.roughness_texname);
			//mat.specular_strength = 2.0f;
			mat.specular_pow = material.shininess;
			mat.specular_vec = glm::vec3(material.specular[0], material.specular[1], material.specular[2]);
			materials.push_back(mat);
		}
		void ParseMesh(
			const tinyobj::shape_t& shape,
			const tinyobj::attrib_t& attrib)
		{
			//tinyobj::ObjReader reader;
			/*auto& attrib = reader.GetAttrib();
			auto& shapes = reader.GetShapes();
			auto& materials = reader.GetMaterials();*/

			unsigned int material_id = 0;
			std::vector<Vertex> vertices;
			std::vector<std::uint32_t> indices;
			int index_offset = 0;
			for (std::size_t f = 0; f < shape.mesh.num_face_vertices.size(); ++f)
			{
				int fv = shape.mesh.num_face_vertices[f];
				if (fv != 3) throw std::runtime_error("Should be triangles ?");
				for (size_t v = 0; v < fv; v++)
				{
					Vertex vertex{};

					tinyobj::index_t idx =
						shape.mesh.indices[index_offset + v];
					vertex.position.x = attrib.vertices[3 * idx.vertex_index + 0];
					vertex.position.y = attrib.vertices[3 * idx.vertex_index + 1];
					vertex.position.z = attrib.vertices[3 * idx.vertex_index + 2];
					vertex.normal.x = attrib.normals[3 * idx.normal_index + 0];
					vertex.normal.y = attrib.normals[3 * idx.normal_index + 1];
					vertex.normal.z = attrib.normals[3 * idx.normal_index + 2];
					vertex.texture.x =
						attrib.texcoords[2 * idx.texcoord_index + 0];
					vertex.texture.y =
						attrib.texcoords[2 * idx.texcoord_index + 1];
					vertices.push_back(vertex);
					indices.push_back(static_cast<int>(indices.size()));
				}
				index_offset += fv;
			}
			material_id = shape.mesh.material_ids[0];
			assert(index_offset == indices.size());
			// give textures / load texture
			meshes.emplace_back(vertices, indices, material_id);
		}
	};
}