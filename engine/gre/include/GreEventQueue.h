#ifndef GRE_EVENT_QUEUE_H
#define GRE_EVENT_QUEUE_H

#include <list>
#include <mutex>

#include "GreDef.h"

class GreEventQueue
{
public:
	GreEventQueue(GreEventType type);
	~GreEventQueue();

	inline bool empty() { return m_evtList.empty(); }
	inline GreEventType type() { return m_type; }

	void enqueue(GreEvent&& evt);
	GreEvent dequeue();

private:
	GreEventType m_type;
	std::mutex m_mutex;
	std::list<GreEvent> m_evtList;
};

#endif // !GRE_EVENT_QUEUE_H