#pragma once
#include "Scenes/Scene.hpp"
#include <Graphics/Texture/Texture.hpp>
#include <Graphics/Shader/Shader.hpp>
#include <Graphics/Buffer/VertexBuffer/VertexBuffer.hpp>
#include <Graphics/Buffer/VertexArray/VertexArray.hpp>
#include <Graphics/Buffer/IndexBuffer/IndexBuffer.hpp>
#include <Primitives/Primitives.hpp>
#include <Model/Mesh.hpp>
#include <Camera/Camera.hpp>
#include <Model/Material.hpp>
#include <Model/Model.hpp>
#include <Lights/PointLight.hpp>

class Models3DScene : public Scene
{
public:
  Models3DScene() = default;
	~Models3DScene() override = default;

public: /* Scene Life Cycle */
	void OnCreate() override;
	void OnEvent(Event& event) override;
	void OnUpdate(float dt)  override;
	void OnDraw()  override;
	void OnImGuiDraw()  override;
	void OnDestroy()  override;

private:
    glm::mat4 m_view_matrix;
    glm::mat4 m_projection_matrix;
    // Projection Variables
    float m_fov = 90.0f; // field of view 90 degrees
    float m_zNear = 0.1f; // min distance to view
    float m_zFar = 100.0f; // max depth to view

    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<Material> m_material;
    std::shared_ptr<Model> m_model;
    std::shared_ptr<PointLight> m_point_light;
    //todo: Skybox
    //todo: Show gui settings (enable back cursor)


    bool m_menu_visible = false;
};

