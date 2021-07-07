#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <iostream>
#include <stb_image.h>
#include <vector>
#include <chrono>
#include "model.h"
#include "shader.h"
#include "material.h"
//#include <GLFW/glfw3.h>

#include "shader.h"

namespace gl {
	class Instancing {
    public:
        unsigned int nbAsteroid_ = 100;
        float thicknessAsteroidX_;
        float thicknessAsteroidY_;
        int densityAsteroid_ = 1000;
        unsigned int instanceVBO_;
        std::unique_ptr<Model> model_ = nullptr;

        glm::vec3 transVec_ = glm::vec3(0.0f, 0.0f, 0.0f);
        std::vector<glm::mat4> modelMatrix_;
        std::vector<float> initTransDistanceX_;
        std::vector<float> initTransDistanceY_;

        void SetModelMatrix(std::chrono::duration<float, std::ratio <1, 1>> dt, unsigned int i)
        {
            modelMatrix_[i] = glm::mat4(1.0f);
            modelMatrix_[i] = glm::translate(modelMatrix_[i],
                transVec_ + glm::vec3(initTransDistanceX_[i], initTransDistanceY_[i],
                    0.0f));
        }

        glm::vec3 GetPosition(unsigned int i)
        {
            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 translation;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(modelMatrix_[i], scale, rotation, translation, skew, perspective);

            return translation;
        }
        Instancing() = default;
		Instancing(std::string filepath) {
            initTransDistanceX_.resize(nbAsteroid_);
            std::generate(initTransDistanceX_.begin(), initTransDistanceX_.end(), [&]()
                {
                    return (rand() % densityAsteroid_) / (densityAsteroid_ / thicknessAsteroidX_);
                });

            initTransDistanceY_.resize(nbAsteroid_);
            std::generate(initTransDistanceY_.begin(), initTransDistanceY_.end(), [&]()
                {
                    return (rand() % densityAsteroid_) / (densityAsteroid_ / thicknessAsteroidY_);
                });

            modelMatrix_.resize(nbAsteroid_, glm::mat4(1.0f));
            model_ = std::make_unique<Model>(filepath);
            const auto& asteroidMesh = model_->Model::GetMesh(0);

            glBindVertexArray(asteroidMesh.GetVAO());

            // VBO instancing
            glGenBuffers(1, &instanceVBO_);

            glBindBuffer(GL_ARRAY_BUFFER, instanceVBO_);
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void Update(std::chrono::duration<float, std::ratio <1, 1>> dt, Shader& shader)
        {
            //shader.Use();
            shader.SetInt("TexDiffuse", 0);
            shader.SetInt("TexNormal", 1);
            // Update asteroids model matrix
            for (unsigned int i = 0; i < modelMatrix_.size(); i++)
            {
                SetModelMatrix(dt, i);
            }
            Mesh mesh = model_->GetMesh(0);
            mesh.Bind();
            const auto& material = model_->materials[mesh.material_index];
            material.color.Bind(0);
            material.specular.Bind(1);
            shader.SetFloat("specular_pow", material.specular_pow);
            shader.SetVec3("specular_vec", material.specular_vec);
            glBindBuffer(GL_ARRAY_BUFFER, instanceVBO_);
            glBufferData(GL_ARRAY_BUFFER,
                sizeof(glm::mat4) * modelMatrix_.size(),
                &modelMatrix_[0],
                GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(mesh.GetVAO());
            glDrawElementsInstanced(GL_TRIANGLES,
                mesh.nb_vertices_,
                GL_UNSIGNED_INT,
                0,
                modelMatrix_.size());
            glBindVertexArray(0);

        }

       /* void Draw() const
        {

        }*/
	};
}