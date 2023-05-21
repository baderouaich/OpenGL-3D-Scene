#pragma once
#include <Window/Window.hpp>
#include <memory>
#include <vector>
#include <cstdint>


#define BIND_FUN(fun) [this](auto&&... args) -> decltype(auto) { return this->fun(std::forward<decltype(args)>(args)...); }

#ifdef _DEBUG
	#define TRACE_FUNCTION() std::cout << fs::path(__FILE__).filename().string() << ':' << __LINE__ << ": "<< __PRETTY_FUNCTION__ << std::endl;
#else
	#define TRACE_FUNCTION() 
#endif

class Scene;
class ImGuiRenderer;
class Event;
class WindowCloseEvent;
class WindowResizeEvent;
class FrameBufferResizeEvent;

class Application
{
public:
	Application();
	~Application();

public: 
	// App Loop
	void Run();
	
	// Push new scene to back of the stack
	void PushScene(std::unique_ptr<Scene> scene);

	// Update dt
	void UpdateDeltaTime() noexcept;

	// Update FPS
	void UpdateFPS() noexcept;

public:
	void OnEvent(Event& event);
	bool OnWindowClose(WindowCloseEvent& event);
  bool OnWindowResize(WindowResizeEvent& event);
	bool OnFrameBufferResize(FrameBufferResizeEvent& event);

public:
	static Application* GetInstance() noexcept { return m_instance; }
	[[nodiscard]] const std::unique_ptr<Window>& GetWindow() const noexcept { return m_window; }
	[[nodiscard]] float GetDeltaTime() const noexcept { return m_delta_time; }

private: // Window
	std::unique_ptr<Window> m_window;

private: // UI
	std::unique_ptr<ImGuiRenderer> m_imgui_renderer;

private: // Scenes
	std::vector<std::unique_ptr<Scene>> m_scenes;

private: // Delta Time
	float m_delta_time{ 0.0f };
	float m_current_frame_time{ 0.0f };
	float m_last_frame_time{ 0.0f };

private: // FPS
	std::uint32_t m_FPS{ 0 };
	float m_FPS_timer{ 0.0f };

private:
	inline static Application* m_instance{ nullptr };
};