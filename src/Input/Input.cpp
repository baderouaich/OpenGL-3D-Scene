#include <pch.hpp>
#include "Input.hpp"
#include <Application/Application.hpp>
#include <GLFW/glfw3.h>

bool Input::IsKeyPressed(KeyCode keycode) noexcept
{
	GLFWwindow* window = Application::GetInstance()->GetWindow()->GetGLFWwindow();
	std::int32_t state = glfwGetKey(window, static_cast<std::int32_t>(keycode));
	return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(MouseCode button) noexcept
{
	GLFWwindow* window = Application::GetInstance()->GetWindow()->GetGLFWwindow();
	std::int32_t state = glfwGetMouseButton(window, static_cast<std::int32_t>(button));
	return state == GLFW_PRESS;
}

std::pair<float, float> Input::GetMousePosition() noexcept
{
	GLFWwindow* window = Application::GetInstance()->GetWindow()->GetGLFWwindow();
	double xPos{0.0}, yPos{0.0};
	glfwGetCursorPos(window, &xPos, &yPos);
	return std::make_pair( static_cast<float>(xPos), static_cast<float>(yPos) );
}

float Input::GetMouseX() noexcept
{
	const auto [x, y] = GetMousePosition();
	return x;
}

float Input::GetMouseY() noexcept
{
	const auto [x, y] = GetMousePosition();
	return y;
}
