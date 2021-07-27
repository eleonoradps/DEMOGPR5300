#include "model.h"

#include "shader.h"
#include <glm/ext/matrix_transform.hpp>

namespace gl {
	Model::Model(const std::string& filename)
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

	Mesh Model::GetMesh(unsigned i)
	{
		return meshes[i];
	}

	void Model::Update(const Shader& shader)
	{
		// Draws each mesh of model
		for (const auto& mesh : meshes)
		{
			mesh.Bind();
			const auto& material = materials[mesh.material_index];
			shader.Use();

			//bind model matrix
			shader.SetMat4("model", _model);
			shader.SetMat4("inv_model", _inv_model);

			//bind texture
			material.color.Bind(0);
			shader.SetInt("diffuseMap", 0);
			material.normal.Bind(1);
			shader.SetInt("normalMap", 1);

			//set parameters
			shader.SetFloat("specular_pow", material.specular_pow);
			shader.SetVec3("specular_vec", material.specular_vec);

			glDrawElements(GL_TRIANGLES, mesh.nb_vertices_, GL_UNSIGNED_INT, 0);
		}
	}

	void Model::SetModelMatrix(glm::vec3 position)
	{
		_model = glm::mat4(1.0f);
		_model = glm::translate(_model, position);
		_inv_model = glm::transpose(glm::inverse(_model));
	}

	void Model::ParseMaterial(const tinyobj::material_t& material)
	{
		Material mat{};
		std::string path = "../data/textures/";
		mat.color = Texture(path + material.diffuse_texname);
		mat.normal = Texture(path + material.bump_texname);
		mat.specular_pow = material.shininess;
		mat.specular_vec = glm::vec3(material.specular[0], material.specular[1], material.specular[2]);
		materials.push_back(mat);
	}

	void Model::ParseMesh(const tinyobj::shape_t& shape, const tinyobj::attrib_t& attrib)
	{
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

				if (v == 2)
				{
					//positions
					glm::vec3 pos1(vertices[vertices.size() - 2].position);
					glm::vec3 pos2(vertices[vertices.size() - 1].position);
					glm::vec3 pos3(vertex.position);

					//texture coordinates
					glm::vec2 uv1(vertices[vertices.size() - 2].texture);
					glm::vec2 uv2(vertices[vertices.size() - 1].texture);
					glm::vec2 uv3(vertex.texture);

					//calculate tangent vectors of both triangles
					glm::vec3 tangent1;

					//calculate first triangle edges and delta uv coords
					glm::vec3 edge1 = pos2 - pos1;
					glm::vec3 edge2 = pos3 - pos1;
					glm::vec2 deltaUV1 = uv2 - uv1;
					glm::vec2 deltaUV2 = uv3 - uv1;

					float f2 = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

					tangent1.x = f2 * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
					tangent1.y = f2 * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
					tangent1.z = f2 * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

					glm::normalize(tangent1);

					vertices[vertices.size() - 2].tangent = tangent1;
					vertices[vertices.size() - 1].tangent = tangent1;
					vertex.tangent = tangent1;
				}
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
}
