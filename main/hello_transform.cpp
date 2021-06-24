#include <SDL_main.h>
#include <glad/glad.h>
#include <array>
#include <string>
#include <cmath>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "engine.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace gl
{
    class HelloSquare : public Program
    {
    public:
        void Init() override;
        void Update(seconds dt) override;
        void Destroy() override;
        void OnEvent(SDL_Event& event) override;
        void DrawImGui() override;

    protected:
        void SetUniformMatrix() const;
        void IsError(const std::string& file, int line) const;

        void SetModelMatrix(seconds dt);
        void SetViewMatrix(seconds dt);
        void SetProjectionMatrix();

    protected:
        unsigned int vao_ = 0;
        unsigned int vbo_[3] = {};
        /*unsigned int ebo_ = 0;*/

        unsigned int vertex_shader_ = 0;
        unsigned int fragment_shader_ = 0;
        unsigned int program_ = 0;
        float timer_ = 0.0f;
        unsigned int texture_ = 0;
        int width_ = 0;
        int height_ = 0;
        int nrChannels_ = 0;

        // Transform
        glm::mat4 model_ = glm::mat4(1.0f);
        glm::mat4 view_ = glm::mat4(1.0f);
        glm::mat4 projection_ = glm::mat4(1.0f);
        glm::vec3 delta_positon_ = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 2.0f);


        std::array<float, 108> vertex_point_ = {
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f,

            -0.5f,  0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
        };

        std::array<float, 108> color_point_ = {
            1.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.5f, 0.0f, 0.5f,
            1.0f, 0.0f, 1.0f,

            // 
            1.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.5f, 0.0f, 0.5f,
            1.0f, 0.0f, 1.0f,

            // 
            1.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.5f, 0.0f, 0.5f,
            1.0f, 0.0f, 1.0f,


            1.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.5f, 0.0f, 0.5f,
            1.0f, 0.0f, 1.0f,


            1.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.5f, 0.0f, 0.5f,
            1.0f, 0.0f, 1.0f,


            1.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.5f, 0.0f, 0.5f,
            1.0f, 0.0f, 1.0f
        };

        //std::array<unsigned int, 36> indices_point_ = {
        //   0, 1, 3, // first triangle
        //   1, 2, 3, // second triangle
        //   4, 5, 6,
        //   4, 6, 7,
        //   2, 3, 6,
        //   3, 6, 7,
        //   0, 1, 5,
        //   0, 4, 5,
        //   0, 3, 7,
        //   0, 4, 7,
        //   1, 2, 6,
        //   1, 5, 6,
        //};

        std::array<float, 72> text_coords_ = {
           0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,

            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,

            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,

            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,

            // down
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f,

            // Up
          0.0f, 1.0f,
          1.0f, 1.0f,
          1.0f, 0.0f,
          1.0f, 0.0f,
          0.0f, 0.0f,
          0.0f, 1.0f
        };

    };

    void HelloSquare::Init()
    {
        // VAO
        glGenVertexArrays(1, &vao_);
        glBindVertexArray(vao_);

        // VBO VERTEX POINT
        glGenBuffers(3, &vbo_[0]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(float) * vertex_point_.size(),
            &vertex_point_,
            GL_STATIC_DRAW);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            3 * sizeof(float),
            0);
        glEnableVertexAttribArray(0);

        // VBO COLOR POINT
        glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(float) * color_point_.size(),
            &color_point_,
            GL_STATIC_DRAW);

        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            3 * sizeof(float),
            0);

        glEnableVertexAttribArray(1);

        // VBO TEXTURE COORD
        glBindBuffer(GL_ARRAY_BUFFER, vbo_[2]);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(float) * text_coords_.size(),
            &text_coords_,
            GL_STATIC_DRAW);

        glVertexAttribPointer(
            2,
            2,
            GL_FLOAT,
            GL_FALSE,
            2 * sizeof(float),
            0);
        glEnableVertexAttribArray(2);

        unsigned char* data = stbi_load(
            "../data/textures/wall.jpg",
            &width_,
            &height_,
            &nrChannels_,
            0);

        glGenTextures(1, &texture_);
        glBindTexture(GL_TEXTURE_2D, texture_);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            width_,
            height_,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        // Mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);



        //// EBO
        /*glGenBuffers(1, &ebo_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(indices_point_),
            indices_point_.data(),
            GL_STATIC_DRAW);*/


            // SHADER
        std::string path = "../";
        std::ifstream ifs_vert(
            path + "data/shaders/hello_transform/transform.vert");
        std::ifstream ifs_frag(
            path + "data/shaders/hello_transform/transform.frag");

        if (!ifs_vert.is_open())
        {
            throw std::runtime_error("Could not open vertex file.");
        }
        if (!ifs_frag.is_open())
        {
            throw std::runtime_error("Could not open fragment file.");
        }

        std::string vertex_source_{
            std::istreambuf_iterator<char>(ifs_vert), {} };
        std::string fragment_source_{
            std::istreambuf_iterator<char>(ifs_frag), {} };

        vertex_shader_ = glCreateShader(GL_VERTEX_SHADER);
        const char* vertex_ptr_ = vertex_source_.c_str();
        glShaderSource(vertex_shader_, 1, &vertex_ptr_, nullptr);
        glCompileShader(vertex_shader_);

        fragment_shader_ = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragment_ptr_ = fragment_source_.c_str();
        glShaderSource(fragment_shader_, 1, &fragment_ptr_, nullptr);
        glCompileShader(fragment_shader_);

        // Program
        program_ = glCreateProgram();
        glAttachShader(program_, vertex_shader_);
        glAttachShader(program_, fragment_shader_);
        glLinkProgram(program_);
    }

    void HelloSquare::Update(seconds dt) {
        // Timer
        timer_ += dt.count();

        // Transform
        SetViewMatrix(dt);
        SetModelMatrix(dt);
        SetProjectionMatrix();
        SetUniformMatrix();

        // Color
        glClearColor(0.82352941f, 0.63137255f, 0.81568627f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_);

        // Program
        glUseProgram(program_);
        const float colorValue = std::sinf(timer_) / 2.0f + 0.5f;
        glUniform1f(glGetUniformLocation(program_, "colorCoeff"), colorValue);
        glBindVertexArray(vao_);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    void HelloSquare::Destroy()
    {
    }

    void HelloSquare::OnEvent(SDL_Event& event)
    {
    }

    void HelloSquare::DrawImGui()
    {
    }

    void HelloSquare::SetUniformMatrix() const
    {
        glUseProgram(program_);

        int location_model = glGetUniformLocation(program_, "model");
        glUniformMatrix4fv(location_model, 1, GL_FALSE, &model_[0][0]);

        int location_view = glGetUniformLocation(program_, "view");
        glUniformMatrix4fv(location_view, 1, GL_FALSE, &view_[0][0]);

        int location_projection = glGetUniformLocation(program_, "projection");
        glUniformMatrix4fv(location_projection, 1, GL_FALSE, &projection_[0][0]);
    }

    void HelloSquare::IsError(const std::string& file, int line) const
    {
        auto error_code = glGetError();
        if (error_code != GL_NO_ERROR)
        {
            std::cerr << error_code <<
                " in file : " << file <<
                " in line : " << line << "\n";
        }
    }

    void HelloSquare::SetModelMatrix(seconds dt)
    {
        timer_ += dt.count();
        model_ = glm::rotate(glm::mat4(1.0f), timer_, glm::vec3(1.0f, 1.0f, 0.0f));
    }

    void HelloSquare::SetViewMatrix(seconds dt)
    {
        position_ += delta_positon_;
        delta_positon_ = glm::vec3(0.0f, 0.0f, 0.0f);
        view_ = glm::lookAt(
            position_, // eye position
            glm::vec3(0.0f, 0.0f, 0.0f), // where it is looking
            glm::vec3(0.0f, 1.0f, 0.0f)); // up vector
    }

    void HelloSquare::SetProjectionMatrix()
    {
        projection_ = glm::perspective(
            glm::radians(65.0f),
            4.0f / 3.0f,
            0.1f,
            100.0f);
    }

}

int main(int argc, char** argv)
{
    gl::HelloSquare program;
    gl::Engine engine(program);
    engine.Run();
    return EXIT_SUCCESS;
}