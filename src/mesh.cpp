#include "mesh.h"

namespace gl
{
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices, const unsigned int material_id) :
        material_index(material_id),
        nb_vertices_(static_cast<unsigned int>(indices.size()))
    {
        // VAO binding should be before VAO.
        glGenVertexArrays(1, &vao_);
        glBindVertexArray(vao_);

        // EBO.
        glGenBuffers(1, &ebo_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(float),
            indices.data(),
            GL_STATIC_DRAW);

        // VBO.
        glGenBuffers(1, &vbo_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(
            GL_ARRAY_BUFFER,
            vertices.size() * sizeof(Vertex),
            vertices.data(),
            GL_STATIC_DRAW);

        GLintptr vertex_color_offset = 3 * sizeof(float);
        GLintptr vertex_tex_offset = 6 * sizeof(float);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            8 * sizeof(float),
            0);
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            8 * sizeof(float),
            (GLvoid*)vertex_color_offset);
        glVertexAttribPointer(
            2,
            2,
            GL_FLOAT,
            GL_FALSE,
            8 * sizeof(float),
            (GLvoid*)vertex_tex_offset);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Mesh::Bind() const
    {
        glBindVertexArray(vao_);
    }

    void Mesh::UnBind() const
    {
        glBindVertexArray(0);
    }

    unsigned Mesh::GetVAO() const
    {
        return vao_;
    }

    void Mesh::IsError(const std::string& file, int line)
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
}