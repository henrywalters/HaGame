#ifndef WATCHABLE_HPP
#define WATCHABLE_HPP

#include <functional>

namespace hagame {
	namespace utils {

		/*
			Watchable provides a simple wrapper for a value which sends an event when it is changed

			This is done in two ways:

			1) The wrapped value is set directly
			2) A reference to the value is modified, and clean() is called.

			Ex:

			auto watching = Watchable<float>(0.0f, [](float old, float value) {
				std::cout << "Changed from " << old << " to " << value << "\n";
			});

			watching = 0.0f; // No event is triggered
			watching = 1.0f; // An event is triggered

			auto value = watching.ref();
			value = 2.0f; // No event is triggered, but the watcher is now "dirty"

			watching.isDirty(); // true

			watching.clean(); // Event is triggered

			watching.isDirty(); // false

		*/
		template <class T>
		class Watchable {
			T m_oldValue;
			T m_value;
			std::function<void(T, T)> m_onChange;

		public:

			Watchable(T value, std::function<void(T, T)> onChange) :
				m_value(value),
				m_oldValue(value),
				m_onChange(onChange) {};

			// Directly setting the watchable object will cause onChange to fire.
			void operator=(const T& value) {
				if (value != m_oldValue) {
					m_onChange(m_oldValue, value);
					m_oldValue = value;
					m_value = value;
				}
			}

			// Get a reference to the watched value. A change will not be detected until isDirty is called.
			T* ref() {
				return &m_value;
			}

			// If the value has been dirtied, the onChange event is fired.
			void clean() {
				if (m_value != m_oldValue) {
					m_onChange(m_oldValue, m_value);
					m_oldValue = m_value;
				}
			}

			// Checks if the current value has been dirtied (modified)
			bool isDirty() {
				return m_oldValue != m_value;
			}
		};
	}
}

#endif
