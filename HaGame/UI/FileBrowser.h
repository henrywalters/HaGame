#ifndef FILE_BROWSER_H
#define FILE_BROWSER_H

#include "../Utils/Aliases.h"
#include "../Utils/FileSystem.h"
#include "../Utils/Path.h"
#include "../HaGame.h"

namespace hagame {
	namespace ui {

		// ImGui implementation of a simple file browser
		class FileBrowser {
		public:

			bool selectMany = false;

			FileBrowser(String root = utils::FileSystem::GetWorkingDirectory()) : fs(root), path(utils::Path(root)), rootFs(utils::FileSystem("")) {
				path.toAbsolute();
			}

			void render(Game* game) {

				fs.basePath = path.getFullPath();

				ImGui::Begin("File Browser");

				int index = 0;

				if (ImGui::Button("<<")) {
					path.pop();
				}

				for (auto part : path.parts) {
					ImGui::SameLine();
					if (ImGui::Button(part.c_str())) {
						for (int i = 0; i < path.parts.size() - index; i++) {
							path.pop();
						}
					}
					index++;
				}

				ImGui::BeginTable("Files", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_BordersOuter);

				ImGui::TableNextRow();

				ImGui::TableSetColumnIndex(0);

				ImGui::TableSetColumnIndex(1);

				{
					ImGui::BeginChild("DirectoryExplorer", ImVec2(ImGui::GetColumnWidth(1), ImGui::GetWindowContentRegionMax().y - 100));

					for (auto dir : fs.listDirs("")) {
						bool isSelected = checkIfSelected(dir);

						if (ImGui::Selectable(dir.c_str(), &isSelected, ImGuiSelectableFlags_AllowDoubleClick)) {
							if (ImGui::IsMouseDoubleClicked(0)) {
								path.push(dir);
							}
							else {
								select(game, dir);
							}
						}
					}

					for (auto file : fs.listFiles("")) {
						bool isSelected = checkIfSelected(file);
						if (ImGui::Selectable(file.c_str(), &isSelected)) {
							select(game, file);
						}
					}

					ImGui::EndChild();
				}

				ImGui::EndTable();

				if (ImGui::Button("Import File(s)")) {

				}

				ImGui::End();
			}

		private:

			utils::Path path;
			utils::FileSystem fs;
			utils::FileSystem rootFs;

			Set<String> selectedFiles;

			String qualify(String path) {
				return fs.basePath + path;
			}

			bool checkIfSelected(String path) {
				String fullPath = qualify(path);
				return hasKey(selectedFiles, fullPath) ? true : false;
			}

			void select(Game* game, String path) {
				if (game->input.keyboardMouse.keyboard.lCtrl)
					addToSelectedFiles(path);
				else
					selectOne(path);
			}

			void selectOne(String path) {
				std::cout << "Clearing files\n";
				selectedFiles.clear();
				addToSelectedFiles(path);
			}

			void addToSelectedFiles(String path) {
				String fullPath = qualify(path);
				if (hasKey(selectedFiles, fullPath))
					selectedFiles.erase(fullPath);
				else
					selectedFiles.insert(fullPath);
			}
		};
	}
}

#endif
