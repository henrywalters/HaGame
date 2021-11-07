#ifndef WATCHABLE_HPP
#define WATCHABLE_HPP

#include <functional>

namespace hagame {
	namespace utils {
		template <class T>
		class Watchable {
			T m_value;
			std::function<void(T)> m_onChange;

		public:

			Watchable(T value, std::function<void(T)> onChange) :
				m_value(value),
				m_onDirty(onChange) {};

			void operator = (const T& value) {
				m_value = value;
				m_onChange(m_value);
			}
		};
	}
}

#endif
