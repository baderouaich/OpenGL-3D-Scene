#pragma once
#include <GLFW/glfw3.h>

// Mouse codes From glfw3.h
enum class MouseCode : std::uint16_t
{
	Button0 = GLFW_MOUSE_BUTTON_1,
	Button1 = GLFW_MOUSE_BUTTON_2,
	Button2 = GLFW_MOUSE_BUTTON_3,
	Button3 = GLFW_MOUSE_BUTTON_4,
	Button4 = GLFW_MOUSE_BUTTON_5,
	Button5 = GLFW_MOUSE_BUTTON_6,
	Button6 = GLFW_MOUSE_BUTTON_7,
	Button7 = GLFW_MOUSE_BUTTON_8,

	ButtonLast = Button7,
	ButtonLeft = Button0,
	ButtonRight = Button1,
	ButtonMiddle = Button2
};

inline std::ostream& operator<<(std::ostream& os, MouseCode keyCode)
{
	return os << static_cast<std::uint16_t>(keyCode);
}