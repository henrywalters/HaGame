#ifndef ENTITY_2_UI_H
#define ENTITY_2_UI_H

#include <functional>
#include "./../Components/UIComponent.h"
#include "./../../Core/ECS/Entity.h"

typedef std::function<hagame::ui::UIComponent*(Ptr<hagame::ecs::Entity>)> entity2ui_t;

#endif