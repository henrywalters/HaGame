#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <functional>
#include <cstdint>

namespace hagame {
	template <class EventId, class EventType>
	struct Subscriber {
		uint32_t id;
		EventId eventId;
		std::function<void(EventType)> onEvent;
	};
}


#endif
