#ifndef ENTITY_TREE_H
#define ENTITY_TREE_H

#include <optional>
#include "./../Core/Game.h"
#include "./../Core/ECS/Entity.h"
#include "./../Core/Publisher.h"

namespace hagame { 

	namespace ui {

	class EntityTree {

	public:

		enum EventType {
			AddEntity,
			AddChild,
			AddChildTo,
			RemoveEntity,
			SelectEntity
		};

		struct Event {
			// If removing an entity, this will be filled
			Ptr<ecs::Entity> entity = NULL;

			// If adding a child, this will be the target parent
			Ptr<ecs::Entity> target = NULL;
		};

		// Listen to events using this member
		Publisher<EventType, Event> events;

		// Render a tree for a single entity. Likely, there are many root entities that this should be called for
		void render(Game* game, Ptr<ecs::Entity> entity) {
			if (entity == NULL)
				return;

			Event e;
			e.entity = entity;

			ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

			if (entity->children.size() == 0) {
				base_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
			}

			if (selectedEntity.has_value() && selectedEntity.value()->id == entity->id) {
				base_flags |= ImGuiTreeNodeFlags_Selected;
			}

			bool displayChildren = false;

			if (ImGui::TreeNodeEx((void*)(intptr_t)entity->id, base_flags, entity->name.c_str()) && entity->children.size() > 0) {
				displayChildren = true;
			}

			if (ImGui::IsItemClicked()) {
				selectedEntity = entity;
				events.emit(EventType::SelectEntity, e);
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
				ImGui::OpenPopup(std::to_string(entity->id).c_str());
			}

			if (ImGui::BeginPopup(std::to_string(entity->id).c_str())) {
				ImGui::Text(("Entity: " + entity->name + " ctx menu").c_str());
				ImGui::Separator();
				if (ImGui::Button("Add child")) {
					events.emit(EventType::AddChild, e);
				}
				if (ImGui::Button("Remove entity")) {
					events.emit(EventType::RemoveEntity, e);
				}
				ImGui::EndPopup();
			}

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
				ImGui::SetDragDropPayload("ENTITY", (void*)&entity->id, sizeof(uint32_t));
				ImGui::Text(entity->name.c_str());
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget()) {
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY")) {
					uint32_t entityId = *(const uint32_t*)payload->Data;
					auto dragging = game->scenes.active()->ecs.entities.get(entityId);
					// addChild(entity, dragging);
					e.entity = dragging;
					e.target = entity;
					events.emit(EventType::AddChildTo, e);
				}
				ImGui::EndDragDropTarget();
			}

			if (displayChildren) {
				for (auto child : entity->children) {
					render(game, child);
				}
				ImGui::TreePop();
			}
		}

	private:

		Optional<Ptr<ecs::Entity>> selectedEntity = std::nullopt;
		
	};

} }

#endif
