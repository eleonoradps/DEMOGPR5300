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
		void SetModelMatrix(seconds dt);
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

		glm::mat4 model_ = glm::mat4(1.0f);
		glm::mat4 view_ = glm::mat4(1.0f);
		glm::mat4 inv_model_ = glm::mat4(1.0f);
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
		camera_ = std::make_unique<Camera>(glm::vec3(.0f, .0f, 30.0f));
		framebuffer_ = std::make_unique<Framebuffer>();
		cubemaps_ = std::make_unique<Cubemaps>();


		std::string path = "../";
		model_obj_ = std::make_unique<Model>(path + "data/meshes/planet.obj");

		shaders_ = std::make_unique<Shader>(
			path + "data/shaders/hello_scene/model.vert",
			path + "data/shaders/hello_scene/model.frag");

		framebufferShader_ = std::make_unique<Shader>(
			path + "data/shaders/hello_scene/framebuffer.vert",
			path + "data/shaders/hello_scene/framebuffer.frag");

		skyboxShader_ = std::make_unique<Shader>(
			path + "data/shaders/hello_scene/cubemaps.vert",
			path + "data/shaders/hello_scene/cubemaps.frag");

		glClearColor(0.82352941f, 0.63137255f, 0.81568627f, 1.0f);
	}

	void HelloModel::SetModelMatrix(seconds dt)
	{
		model_ = glm::rotate(glm::mat4(1.0f), time_, glm::vec3(0.f, 1.f, 0.f));
		inv_model_ = glm::transpose(glm::inverse(model_));
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
		shaders_->Use();
		shaders_->SetMat4("model", model_);
		shaders_->SetMat4("view", view_);
		shaders_->SetMat4("projection", projection_);
		shaders_->SetMat4("inv_model", inv_model_);
		shaders_->SetVec3("camera_position", camera_->position);
	}

	void HelloModel::Update(seconds dt)
	{
		framebuffer_->Bind();

		SetViewMatrix(dt);
		SetModelMatrix(dt);
		SetProjectionMatrix();
		SetUniformMatrix();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (const auto& mesh_ : model_obj_->meshes)
		{
			// Bind uniform to program.
			mesh_.Bind();
			const auto& material = model_obj_->materials[mesh_.material_index];
			shaders_->Use();
			material.color.Bind(0);
			shaders_->SetInt("Diffuse", 0);
			material.specular.Bind(1);
			shaders_->SetInt("Specular", 1);
			shaders_->SetFloat("specular_pow", material.specular_pow);
			shaders_->SetVec3("specular_vec", material.specular_vec);
			glDrawElements(GL_TRIANGLES, mesh_.nb_vertices_, GL_UNSIGNED_INT, 0);
		}

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
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			glViewport(0, 0, event.window.data1, event.window.data2);
		}
		if (event.type == SDL_MOUSEMOTION)
		{
			const auto mouse_state = SDL_GetMouseState(nullptr, nullptr);
			if (mouse_state & SDL_BUTTON(3))
			{
				camera_->ProcessMouseMovement(event.motion.xrel, event.motion.yrel, true);
			}
		}

		if (event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
				exit(0);
			if (event.key.keysym.sym == SDLK_w)
			{
				camera_->ProcessKeyboard(CameraMovementEnum::FORWARD, delta_time_);
			}
			if (event.key.keysym.sym == SDLK_s)
			{
				camera_->ProcessKeyboard(CameraMovementEnum::BACKWARD, delta_time_);
			}
			if (event.key.keysym.sym == SDLK_a)
			{
				camera_->ProcessKeyboard(CameraMovementEnum::LEFT, delta_time_);
			}
			if (event.key.keysym.sym == SDLK_d)
			{
				camera_->ProcessKeyboard(CameraMovementEnum::RIGHT, delta_time_);
			}
		}
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


//void MaFonction() {
//
//}
//
//int MaFonction2() {
//	return 0;
//}
//
//std::string MaFonction3() {
//	return "";
//}
//
//std::vector<int> MaFonction4() {
//	return std::vector<int>(10);
//}
//
//class MaClass {
//public:
//	void MaMethode(int parama1); //déclaration
//
//	std::unique_ptr<float> MaMethode2();
//};
// 
//void MaClass::MaMethode(int param1) { //définition
//	MaMethode2(); //Appel de fonction
//	std::unique_ptr<float> maVariable = MaMethode2();
//
//	if (*maVariable > 5.0f) {
//
//	}
//
//	std::cout << maVariable << "\n";
//}
//
///*
//*  TypeDeRetour NomDeFonction(Paramètre) 
//*/
//std::unique_ptr<float> MaClass::MaMethode2() {
//	return std::make_unique<float>(2.0f);
//}
//
///**
//* Différence Stack et la Heap
//* 1. La heap est dynamic | La stack static
//* 2. La stack a une mémoire plus petite que la heap. C'est dans la RAM
//* 3. La stack est plus rapide que la heap
//*/
//
//#include <list>
//
//int a = 0;  //Stack
//float b = 0.0f; // Stack
//
//MaClass maClass; //Stack
//std::unique_ptr<MaClass> ptrMaClass;//MaCLass -> Heap. ptr -> Stack
//
//std::unique_ptr<int> ptrInt; //int -> heap. ptr -> Stack
//
//std::vector<int> vecInts; //tous les ints -> heap. std::vector -> Stack
//
//std::array<int, 10> arrInts; //Tous les nits -> stacks. std::array -> Stack
//
//std::list<int> listInst; //tous les ints ->heap. std::list -> Stack
//
//std::unique_ptr<std::vector<int>> ptrVecInts; //TOus les ints -> Heap, std::vector -> heap, ptr -> stack
//
//std::string s; //Chars -> heap, 
//
//
//
//void MaFonction() {
//
//}
//
//int MaFonction2() {
//	return 0;
//}
//
//std::string MaFonction3() {
//	return "";
//}
//
//std::vector<int> MaFonction4() {
//	return std::vector<int>(10);
//}
//
//class MaClass {
//public:
//	void MaMethode(int parama1); //déclaration
//
//	std::unique_ptr<float> MaMethode2();
//};
//
//void MaClass::MaMethode(int param1) { //définition
//	MaMethode2(); //Appel de fonction
//	std::unique_ptr<float> maVariable = MaMethode2();
//
//	if (*maVariable > 5.0f) {
//
//	}
//
//	std::cout << maVariable << "\n";
//}
//
///*
//*  TypeDeRetour NomDeFonction(Paramètre)
//*/
//std::unique_ptr<float> MaClass::MaMethode2() {
//	return std::make_unique<float>(2.0f);
//}
//
///**
//* Différence Stack et la Heap
//* 1. La heap est dynamic | La stack static
//* 2. La stack a une mémoire plus petite que la heap. C'est dans la RAM
//* 3. La stack est plus rapide que la heap
//*/
//
//#include <list>
//
//int a = 0;  //Stack
//float b = 0.0f; // Stack
//
//MaClass maClass; //Stack
//std::unique_ptr<MaClass> ptrMaClass;//MaCLass -> Heap. ptr -> Stack
//
//std::unique_ptr<int> ptrInt; //int -> heap. ptr -> Stack
//
//std::vector<int> vecInts; //tous les ints -> heap. std::vector -> Stack
//
//std::array<int, 10> arrInts; //Tous les nits -> stacks. std::array -> Stack
//
//std::list<int> listInst; //tous les ints ->heap. std::list -> Stack
//
//std::unique_ptr<std::vector<int>> ptrVecInts; //TOus les ints -> Heap, std::vector -> heap, ptr -> stack
//
//std::string s; //Chars -> heap, 
