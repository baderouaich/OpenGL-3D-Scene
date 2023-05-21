#pragma once
#include <Event/Event.hpp>
#include <Event/EventDispatcher.hpp>
#include <Event/ApplicationEvent.hpp>
#include <Event/MouseEvent.hpp>
#include <Event/KeyEvent.hpp>
#include <Input/Input.hpp>

/*
*	Scene interface
*/
class Scene
{
public:
	Scene() : m_quit(false) {};
	virtual ~Scene() = default;

	/* Scene Life Cycle */
	virtual void OnCreate() = 0; // < Called once on scene created
	virtual void OnEvent(Event& event) = 0; // < Called each time an event occurred
	virtual void OnUpdate(float dt) = 0; // < Called each frame for updating
	virtual void OnDraw() = 0; // < Called each frame for drawing
	virtual void OnImGuiDraw() = 0; // < Called each frame after OnDraw for UI drawing
	virtual void OnDestroy() = 0; // < Called when scene is about to be destroyed

public: /* Accessors */
	/** Returns true if scene wants to be destroyed */
	[[nodiscard]] bool WantsToExit() const noexcept { return m_quit; }

public: /* Modifiers */
	/** Call this to end scene */
	void Exit() noexcept { m_quit = true; }

protected:
	bool m_quit{};
};