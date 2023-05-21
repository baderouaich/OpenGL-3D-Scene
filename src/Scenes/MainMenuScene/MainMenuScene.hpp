#pragma once
#include "Scenes/Scene.hpp"
#include <Graphics/Texture/Texture.hpp>
#include <Graphics/Shader/Shader.hpp>

class MainMenuScene : public Scene
{
public:
	MainMenuScene() = default;
	~MainMenuScene() override = default;

public: /* Scene Life Cycle */
	void OnCreate() override;
	void OnEvent(Event& event) override;
	void OnUpdate(float dt)  override;
	void OnDraw()  override;
	void OnImGuiDraw()  override;
	void OnDestroy()  override;

};

