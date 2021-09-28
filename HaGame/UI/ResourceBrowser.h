#ifndef RESOURCE_BROWSER_H
#define RESOURCE_BROWSER_H

#include <optional>
#include <any>
#include <functional>
#include "./../Core/Game.h"
#include "./../Core/ECS/Entity.h"
#include "./../Core/Publisher.h"

namespace hagame {
	namespace ui {

		class ResourceBrowser {
		public:

			enum EventType {};

			struct Event {};

			float imageSize = 75.0f;

			void render(Game* game) {
				ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
				if (ImGui::BeginTabBar("Resource types", tab_bar_flags))
				{

					auto windowSize = ImGui::GetWindowSize();
					int maxPerLine = (int) floor(windowSize.x / imageSize) - 2;

					for (auto tab : tabs) {
						if (ImGui::BeginTabItem(tab.label)) {
							auto id = 0;
							for (auto& [name, resource] : tab.getResources(game)) {
								if ((id % maxPerLine) != 0) {
									ImGui::SameLine();
								}

								auto texId = (ImTextureID)tab.getTextureId(game, resource);
								auto scaled = tab.getTextureSize(resource, imageSize);
								ImVec2 imgScaled = ImVec2(scaled[0], scaled[1]);
								
								ImGui::BeginGroup();
								ImGui::ImageButton(texId, imgScaled);
								ImGui::Text(name.c_str());
								ImGui::EndGroup();

								if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
									ImGui::SetDragDropPayload(tab.dragId, &name, sizeof(String));
									ImGui::Image(texId, imgScaled);
									ImGui::EndDragDropSource();
								}

								id++;
							}

							ImGui::EndTabItem();
						}
					}
				}
			}

		private:

			Publisher<EventType, Event> events;

			struct Tab {
				const char* label;
				const char* dragId;
				std::function<Vec2(std::any, Vec2)> getTextureSize;
				std::function<GLuint(Game*, std::any)> getTextureId;
				std::function<Map<String, std::any>(Game*)> getResources;
			};

			Tab shaderTab{
				"Shader Programs",
				"SHADER",
				[](std::any resource, Vec2 imageSize) { return imageSize; },
				[](Game* game, std::any resource) { return game->resources->getTexture("shader_texture")->id; },
				[](Game* game) { return make_any(game->resources->shaderPrograms); }
			};

			Tab fontTab{
				"Fonts",
				"FONT",
				[](std::any resource, Vec2 imageSize) { return imageSize; },
				[](Game* game, std::any resource) { return game->resources->getTexture("font_texture")->id; },
				[](Game* game) { return make_any(game->resources->fonts); }
			};

			Tab meshTab{
				"Meshes",
				"MESH",
				[](std::any resource, Vec2 imageSize) { return imageSize; },
				[](Game* game, std::any resource) { return game->resources->getTexture("mesh_texture")->id; },
				[](Game* game) { return make_any(game->resources->meshes); }
			};

			Tab textureTab{
				"Textures",
				"TEXTURE",
				[](std::any resource, Vec2 imageSize) { return std::any_cast<Ptr<graphics::Texture>>(resource)->image.size.cast<float>().fill(imageSize); },
				[](Game* game, std::any resource) { return std::any_cast<Ptr<graphics::Texture>>(resource)->id; },
				[](Game* game) { return make_any(game->resources->textures); }
			};

			Tab audioTab{
				"Audio Samples",
				"AUDIO",
				[](std::any resource, Vec2 imageSize) { return imageSize; },
				[](Game* game, std::any resource) { return game->resources->getTexture("audio_texture")->id; },
				[](Game* game) { return make_any(game->resources->audioSamples); }
			};

			Array<Tab> tabs = {
				textureTab,
				meshTab,
				shaderTab,
				audioTab,
				fontTab
			};
		};
	}
}

#endif
