#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include <cstdint>
#include "../../Vendor/entt/entt.hpp"
#include "System.h"
#include "../../Input/InputManager.h"
#include "../../Utils/Aliases.h"
#include <type_traits>

namespace hagame {

	class Game;
	class Scene;

	namespace ecs {

		class SystemManager {
		private:
			
			// Owned data
			Array<Ptr<System>> systems;

			// Reference data
			entt::basic_registry<uint32_t>* registry;

		public:
			SystemManager(entt::basic_registry<uint32_t>* _registry) : registry(_registry) {
				systems = Array<Ptr<System>>();
			}

			template <class T>
			T* add(Game* game) {
				static_assert(std::is_base_of<System, T>::value, "Must be an instance of System");
				Ptr<T> system = std::make_shared<T>();
				system->registry = registry;
				system->game = game;
				system->window = &Game::window;
				system->input = &Game::input;
				systems.push_back(system);
				return system.get();
			}

			template <class T>
			T* add(Game* game, Scene* scene) {
				static_assert(std::is_base_of<System, T>::value, "Must be an instance of System");
				Ptr<T> system = std::make_unique<T>();
				system->registry = registry;
				system->game = game;
				system->scene = scene;
				systems.push_back(system);
				return system.get();
			}

			template <class T>
			T* get(Game* game, Scene* scene) {
				for (auto system : systems) {
					if (typeid(T) == typeid(*system))
						return (T*) system.get();
				}
				throw new std::exception("System does not exist in this scene");
			}

			void forAll(std::function<void(System*)> lambda) {
				for (int i = 0; i < systems.size(); i++) {
					lambda(systems[i].get());
				}
			}

			void startAll() {
				forAll([](ecs::System* system) { system->start(); });
			}

			void stopAll() {
				forAll([](ecs::System* system) { system->stop(); });
			}

			void beforeUpdateAll(double dt) {
				forAll([dt](ecs::System* system) { system->beforeUpdate(dt); });
			}

			void updateAll(double dt) {
				forAll([dt](ecs::System* system) { system->update(dt); });
			}

			void afterUpdateAll(double dt) {
				forAll([dt](ecs::System* system) { system->afterUpdate(dt); });
			}
		};
	}
}

#endif

