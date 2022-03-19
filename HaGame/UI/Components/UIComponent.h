#ifndef UI_COMPONENT_H
#define UI_COMPONENT_H

#include "./../../Utils/Aliases.h"
#include "./../../Core/ECS/Entity.h"

namespace hagame {
	namespace ui {
		class UIComponent {
		public:

			Vec3 offset = Vec2::Zero();
			virtual Rect getBoundingBox() = 0;
		};

		struct UIEntity {
			hagame::ecs::Entity* entity;
			std::function<UIComponent* (hagame::ecs::Entity*)> uiFn;

			UIEntity(hagame::ecs::Entity* _entity, std::function<UIComponent* (hagame::ecs::Entity*)> _uiFn) :
				entity(_entity),
				uiFn(_uiFn)
			{}
		};
	}
}

#endif
