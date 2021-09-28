#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "./../Utils/Aliases.h"
#include "./Subscriber.h"

namespace hagame {
	template <class EventId, class EventType>
	class Publisher {
		Map<uint32_t, Ptr<Subscriber<EventId, EventType>>> subscribers;
		uint32_t subCount = 0;
		uint32_t activeSubCount = 0;
	public:

		Ptr<Subscriber<EventId, EventType>> subscribe(EventId eventId, std::function<void(EventType)> onEventFn) {
			Ptr<Subscriber<EventId, EventType>> subscriber = std::make_shared<Subscriber<EventId, EventType>>();
			subscriber->onEvent = onEventFn;
			subscriber->eventId = eventId;
			subscriber->id = subCount;
			subCount++;
			activeSubCount++;
			subscribers.insert(std::make_pair(subscriber->id, subscriber));
			return subscriber;
		}

		void unsubscribe(Ptr<Subscriber<EventId, EventType>> subscriber) {
			subscribers.erase(subscriber->id);
			subscriber.reset();
		}

		void emit(EventId eventId, EventType e) {
			for (auto& [id, subscriber] : subscribers) {
				if (subscriber->eventId == eventId) {
					subscriber->onEvent(e);
				}	
			}
		}
	};
}

#endif
