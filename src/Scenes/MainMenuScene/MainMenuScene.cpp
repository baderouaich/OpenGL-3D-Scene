#include "pch.hpp"
#include "MainMenuScene.hpp"
#include <Application/Application.hpp>
#include <memory>
#include <Graphics/Shader/Shader.hpp>
#include <Graphics/Buffer/VertexArray/VertexArray.hpp>
#include <Graphics/Buffer/VertexBuffer/VertexBuffer.hpp>
#include <glad/glad.h>
#include <Scenes/StaticTriangleScene/StaticTriangleScene.hpp>
#include <Scenes/Models3DScene/Models3DScene.hpp>


void MainMenuScene::OnCreate()
{
	TRACE_FUNCTION();

}

void MainMenuScene::OnEvent([[maybe_unused]] Event& event)
{

}

void MainMenuScene::OnUpdate([[maybe_unused]] float dt)
{

}

void MainMenuScene::OnDraw()
{
	glAssert(glClearColor(0.434F + 40,0.434F + 30,0.434F+40, 1.0f));
	glAssert(glClear(GL_COLOR_BUFFER_BIT));
}

void MainMenuScene::OnImGuiDraw()
{
	const auto win_size = Application::GetInstance()->GetWindow()->GetSize();
	[[maybe_unused]] const auto win_pos = Application::GetInstance()->GetWindow()->GetPosition();

	ImGui::Begin(typeid(this).name(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
	ImGui::SetWindowPos(ImVec2{ 0.0f, 0.0f });
	ImGui::SetWindowSize(ImVec2{ static_cast<float>(win_size.x), static_cast<float>(win_size.y) });
	{
		static constexpr const char* title = "OpenGL Playground Scenes Menu";
		static const auto title_size = ImGui::CalcTextSize(title);
		ImGui::SetCursorPos(ImVec2((win_size.x - title_size.x) / 2.0f, 60.0f));
		ImGui::Text(title);

		const auto button_size = ImVec2(win_size.x * 0.30f, 36.0f);
		[[maybe_unused]] float y = 70.0f, margin_y = 70.0f;

		ImGui::SetCursorPos(ImVec2((win_size.x - button_size.x) / 2.0f, y += margin_y));
		if (ImGui::Button("Start", button_size))
		{
		}
		ImGui::SetCursorPos(ImVec2((win_size.x - button_size.x) / 2.0f, y += margin_y));
		if (ImGui::Button("Static Triangle", button_size))
		{
			Application::GetInstance()->PushScene(std::make_unique<StaticTriangleScene>());
		}
    /*
		ImGui::SetCursorPos(ImVec2((win_size.x - button_size.x) / 2.0f, y += margin_y));
		if (ImGui::Button("Pyramid 3D", button_size))
		{
			Application::GetInstance()->PushScene(std::make_unique<Pyramid3DScene>());
		}*/
		ImGui::SetCursorPos(ImVec2((win_size.x - button_size.x) / 2.0f, y += margin_y));
		if (ImGui::Button("3D Models", button_size))
		{
			Application::GetInstance()->PushScene(std::make_unique<Models3DScene>());
		}
    /*
		ImGui::SetCursorPos(ImVec2((win_w - button_size.x) / 2.0f, y += 50.0f));
		if (ImGui::Button("Exercices", button_size))
		{
			Application::GetInstance()->PushScene(std::make_unique<ExercicesScene>());
		}
  */

		ImGui::SetCursorPos(ImVec2((win_size.x - button_size.x) / 2.0f, win_size.y - button_size.y - 40.0f));
		if (ImGui::Button("Exit", button_size))
		{
			Scene::Exit();
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Text("**NOTE**: Press ESCAPE to exit from a scene");
	}
	ImGui::End();

}

void MainMenuScene::OnDestroy()
{
	TRACE_FUNCTION();

}
