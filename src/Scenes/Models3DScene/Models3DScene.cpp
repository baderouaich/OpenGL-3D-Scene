#include "pch.hpp"
#include "Models3DScene.hpp"
#include <Application/Application.hpp>
#include <memory>
#include <Graphics/Shader/Shader.hpp>
#include <Graphics/Buffer/VertexArray/VertexArray.hpp>
#include <Graphics/Buffer/VertexBuffer/VertexBuffer.hpp>
#include <glad/glad.h>
#include <Scenes/StaticTriangleScene/StaticTriangleScene.hpp>
#include <portable-file-dialogs.h>
#include <ResourceManager/ResourceManager.hpp>

void Models3DScene::OnCreate()
{
	TRACE_FUNCTION();

  // Init Shaders
  m_shader.reset(new Shader({
                          {Shader::ShaderType::Vertex, Shader::ShaderSourceType::File, "res/shaders/default/default.vert"},
                          {Shader::ShaderType::Fragment, Shader::ShaderSourceType::File,"res/shaders/default/default.frag"},
                        }));

  // Init Camera
  m_camera.reset(new Camera(
            glm::vec3(0.0f), // position
            glm::vec3(0.0f, 1.0f, 0.0f) // world up
          ));


  // Init Matrices
  {
    // Init View Matrix
    m_view_matrix = glm::mat4(1.0f);
    m_view_matrix = glm::lookAt(m_camera->GetPosition(), m_camera->GetPosition() + m_camera->GetFront(),
                                m_camera->GetWorldUp());


    //Projection Matrix
    m_projection_matrix = glm::mat4(1.0f);
    m_projection_matrix = glm::perspective(
            glm::radians(this->m_fov),
            Application::GetInstance()->GetWindow()->GetAspectRatio(),
            this->m_zNear,
            this->m_zFar
    );
  }

  // Init Textures & materials (TODO: resources manager->textures|shaders|models...)
  //ResourceManager::LoadTexture(fs::path(TEXTURES_DIR) / "box.png", Texture::TextureType::Texture2D, "box");
  ResourceManager::LoadTexture(fs::path(TEXTURES_DIR) / "concrete.png", Texture::TextureType::Texture2D, "concrete");
  //ResourceManager::LoadTexture(fs::path(TEXTURES_DIR) / "box_specular.png", Texture::TextureType::Texture2D, "box_specular");
  ResourceManager::LoadTexture(fs::path(TEXTURES_DIR) / "concrete_specular.png", Texture::TextureType::Texture2D, "concrete_specular");
  // Init Materials
  m_material.reset(new Material(
          glm::vec3(0.3f),
          glm::vec3(1.0f),
          glm::vec3(1.0f), // increase FOR wider strong light
          0, // tex id "box"
          1 // tex id "box_specular"
  ));



  // Init Model (default until user loads an .obj model from gui)
  // Add ground mesh (scaledx100 plane quad rotated -90 degrees to become a ground floor)
  // Plane mesh (scaledx100 plane quad rotated -90 degrees to become a ground floor)
   auto ground_mesh = std::make_shared<Mesh>(std::make_shared<Quad>(), glm::vec3(0.0f), glm::vec3(0.f), glm::vec3(-90.0f, 0.0f, 0.0f), glm::vec3(50.0f));
  //std::make_shared<Mesh>(Pyramid{}, glm::vec3(0.0f), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.f)),
  m_model.reset(new Model(
          m_camera->GetPosition() + glm::vec3(0.0f, -2.0f, 0.0f), // model will be placed in bellow of camera (upy = -2.0f)
          m_material,
          ResourceManager::GetTexture("concrete"),
          ResourceManager::GetTexture("concrete_specular")
          ));
  m_model->AddMesh(ground_mesh);

  // Init Lights
  glm::vec4 light_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
  m_point_light.reset(new PointLight(m_camera->GetPosition(), 10.0f, light_color));


  // Init Shader Uniforms
  m_shader->SetMat4("view_matrix", m_view_matrix, false);
  // Send ProjectionMatrix to vertex shader
  m_shader->SetMat4("projection_matrix", m_projection_matrix, false);
  // Send camera position to fragment Shader to calculate point light position
  m_shader->SetVec3f("camera_position", m_camera->GetPosition());
  // Send Lights to fragment shader
  m_point_light->SendToShader(m_shader);


  // Hide arrow cursor to give camera fps view
   Application::GetInstance()->GetWindow()->DisableCursor();
}

