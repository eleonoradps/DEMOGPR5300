#include "particle.h"

namespace gl
{
	//Ask
	/*ParticleGenerator::ParticleGenerator(Shader shader, Texture texture, unsigned int amount) :
		shader(shader), texture(texture), amount(amount)
	{
		this->Init();
	}*/

	void ParticleGenerator::Update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset)
	{
		//add new particles
		for (unsigned int i = 0; i < newParticles; i++)
		{
			// FirstUnusedParticles tries to find the first particle
			//that is dead and returns its index to the caller
			int unusedParticle = this->FirstUnusedParticle();
			this->RespawnParticle(this->particles[unusedParticle], object, offset);

		}

		//update all particles
		for (unsigned int i = 0; i < this->amount; i++)
		{
			Particle& p = this->particles[i];
			p.life -= dt; // reduce particle life
			if (p.life > 0.0f)
			{
				// particle is alive : update
				p.position -= p.velocity * dt;
				p.color.a -= dt * 2.5f;
			}
		}
	}

	//render all particles
	void ParticleGenerator::Draw()
	{

	}
}