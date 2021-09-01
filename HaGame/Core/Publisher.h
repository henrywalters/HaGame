#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "./../Utils/Aliases.h"
#include "./Subscriber.h"

namespace hagame {
	template <class EventType>
	class Publisher {
		Map<uint32_t, Ptr<Subscriber<EventType>>> subscribers;
		uint32_t subCount = 0;
		uint32_t activeSubCount = 0;
	public:

		Ptr<Subscriber<EventType>> subscribe(std::function<void(EventType)> onEventFn) {
			Ptr<Subscriber<EventType>> subscriber = std::make_shared<Subscriber<EventType>>();
			subscriber->onEvent = onEventFn;
			subscriber->id = subCount;
			subCount++;
			activeSubCount++;
			subscribers.insert(std::make_pair(subscriber->id, subscriber));
			return subscriber;
		}

		void unsubscribe(Ptr<Subscriber<EventType>> subscriber) {
			subscribers.erase(subscriber->id);
			subscriber.reset();
		}

		void emit(EventType e) {
			for (auto& [id, subscriber] : subscribers) {
				subscriber->onEvent(e);
			}
		}
	};
}

#endif
