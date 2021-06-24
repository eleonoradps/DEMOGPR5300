#pragma once

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <stb_image.h>
//#include <GLFW/glfw3.h>

#include "shader.h"

namespace gl {
	class Instancing {
    public:
        unsigned int amount = 100;
        glm::mat4 *modelMatrices;
        float radius = 50.0f;
        float offset = 2.5f;

        // Initialize random seed
        //srand(glfwGetTime())

		Instancing() {
            //glm::mat4* modelMatrices;
            modelMatrices = new glm::mat4[amount];
            
            for (unsigned int i = 0; i < amount; i++)
            {
                // We transform the x and y position of the asteroid along a circle
                // with a radius defined by radius and randomly displace each obj 
                // a little around the circle by -offset and offset.

                // We create a more flat obj ring by giving y displacement less impact

                // We apply scale and rotation transformations and store resulting
                // transformation matrix in modelMatrices that is size of amount.

                // Generate 1000 model matrices, one per obj


                glm::mat4 model = glm::mat4(1.0f);
                
                // Translation : displace along circle with radius in range[-0ffset, offset]
                float angle = (float)i / (float)amount * 360.0f;
                float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;

                float x = sin(angle) * radius + displacement;
                displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;

                // Keep height of field smaller compared to width of x and z
                float y = displacement * 0.4f;
                displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;

                float z = cos(angle) * radius + displacement;
                model = glm::translate(model, glm::vec3(x, y, z));

                // Scale : scale between 0.05 and 0.25
                float scale = (rand() % 20) / 100.0f + 0.05f;
                model = glm::scale(model, glm::vec3(scale));

                // Rotation : add random rotation around a (semi) randomly picked rotation axis vector
                float rotAngle = (rand() % 360);
                model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

                // Add to list of matrices
                modelMatrices[i] = model;
            }
            
		}
	};
}