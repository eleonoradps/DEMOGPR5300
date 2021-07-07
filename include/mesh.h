#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
//#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
//#include <tiny_obj_loader.h>

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
            const unsigned int material_id) : 
            material_index(material_id), 
            nb_vertices_(static_cast<unsigned int>(indices.size()))
        {
            // VAO binding should be before VAO.
            glGenVertexArrays(1, &vao_);
            IsError(__FILE__, __LINE__);
            glBindVertexArray(vao_);
            IsError(__FILE__, __LINE__);

            // EBO.
            glGenBuffers(1, &ebo_);
            IsError(__FILE__, __LINE__);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
            IsError(__FILE__, __LINE__);
            glBufferData(
                GL_ELEMENT_ARRAY_BUFFER,
                indices.size() * sizeof(float),
                indices.data(),
                GL_STATIC_DRAW);
            IsError(__FILE__, __LINE__);

            // VBO.
            glGenBuffers(1, &vbo_);
            IsError(__FILE__, __LINE__);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_);
            IsError(__FILE__, __LINE__);
            glBufferData(
                GL_ARRAY_BUFFER,
                vertices.size() * sizeof(Vertex),
                vertices.data(),
                GL_STATIC_DRAW);
            IsError(__FILE__, __LINE__);

            GLintptr vertex_color_offset = 3 * sizeof(float);
            GLintptr vertex_tex_offset = 6 * sizeof(float);
            glVertexAttribPointer(
                0,
                3,
                GL_FLOAT,
                GL_FALSE,
                8 * sizeof(float),
                0);
            IsError(__FILE__, __LINE__);
            glVertexAttribPointer(
                1,
                3,
                GL_FLOAT,
                GL_FALSE,
                8 * sizeof(float),
                (GLvoid*)vertex_color_offset);
            IsError(__FILE__, __LINE__);
            glVertexAttribPointer(
                2,
                2,
                GL_FLOAT,
                GL_FALSE,
                8 * sizeof(float),
                (GLvoid*)vertex_tex_offset);
            IsError(__FILE__, __LINE__);
            glEnableVertexAttribArray(0);
            IsError(__FILE__, __LINE__);
            glEnableVertexAttribArray(1);
            IsError(__FILE__, __LINE__);
            glEnableVertexAttribArray(2);
            IsError(__FILE__, __LINE__);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            IsError(__FILE__, __LINE__);
            glBindVertexArray(0);
            IsError(__FILE__, __LINE__);
        }
        void Bind() const
        {
            glBindVertexArray(vao_);
        }
        void UnBind() const
        {
            glBindVertexArray(0);
        }

        unsigned GetVAO() const
        {
            return vao_;
        }


    protected:
        void IsError(const std::string& file, int line)
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
    };
}