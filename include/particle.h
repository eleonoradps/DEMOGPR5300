#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

#include "shader.h"
#include "texture.h"
#include "game_object.h"

namespace gl
{
	// Represents a single particle and its state.
	struct Particle
	{
		glm::vec2 position;
		glm::vec2 velocity;
		glm::vec4 color;

		// life variable that slowly decays once its spawned.
		// When life is less than certain threshold (usually 0),
		// particle is killed and replaced with new particle when the next spawns.
		float life;

		Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) {}

		
	};

	class ParticleGenerator
	{
	public:
		ParticleGenerator(Shader shader, Texture texture, unsigned int amount);

		// Update all particles
		void Update(
			float dt,
			GameObject& object,
			unsigned int newParticles,
			glm::vec2 offset = glm::vec2(0.0f, 0.0f));

		//render all particles
		void Draw();

	private:
		//state
		std::vector<Particle> particles;
		unsigned int amount;

		//render state
		Shader shader;
		Texture texture;
		unsigned int VAO;

		//initializes buffer and vertex attributes
		void Init();

		//returns the first particle index that is unused
		// meaning if no particle is currently active
		unsigned int FirstUnusedParticle();

		//respawns particle
		void RespawnParticle(
			Particle& particle,
			GameObject& object,
			glm::vec2 offset = glm::vec2(0.0f, 0.0f));
	};
}