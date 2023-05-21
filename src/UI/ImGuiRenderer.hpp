#pragma once

#define USING_DOCKING_BRANCH false

/**
*	Ref: examples/example_glfw_opengl3/main.cpp
*/
class ImGuiRenderer final
{
public:
	// Constructor Initializes imgui context
	ImGuiRenderer();

	// Destructor Shuts down imgui context
	~ImGuiRenderer();

	// Begin imgui rendering
	void Begin();

	// End imgui rendering
	void End();
};

