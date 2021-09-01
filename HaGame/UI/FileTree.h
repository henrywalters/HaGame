#ifndef FILE_TREE_H
#define FILE_TREE_H

#include "../Utils/Aliases.h"
#include "../Utils/FileSystem.h"

namespace hagame {
	namespace ui {
		class FileTree {
			utils::Path path;
			utils::FileSystem fs;
			Array<String> extensions;
			std::function<void(String)> _onFileSelect = [](String str){};

		public:

			FileTree(String root = utils::FileSystem::GetWorkingDirectory(), Array<String> _extensions = {}) : fs(root), path(utils::Path(root)), extensions(_extensions) {
				path.toAbsolute();
			}

			void onFileSelect(std::function<void(String)> lambda) {
				_onFileSelect = lambda;
			}

			void render() {

				fs.basePath = path.getFullPath();

				auto files = fs.listFiles("");

				ImGui::SameLine();
				ImGui::TextWrapped(path.join(" > ").c_str());

				ImGui::TreeNodeEx("...", ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth);

				if (ImGui::IsItemClicked()) {
					path.pop();
				}

				for (auto file : files) {

					static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

					bool isDir = fs.isDir(file);

					if (isDir) {
						ImGui::TreeNodeEx(("> " + stringReplace(file, '\\', "")).c_str(), base_flags);
					}
					else {
						if (extensions.size() > 0) {
							bool extMatch = false;
							auto fileParts = stringSplit(file, '.');
							auto fileExt = fileParts[fileParts.size() - 1];
							for (auto ext : extensions) {
								if (fileExt == ext) {
									extMatch = true;
								}
							}

							if (!extMatch)
								continue;
						}

						base_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
						ImGui::TreeNodeEx(stringReplace(file, '\\', "").c_str(), base_flags);
					}
					
					if (ImGui::IsItemClicked()) {
						if (fs.isDir(file)) {
							path.push(stringReplace(file, '\\', ""));
						}
						else {
							_onFileSelect(fs.getFullPath(stringReplace(file, '\\', "")));
						}
						
					}
				}
			}
		};
	}
}

#endif
