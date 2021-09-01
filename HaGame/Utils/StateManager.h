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
			String _activeName;
		public:

			StateManager() : _states(Map<String, Ptr<StateType>>()), _active(std::nullopt), _onActivate([](StateType* s) {}), _onDeactivate([](StateType* s) {}) {}

			StateManager(std::function<void(StateType*)> onActivate, std::function<void(StateType*)> onDeactivate)
				: _active(std::nullopt), 
				_onActivate(onActivate), 
				_onDeactivate(onDeactivate),
				_states(Map<String, Ptr<StateType>>())
			{}

			Ptr<StateType> add(String stateName) {
				Ptr<StateType> state = std::make_shared<StateType>();
				_states.insert(std::make_pair(stateName, state));
				return state;
			}

			template <class T>
			Ptr<StateType> add(String stateName) {
				Ptr<StateType> state = std::make_shared<T>();
				_states.insert(std::make_pair(stateName, state));
				return state;
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
				if (_activeName != stateName)
					_active = _states[stateName];
			}

			void activate(String stateName) {

				if (!has(stateName)) {
					throw new Exception("Requested state does not exist");
				}

				if (_activeName != stateName) {

					std::cout << stateName << "\n";

					if (hasActive()) {
						_onDeactivate(active());
					}

					_active = _states[stateName];

					_onActivate(active());
				}	
			}

			StateType* active() {
				if (!_active.has_value()) {
					throw new Exception("No active state");
				}

				return _active.value().get();
			}

			void forEach(std::function<void(Ptr<StateType>)> lambda) {
				for (auto& [key, state] : _states) {
					lambda(state);
				}
			}

			void forEach(std::function<void(String, Ptr<StateType>)> lambda) {
				for (auto& [key, state] : _states) {
					lambda(key, state);
				}
			}
		};
	}
}

#endif
