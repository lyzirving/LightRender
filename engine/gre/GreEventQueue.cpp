#include "GreEventQueue.h"

GreEventQueue::GreEventQueue(GreEventType type) : m_type(type), m_mutex(), m_evtList()
{
}

GreEventQueue::~GreEventQueue()
{
	std::list<GreEvent>().swap(m_evtList);
}

void GreEventQueue::enqueue(GreEvent&& evt)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_evtList.push_back(std::forward<GreEvent>(evt));
}

GreEvent GreEventQueue::dequeue()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	GreEvent evt = m_evtList.front();
	m_evtList.pop_front();
	return evt;
}
