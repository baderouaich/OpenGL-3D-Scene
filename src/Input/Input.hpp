#pragma once
#include <Input/Keyboard/KeyCodes.hpp>
#include <Input/Mouse/MouseCodes.hpp>

class Input
{
public: /* Keyboard */
	static bool IsKeyPressed(KeyCode keycode) noexcept;

public: /* Mouse */
	static bool IsMouseButtonPressed(MouseCode button) noexcept;
	static std::pair<float, float> GetMousePosition() noexcept;
	static float GetMouseX() noexcept;
	static float GetMouseY() noexcept;
};
