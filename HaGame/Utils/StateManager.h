#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <functional>
#include <optional>
#include "Aliases.h"

namespace hagame {
	namespace utils {

		/**
		* @brief StateManager provides a container type that only has one active item at any given time.
		*/
		template <class StateType>
		class StateManager {
		private:
			Map<String, Ptr<StateType>> _states;
			std::optional<Ptr<StateType>> _active;
			std::function<void(StateType*)> _onDeactivate;
			std::function<void(StateType*)> _onActivate;
		public:

			StateManager() : _states(Map<String, Ptr<StateType>>()), _active(std::nullopt), _onActivate([](StateType* s) {}), _onDeactivate([](StateType* s) {}) {}

			StateManager(std::function<void(StateType*)> onActivate, std::function<void(StateType*)> onDeactivate)
				: _active(std::nullopt), 
				_onActivate(onActivate), 
				_onDeactivate(onDeactivate),
				_states(Map<String, Ptr<StateType>>())
			{}

			StateType* add(String stateName) {
				Ptr<StateType> state = std::make_unique<StateType>();
				_states.insert(std::make_pair(stateName, state));
				return state.get();
			}

			template <class T>
			StateType* add(String stateName) {
				Ptr<StateType> state = std::make_unique<T>();
				_states.insert(std::make_pair(stateName, state));
				return state.get();
			}

			bool has(String stateName) {
				return _states.find(stateName) != _states.end();
			}

			bool hasActive() {
				return _active.has_value();
			}

			// Sets the active state but does not trigger the onActivate function
			void setActive(String stateName) {
				if (!has(stateName)) {
					throw new Exception("Requested state does not exist");
				}

				_active = _states[stateName];
			}

			void activate(String stateName) {

				if (!has(stateName)) {
					throw new Exception("Requested state does not exist");
				}

				if (hasActive()) {
					_onDeactivate(active());
				}

				_active = _states[stateName];

				_onActivate(active());
			}

			StateType* active() {
				if (!_active.has_value()) {
					throw new Exception("No active state");
				}

				return _active.value().get();
			}
		};
	}
}

#endif
