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
			Ptr<T> add(Game* game) {
				static_assert(std::is_base_of<System, T>::value, "Must be an instance of System");
				Ptr<T> system = std::make_shared<T>();
				system->registry = registry;
				system->game = game;
				system->window = &Game::window;
				system->input = &Game::input;
				systems.push_back(system);
				return system;
			}

			template <class T>
			Ptr<T> add(Game* game, Scene* scene) {
				static_assert(std::is_base_of<System, T>::value, "Must be an instance of System");
				Ptr<T> system = std::make_unique<T>();
				system->registry = registry;
				system->game = game;
				system->scene = scene;
				systems.push_back(system);
				return system;
			}

			template <class T>
			Ptr<T> get(Game* game, Scene* scene) {
				for (auto system : systems) {
					if (typeid(T) == typeid(*system))
						return std::dynamic_pointer_cast<T>(system);
				}
				throw new std::exception("System does not exist in this scene");
			}

			void forAll(std::function<void(Ptr<System>)> lambda) {
				for (int i = 0; i < systems.size(); i++) {
					lambda(systems[i]);
				}
			}

			void initAll() {
				forAll([](Ptr<ecs::System> system) { system->init();  });
			}

			void startAll() {
				forAll([](Ptr<ecs::System> system) { std::cout << system->getSystemName();  system->start(); });
			}

			void stopAll() {
				forAll([](Ptr<ecs::System> system) { system->stop(); });
			}

			void beforeUpdateAll(double dt) {
				forAll([dt](Ptr<ecs::System> system) { system->beforeUpdate(dt); });
			}

			void updateAll(double dt) {
				forAll([dt](Ptr<ecs::System> system) { system->update(dt); });
			}

			void afterUpdateAll(double dt) {
				forAll([dt](Ptr<ecs::System> system) { system->afterUpdate(dt); });
			}
		};
	}
}

#endif