void Models3DScene::OnEvent(Event& event)
{
  EventDispatcher dispatcher(event);
  static float last_mouse_x = Input::GetMouseX();
  static float last_mouse_y = Input::GetMouseX();

  // Handle mouse motion to move camera
  dispatcher.Dispatch<MouseMotionEvent>([this](const MouseMotionEvent& e) -> bool
    {
        const float mouse_offset_x = e.GetX() - last_mouse_x;
        const float mouse_offset_y = last_mouse_y - e.GetY(); // reversed since y-coordinates go from bottom to top
        const float dt = Application::GetInstance()->GetDeltaTime();

        m_camera->UpdateMouseInput(dt, mouse_offset_x, mouse_offset_y);
        last_mouse_x = e.GetX();
        last_mouse_y = e.GetY();
        return true;

    });


  dispatcher.Dispatch<KeyPressedEvent>([this](KeyPressedEvent& e) -> bool
   {
        // Toggle menu ESCAPE
       if (e.GetKeyCode() == KeyCode::Escape)
       {
         m_menu_visible = !m_menu_visible;
         if(m_menu_visible) {
           Application::GetInstance()->GetWindow()->ShowCursor();
         } else {
           Application::GetInstance()->GetWindow()->DisableCursor();
         }
         return true;
       }
       return false;
   });



  // Move Light to current camera position when Mouse button is pressed
//  dispatcher.Dispatch<MouseButtonPressedEvent>([this](const MouseButtonEvent& e) -> bool
//   {
//      if(e.GetMouseButton() == MouseCode::ButtonLeft)
//      {
//        m_point_light->SetPosition(m_camera->GetPosition());
//        return true;
//      }
//      return false;
//   });

}

void Models3DScene::OnUpdate(float dt)
{
  // Handle keyboard keys held events for camera movement
  // Don't do this in OnEvent because it only dispatches key press/release, we want current state of keyboard every frame
  if (Input::IsKeyPressed(KeyCode::W))
  {
    m_camera->Move(dt, Direction::FORWARD);  //MOVE CAMERA FORWARD (in screen)
  }
  if (Input::IsKeyPressed(KeyCode::S))
  {
    m_camera->Move(dt, Direction::BACKWARD); //MOVE CAMERA BACKWARD (out of screen)
  }
  if (Input::IsKeyPressed(KeyCode::A))
  {
    m_camera->Move(dt, Direction::LEFT); //MOVE CAMERA LEFT
  }
  if (Input::IsKeyPressed(KeyCode::D))
  {
    m_camera->Move(dt, Direction::RIGHT); //MOVE CAMERA RIGHT
  }
  if (Input::IsKeyPressed(KeyCode::Up))
  {
    m_camera->Move(dt, Direction::UP); //MOVE CAMERA UP
  }
  if (Input::IsKeyPressed(KeyCode::Down))
  {
    m_camera->Move(dt, Direction::DOWN); //MOVE CAMERA Down
  }



  // Update uniforms TODO: put in a separated func
  // ViewMatrix
  {
    //ViewMatrix = glm::lookAt(camPosition, camPosition + camFront, worldUp);
    m_view_matrix = m_camera->GetViewMatrix();
    m_shader->SetMat4("view_matrix", m_view_matrix, false);
  }
  // ProjectionMatrix
  {
    // Update Frame buffer size & perspective matrix
    m_projection_matrix = glm::perspective(
            glm::radians(this->m_fov),
            Application::GetInstance()->GetWindow()->GetAspectRatio(),
            this->m_zNear,
            this->m_zFar
    );
    m_shader->SetMat4("projection_matrix", m_projection_matrix, false);
  }

  //Camera Position
  m_shader->SetVec3f("camera_position", m_camera->GetPosition());

  // Update Lights
  m_point_light->SetPosition(m_camera->GetPosition()); // light moves with ur camera, you are the beam!
  m_point_light->SendToShader(m_shader);

//  //UPDATE SHADER UNIFORMS
//  m_shader->SetInteger("texture0", 0); // box texture
//  m_shader->SetInteger("texture1", 1); // box_specular texture


  // Update our model
  m_model->Update(dt);
}

