#ifndef UI_SYSTEM_H
#define UI_SYSTEM_H

#include "./../../HaGame.h"
#include "./../Components/Grid.h"
#include "./../Components/Button.h"
#include "./../Components/MultiSelect.h"

namespace hagame {
	namespace ui {
		class UISystem : public hagame::ecs::System {
		public:
			String getSystemName() {
				return "hagame::ui::UISystem";
			}

			void onSystemUpdate(double dt) {

				Vec2 mousePos = game->input.keyboardMouse.mouse.position;
				mousePos[1] = game->window->size[1] - mousePos[1];

				forEach<Button>([this, mousePos](Button* b, Ptr<hagame::ecs::Entity> entity) {
					b->update(entity, mousePos, game->input.keyboardMouse.mouse.leftPressed);
				});

				forEach<Grid>([this](Grid* grid, Ptr<hagame::ecs::Entity> entity) {
					grid->update(entity->getPos());
				});

				forEach<MultiSelect>([this](MultiSelect* ms, Ptr<hagame::ecs::Entity> entity) {
					ms->update(entity);
				});
			}

		};
	}
}

#endif
