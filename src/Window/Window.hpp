#pragma once
#include <string>
#include <GLFW/glfw3.h>
#include <Event/Event.hpp>

class Window final
{
	/*
		Interval 0: unlimited FPS
		Interval 1: 60 FPS
		Interval 2: 30 FPS
	*/
	enum FrameRate : std::int32_t
	{
		Unlimited = 0,
		FPS_60 = 1,
		FPS_30 = 2,
	};

	using EventCallback = std::function<void(Event&)>;

public:
	Window(const std::string & title, int width, int height);
	~Window();

public:
  glm::ivec2 GetSize() const;
  glm::ivec2 GetPosition() const;
	GLFWwindow* GetGLFWwindow() const noexcept;
	bool ShouldClose() const noexcept;
	bool IsFullscreen() const noexcept;
	const std::string& GetTitle() const noexcept;
  float GetAspectRatio() const;

public:
	void SetShouldClose(const bool should_close) noexcept;
	void SetEventCallback(const EventCallback& callback) noexcept;
	void SetTitle(const std::string& title, std::uint32_t FPS = 0) noexcept;

public:
	void PollEvents() const noexcept;
	void SwapBuffers() const noexcept;

public:
    // Will hide cursor but when you move out of the window it will show back
    void HideCursor() noexcept;
    void ShowCursor() noexcept;
    // Will completely disable cursor to have no chance of going out of window (used for camera FPS view for example)
    void DisableCursor() noexcept;
private:
	bool InitGLFW(const std::string& title, const int width, const int height);
	bool InitGLFWCallbacks();
	bool InitGLAD();
	bool InitOpenGLOptions();


private:
	std::string m_title;

private:
	GLFWwindow* m_window;

private: /* Events */
	EventCallback m_event_callback;
};