void Models3DScene::OnDraw()
{
	glAssert(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
	glAssert(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

//  glAssert(glEnable(GL_DEPTH_TEST));
//  glAssert(glEnable(GL_CULL_FACE));
//  glAssert(glEnable(GL_BLEND)); // ENABLE BLENDING
  {
    m_model->Draw(m_shader);
  }
//  glAssert(glDisable(GL_DEPTH_TEST));
//  glAssert(glDisable(GL_CULL_FACE));
//  glAssert(glDisable(GL_BLEND));
}

void Models3DScene::OnImGuiDraw()
{
  if(not m_menu_visible) return;

  ImGui::Begin("3D Model");
  {
//    static char model_path[PATH_MAX]{};
//    ImGui::InputTextWithHint("Obj File path", "/path/to/3d/obj/model", model_path, sizeof(model_path));
    if (ImGui::Button("Load 3D Model (.obj)")) {
        pfd::open_file of("Select a 3D Model .obj File", MODELS_DIR, { "Object Files", "*.obj *.object"}, pfd::opt::none);
        if(!of.result().empty()){
          std::cout << of.result()[0] << std::endl;
          std::string obj_filename = of.result()[0];
          if(fs::exists(obj_filename) && fs::is_regular_file(obj_filename))
          {
            //why instead not just load mess add mesh to
            std::vector<Vertex> vertices = OBJLoader::LoadOBJ(obj_filename);
            std::shared_ptr<Mesh> mesh(new Mesh(vertices,{}, m_camera->GetPosition(), glm::vec3(0.f), glm::vec3(0.f), glm::vec3(1.0f)));
            m_model->AddMesh(mesh);

            /*
            m_model.reset(new Model(
                    glm::vec3(0.0, -2.0f, 0.0), // when new model is reloaded, should we reset position to 0.0 ?
                    //m_camera->GetPosition(),
                    m_material,
                    ResourceManager::GetTexture("concrete"),
                    ResourceManager::GetTexture("concrete_specular"),
                    obj_filename
            ));*/
          }

        }

    }
    ImGui::End();
  }


  ImGui::Begin("Camera");
  {
    ImGui::InputFloat3("Camera Position", glm::value_ptr(m_camera->GetPosition()), "%.3f");
    ImGui::End();
  }

  ImGui::Begin("Model");
  {
    ImGui::InputFloat3("Model Position", glm::value_ptr(m_model->GetPosition()), "%.3f");
    int i = 0;
    for(const std::shared_ptr<Mesh>& mesh : m_model->GetMeshes())
    {
      i++;
      ImGui::Separator();
      ImGui::InputFloat3(("Mesh Position " + std::to_string(i)).data(), glm::value_ptr(mesh->getPosition()), "%.3f");
      ImGui::SliderFloat3(("Mesh Rotation " + std::to_string(i)).data(), glm::value_ptr(mesh->getRotation()), -180.0f, 180.0f, "%.3f");

    }

    ImGui::End();
  }

  ImGui::Begin("Light");
  {
    ImGui::InputFloat3("Light Position", glm::value_ptr(m_point_light->GetPosition()), "%.3f");

    ImGui::End();
  }

  ImGui::Begin("Projection");
  {
    ImGui::SliderFloat("zNear",&m_zNear, 0.0f, 1000.0f, "%.3f");
    ImGui::SliderFloat("zFar",&m_zFar, 0.0f, 1000.0f, "%.3f");
    ImGui::SliderFloat("FOV", &m_fov, 0.0f, 180.0f, "%.3f");

    ImGui::End();
  }


  ImGui::Begin("Scene");
  {
    if(ImGui::Button("Exit")){
      this->Exit();
    }

    ImGui::End();
  }
}

void Models3DScene::OnDestroy()
{
	TRACE_FUNCTION();
  Application::GetInstance()->GetWindow()->ShowCursor();
  m_shader->Unbind();
  ResourceManager::Clear();
}
