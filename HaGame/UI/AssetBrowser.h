#ifndef ASSET_BROWSER_H
#define ASSET_BROWSER_H

#include "../Core/Game.h"
#include "../Utils/Aliases.h"
#include "../Utils/Path.h"
#include "../Utils/FileSystem.h"

namespace hagame {
	namespace ui {
		class AssetBrowser {
		public:

			AssetBrowser(String root = utils::FileSystem::GetWorkingDirectory()) : fs(root) {}
			~AssetBrowser() {}

			void render(hagame::Game* game) {
				for (auto file : fs.listFiles("")) {
					std::cout << file << "\n";
					ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

					if (file == selectedPath) {
						base_flags |= ImGuiTreeNodeFlags_Selected;
					}

					if (fs.isDir(file)) {
						ImGui::TreeNodeEx(file.c_str(), base_flags);
					}
				}
			}

		private:

			utils::FileSystem fs;
			String selectedPath;

			void renderFile(String path) {

			}
		};
	}
}

#endif
