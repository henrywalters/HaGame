#include "Editor.h"

void Editor::onSceneInit() {
	addSystem<RenderSystem>();
	addSystem<UISystem>();

	cameraEntity = addEntity();
	auto camera = cameraEntity->addComponent<CameraComponent>();
	orth = std::make_shared<OrthographicCamera>(game->window->size);
	orth->centered = true;
	camera->camera = orth;

	editorEntity = addEntity();
	editorEntity->move(GRID_SIZE * -0.5);
	auto editorGrid = editorEntity->addComponent<Grid>(GRID_SIZE, 50, 50);
	editorGrid->display = true;
	editorGrid->color = PRUSSIAN_BLUE;

	game->input.keyboardMouse.mouseEvents.subscribe(hagame::input::devices::MouseEvents::Moved, [this](hagame::input::devices::MouseEvent e) {
		mousePos = e.mousePos;
		mousePos[1] = game->window->size[1] - mousePos[1];
		worldPos = orth->getGamePos(cameraEntity->transform, mousePos);
	});

	game->input.windowEvents.subscribe(hagame::input::WindowEvents::Resized, [this](hagame::input::WindowEvent e) {
		orth->size = e.data;
		game->window->setViewport(Rect(Vec2::Zero(), e.data));
	});
}

void Editor::onSceneUpdate(double dt) {

	if (game->input.keyboardMouse.startPressed) {
		game->setScene("home");
	}
	else {
		updateCamera(dt);
		updateGrid();
		renderGUI();
		render();
	}


}

void Editor::updateCamera(double dt) {

	cameraEntity->move(game->input.player(0).lAxis * cameraSpeed * dt);

	if (game->input.keyboardMouse.keyboard.numbers[1])
		orth->zoomIn(zoomSpeed * dt);

	if (game->input.keyboardMouse.keyboard.numbers[2])
		orth->zoomOut(zoomSpeed * dt);

	if (game->input.player(0).rTrigger && !isDragging && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)) {
		isDragging = true;
		dragStartPos = mousePos;
		startOffset = cameraEntity->getPos();
	}


	if (!game->input.player(0).rTrigger) {
		isDragging = false;
	}

	if (isDragging) {
		auto dragDelta = mousePos - dragStartPos;
		cameraEntity->setPos(startOffset - dragDelta);
	}

}

void Editor::updateGrid() {

	auto qr = editorEntity->getComponent<Grid>();

	if (qr->getGridRect().contains(worldPos)) {

		auto idx = qr->getCellIdx(worldPos);
		auto cell = qr->getCell(idx);
		cell.size *= 1.2f;
		cell.pos += editorEntity->getPos() - cell.size * 0.1f;
		drawRect(cell, PRIMARY, DEBUG_SHADER, 1.0f);
	}
}

void Editor::renderGUI() {
	bool open = true;
	ImGui::Begin("Level Editor Menu", &open, ImGuiWindowFlags_MenuBar);

	renderMenuBar();
	renderToolBox();
	renderSelectedTool();
	renderControlSettings();

	ImGui::End();

	ImGui::ShowDemoWindow();
}

void Editor::renderMenuBar() {

	Optional<const char*> menuChoice = std::nullopt;

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save", "Ctrl+S")) {
				std::cout << "SAVE\n";
				ImGuiFileDialog::Instance()->OpenDialog("Save As", "Choose File", ".level", ".");
			}

			if (ImGui::MenuItem("Save As")) {
				ImGuiFileDialog::Instance()->OpenDialog("Save As", "Choose File", ".level", ".");
			}

			if (ImGui::MenuItem("Load")) {
				ImGuiFileDialog::Instance()->OpenDialog("Load Particle File", "Choose File", ".level", ".");
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Settings"))
		{
			if (ImGui::MenuItem("Controls")) {
				menuChoice = CONTROL_SETTINGS;
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	if (menuChoice.has_value()) {
		ImGui::OpenPopup(menuChoice.value());
	}
}

void Editor::renderControlSettings() {
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal(CONTROL_SETTINGS, NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text(CONTROL_SETTINGS);
		ImGui::SliderFloat("zoomSpeed", &zoomSpeed, 0.0, 100.0);
		ImGui::SliderFloat("cameraSpeed", &cameraSpeed, 0.0, 100.0);
		if (ImGui::Button("Close", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}
}


void Editor::renderSelectedTool()
{
	switch (selectedTool) {
	case Tools::TileEditor:
		tileEditor();
		break;
	case Tools::EnemyEditor:
		enemyEditor();
		break;
	case Tools::FxEditor:
		fxEditor();
		break;
	case Tools::Connector:
		connector();
		break;
	}
}

void Editor::tileEditor()
{
	ImGui::Text("SELECT TILE TYPE");
}

void Editor::enemyEditor()
{
}

void Editor::fxEditor()
{
}

void Editor::connector()
{
}

void Editor::renderToolBox() {
	int selectedToolIdx = (int)selectedTool;

	for (int i = 0; i < TOOL_NAMES.size(); i++) {
		ImGui::RadioButton(TOOL_NAMES[i].c_str(), &selectedToolIdx, i);
	}

	selectedTool = (Tools)selectedToolIdx;
}


void Editor::render() {
	// drawCrosshairs(mousePos, 5, 15, SECONDARY, 2.0f);

	auto shader = game->resources->getShaderProgram("batch_line");

	shader->use();

	shader->setMVP(Mat4::Identity(), Mat4::Identity(), orth->getProjMatrix(cameraEntity->getPos()));
	lineBuffer.draw();

	shader = game->resources->getShaderProgram("color");
	shader->use();
	shader->setMVP(Mat4::Identity(), Mat4::Identity(), orth->getProjMatrix(cameraEntity->getPos()));

	DEBUG_SHADER = game->resources->getShaderProgram("color");
}