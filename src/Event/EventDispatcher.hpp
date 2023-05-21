#pragma once
#include <functional>
#include "Event.hpp"

class EventDispatcher
{
	template<typename T>
	using EventFun = std::function<bool(T&)>;

public:
	explicit EventDispatcher(Event& event)
		:
		m_event(event)
	{
	}

	template<typename T>
	//bool Dispatch(EventFun<T> func)
	bool Dispatch(const EventFun<T>& func)
	{
		if (m_event.GetEventType() == T::GetStaticType())
		{
			m_event.SetHandled(func(*(T*)&m_event));
			return true;
		}
		return false;
	}
private:
	Event& m_event;
};
