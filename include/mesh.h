#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>

namespace gl {

    class Vertex
    {
    public:
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texture;
    };

    class Mesh
    {
    public:
        unsigned int vao_;
        unsigned int vbo_;
        unsigned int ebo_;
        unsigned int nb_vertices_;
        unsigned int material_index;

        Mesh(const std::vector<Vertex>& vertices, 
            const std::vector<std::uint32_t>& indices,
            const unsigned int material_id);

        void Bind() const;

        void UnBind() const;

        unsigned GetVAO() const;


    protected:
        void IsError(const std::string& file, int line);
    };
}