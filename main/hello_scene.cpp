#include <SDL_main.h>
#include <glad/glad.h>
#include <array>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define TINYOBJLOADER_IMPLEMENTATION

#include "framebuffer.h"
#include "cubemaps.h"
#include "engine.h"
#include "camera.h"
#include "texture.h"
#include "shader.h"
#include "model.h"

namespace gl {
	class HelloModel : public Program
	{
	public:
		void Init() override;
		void Update(seconds dt) override;
		void Destroy() override;
		void OnEvent(SDL_Event& event) override;
		void DrawImGui() override;

	protected:
		void SetViewMatrix(seconds dt);
		void SetProjectionMatrix();
		void IsError(const std::string& file, int line) const;
		void SetUniformMatrix() const;

	protected:
		unsigned int vertex_shader_;
		unsigned int fragment_shader_;
		unsigned int program_;

		// FBO framebuffer object
		unsigned int fbo_;


		float time_ = 0.0f;
		float delta_time_ = 0.0f;

		std::unique_ptr<Camera> camera_ = nullptr;
		std::unique_ptr<Shader> shaders_ = nullptr;
		std::unique_ptr<Model> model_obj_ = nullptr;
		std::unique_ptr<Framebuffer> framebuffer_ = nullptr;
		std::unique_ptr<Shader> framebufferShader_ = nullptr;
		std::unique_ptr<Shader> skyboxShader_ = nullptr;
		std::unique_ptr<Cubemaps> cubemaps_ = nullptr;
		std::unique_ptr<Shader> dirLightShader_ = nullptr;
		std::unique_ptr<Shader> normalMapShader_ = nullptr;

		glm::mat4 model_ = glm::mat4(1.0f);
		glm::mat4 view_ = glm::mat4(1.0f);
		glm::mat4 projection_ = glm::mat4(1.0f);
	};

	void HelloModel::IsError(const std::string& file, int line) const
	{
		auto error_code = glGetError();
		if (error_code != GL_NO_ERROR)
		{
			std::cerr
				<< error_code
				<< " in file: " << file
				<< " at line: " << line
				<< "\n";
		}
	}

	void HelloModel::Init()
	{
		glEnable(GL_DEPTH_TEST);
		camera_ = std::make_unique<Camera>(glm::vec3(50.0f, 90.0f, 50.0f));
		framebuffer_ = std::make_unique<Framebuffer>();
		cubemaps_ = std::make_unique<Cubemaps>();


		std::string path = "../";
		model_obj_ = std::make_unique<Model>(path + "data/meshes/mountain.obj");

		shaders_ = std::make_unique<Shader>(
			path + "data/shaders/hello_scene/model.vert",
			path + "data/shaders/hello_scene/model.frag");

		framebufferShader_ = std::make_unique<Shader>(
			path + "data/shaders/hello_scene/framebuffer.vert",
			path + "data/shaders/hello_scene/framebuffer.frag");

		skyboxShader_ = std::make_unique<Shader>(
			path + "data/shaders/hello_scene/cubemaps.vert",
			path + "data/shaders/hello_scene/cubemaps.frag");

		dirLightShader_ = std::make_unique<Shader>(
			path + "data/shaders/hello_scene/dirlight.vert",
			path + "data/shaders/hello_scene/dirlight.frag");

		normalMapShader_ = std::make_unique<Shader>(
			path + "data/shaders/hello_scene/normalmap.vert",
			path + "data/shaders/hello_scene/normalmap.frag");

		glClearColor(0.82352941f, 0.63137255f, 0.81568627f, 1.0f);
	}

	void HelloModel::SetViewMatrix(seconds dt)
	{
		view_ = camera_->GetViewMatrix();
	}

	void HelloModel::SetProjectionMatrix()
	{
		projection_ = glm::perspective(glm::radians(45.0f),
			4.0f / 3.0f,
			0.1f,
			100.f);
	}

	void HelloModel::SetUniformMatrix() const
	{
		normalMapShader_->Use();
		normalMapShader_->SetMat4("view", view_);
		normalMapShader_->SetMat4("projection", projection_);
		normalMapShader_->SetVec3("camera_position", camera_->position);
		normalMapShader_->SetVec3("viewPos", camera_->position);
		normalMapShader_->SetVec3("lightPos",  -0.2f, -1.0f, -0.3f);
	}

	void HelloModel::Update(seconds dt)
	{
		delta_time_ = dt.count();
		framebuffer_->Bind();

		float speed = 1.0f;
		camera_->position += glm::normalize(-camera_->front) * delta_time_ * speed;
		SetViewMatrix(dt);
		SetProjectionMatrix();
		SetUniformMatrix();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		model_obj_->SetModelMatrix(glm::vec3(0, 90, 0));
		model_obj_->Update(*normalMapShader_);

		model_obj_->SetModelMatrix(glm::vec3(80, 90, 0));
		model_obj_->Update(*normalMapShader_);

		model_obj_->SetModelMatrix(glm::vec3(40, 90, 0));
		model_obj_->Update(*normalMapShader_);

		model_obj_->SetModelMatrix(glm::vec3(10, 90, 80));
		model_obj_->Update(*normalMapShader_);

		model_obj_->SetModelMatrix(glm::vec3(80, 90, 80));
		model_obj_->Update(*normalMapShader_);

		model_obj_->SetModelMatrix(glm::vec3(-80, 90, 0));
		model_obj_->Update(*normalMapShader_);

		model_obj_->SetModelMatrix(glm::vec3(-10, 90, -80));
		model_obj_->Update(*normalMapShader_);

		model_obj_->SetModelMatrix(glm::vec3(-80, 90, -80));
		model_obj_->Update(*normalMapShader_);


		// Skybox
		glDepthFunc(GL_LEQUAL);
		cubemaps_->Bind();
		skyboxShader_->Use();
		skyboxShader_->SetInt("skybox", 0);
		skyboxShader_->SetMat4("projection", glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f));
		glm::mat4 view = glm::mat4(glm::mat3(camera_->GetViewMatrix()));
		skyboxShader_->SetMat4("view", view);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Framebuffer
		framebuffer_->Unbind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		framebufferShader_->Use();
		framebufferShader_->SetInt("screenTexture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, framebuffer_->GetColorBuffer());
		framebuffer_->Draw();
		
	}

	void HelloModel::Destroy()
	{
	}

	void HelloModel::OnEvent(SDL_Event& event)
	{
	}

	void HelloModel::DrawImGui()
	{
	}

} // End namespace gl.

int main(int argc, char** argv)
{
	gl::HelloModel program;
	gl::Engine engine(program);
	engine.Run();
	return EXIT_SUCCESS;
}