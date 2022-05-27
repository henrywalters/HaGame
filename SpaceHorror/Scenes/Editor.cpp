#include "Editor.h"

void Editor::onSceneInit() {

	loadTiles();

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

	// Dragging is kind of annoying so just disable by default
	/*
	if (game->input.player(0).rTrigger && !isDragging && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)) {
		isDragging = true;
		dragStartPos = mousePos;
		startOffset = cameraEntity->getPos();
	}
	*/

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
	renderNewLevel();
	renderSaveLevelAs();
	renderLoadLevel();

	ImGui::End();

	resourceBrowser.render(game);

	ImGui::ShowDemoWindow();
}

void Editor::newLevel()
{
	editorEntity->removeComponent<Grid>();
	for (auto tile : tiles) {
		tile->placements.forEach([this](RawPtr<Entity> entity) { removeEntity(entity); });
		tile->placements.clear();
	}
	auto editorGrid = editorEntity->addComponent<Grid>(gridPartitions.cast<float>() * cellSize, gridPartitions[0], gridPartitions[1]);
	editorGrid->display = true;
	editorGrid->color = PRUSSIAN_BLUE;
}

Optional<Editor::Error> Editor::saveLevel()
{
	Optional<Editor::Error> error = std::nullopt;

	auto config = hagame::utils::MultiConfigFile();
	config.addPage("settings");
	config.addPage("tiles");
	config.addPage("enemies");
	config.addPage("fx");
	config.addPage("connections");
	auto tileConfig = config.getPage("tiles");
	auto settings = config.getPage("settings");

	settings->addSection("level_size");
	settings->setArray<int, 2>("level_size", "grid_partitions", gridPartitions.vector);
	settings->setValue("level_size", "cell_size", cellSize);

	for (auto tile : tiles) {
		tileConfig->addSection(tile->name);
		std::vector<std::array<int, 2>> placements;
		tile->placements.forEach([&placements](auto x, auto y, auto entity) {
			std::array<int, 2> coords;
			coords[0] = x;
			coords[1] = y;
			placements.push_back(coords);
		});
		tileConfig->setListOfArrays(tile->name, "placements", placements);
	}

	config.writeToFile(&activeFile.value());

	return error;
}

Optional<Editor::Error> Editor::loadLevel()
{
	Optional<Editor::Error> error = std::nullopt;
	hagame::utils::MultiConfigFile config;
	try {
	
		config = hagame::utils::MultiConfigFile(&activeFile.value());

		auto settings = config.getPage("settings");
		settings->getArray("level_size", "grid_partitions", gridPartitions.vector);
		cellSize = settings->getValue<float>("level_size", "cell_size");

		// Initialize the level after setting basic configurations
		newLevel();
		loadTiles();

		auto tiles = config.getPage("tiles");
		for (auto section : tiles->getSections()) {
			auto placements = tiles->getListOfArrays<float, 2>(section, "placements");
			for (auto placement : placements) {
				addTile(tileMap[section], Vec2Int(placement[0], placement[1]));
			}
		}
	}
	catch (...) {
		return Editor::Error{ 0, "Failed to parse config file" };
	}

	return error;
}

void Editor::renderMenuBar() {

	Optional<const char*> menuChoice = std::nullopt;

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New")) {
				menuChoice = NEW_LEVEL;
			}

			if (ImGui::MenuItem("Save", "Ctrl+S")) {
				if (activeFile.has_value()) {
					saveLevel();
				}
				else {
					ImGuiFileDialog::Instance()->OpenDialog("Save As", "Choose File", ".level", ".");
				}
			}

			if (ImGui::MenuItem("Save As")) {
				ImGuiFileDialog::Instance()->OpenDialog("Save As", "Choose File", ".level", ".");
			}

			if (ImGui::MenuItem("Load")) {
				ImGuiFileDialog::Instance()->OpenDialog("Load", "Choose File", ".level", ".");
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

void Editor::renderNewLevel()
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal(NEW_LEVEL, NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{

		ImGui::Text(NEW_LEVEL);
		ImGui::SliderInt2("Level Size (in cells)", gridPartitions.vector, 0, 200);
		ImGui::SliderFloat("Cell Size (in meters)", &cellSize, 0, 2000);
		ImGui::Text((("Level Size (in meters): " + (gridPartitions.cast<float>() * cellSize).toString()).c_str()));

		if (ImGui::Button("Create", ImVec2(120, 0))) {
			newLevel();
		}

		ImGui::SameLine();

		if (ImGui::Button("Close", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}
}

void Editor::renderSaveLevelAs()
{
	if (ImGuiFileDialog::Instance()->Display("Save As"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			auto filePathParts = stringSplit(filePathName, '\\');
			activeFile = hagame::utils::File(filePath, filePathParts[filePathParts.size() - 1]);
			saveLevel();
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}
}

void Editor::renderLoadLevel()
{
	if (ImGuiFileDialog::Instance()->Display("Load"))
	{
		// action if OK
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			auto filePathParts = stringSplit(filePathName, '\\');
			activeFile = hagame::utils::File(filePath, filePathParts[filePathParts.size() - 1]);
			loadLevel();
		}

		// close
		ImGuiFileDialog::Instance()->Close();
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

	for (int i = 0; i < tiles.size(); i++) {
		ImGui::RadioButton(tiles[i]->name.c_str(), &selectedTileIdx, i);
		if (selectedTileIdx == i) {

			auto size = tiles[i]->texture->image->size.cast<float>().fill(resourceBrowser.imageSize);

			ImGui::Image((ImTextureID)tiles[i]->texture->id, ImVec2(size[0], size[1]));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE"))
				{
					String name = *(const String*)payload->Data;
					std::cout << name << "\n";
					tiles[i]->texture = game->resources->getTexture(name);
				}
			}

			auto zIndex = hagame::utils::Watchable<float>(tiles[i]->zIndex, [this, i](auto old, auto value){
				tiles[i]->zIndex = value;
				tiles[i]->placements.forEach([this, value](Entity* entity) {
					auto renderer = entity->getComponent<Sprite2DRenderer>();
					renderer->sprite->pos[2] = value;
				});
			});

			auto opacity = hagame::utils::Watchable<float>(tiles[i]->opacity, [this, i](auto old, auto value) {
				tiles[i]->opacity = value;
				tiles[i]->placements.forEach([this, value](Entity* entity) {
					auto renderer = entity->getComponent<Sprite2DRenderer>();
					renderer->sprite->color[3] = value;
				});
			});

			ImGui::SliderFloat("Z Index", zIndex.ref(), -100.0, 100.0);
			ImGui::SliderFloat("Opacity", opacity.ref(), 0.0, 1.0);

			zIndex.clean();
			opacity.clean();
		}
	}

	auto qr = editorEntity->getComponent<Grid>();

	if (qr->getGridRect().contains(worldPos) && !guiActive()) {
		auto idx = qr->getCellIdx(worldPos);

		if (game->input.player(0).rTrigger) {

			if (!tiles[selectedTileIdx]->placements.has(idx[0], idx[1])) {
				addTile(selectedTileIdx, idx);
			}
		}

		if (game->input.player(0).lTrigger && tiles[selectedTileIdx]->placements.has(idx[0], idx[1])) {
			
			auto tile = tiles[selectedTileIdx]->placements.get(idx[0], idx[1]);
			removeEntity(tile);
			tiles[selectedTileIdx]->placements.remove(idx[0], idx[1]);
		}
	}

	if (qr->getGridRect().contains(worldPos) && game->input.player(0).rTriggerPressed) {


	}
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

void Editor::addTile(int tileIdx, Vec2Int idx)
{
	auto qr = editorEntity->getComponent<Grid>();
	auto tile = addEntity();
	auto cell = qr->getCell(idx);
	auto pos = cell.pos + GRID_SIZE * -0.5 + cell.size * 0.5;
	pos[2] = tiles[tileIdx]->zIndex;
	tile->setPos(pos);
	auto renderer = tile->addComponent<Sprite2DRenderer>();
	renderer->shader = game->resources->getShaderProgram("sprite2d");
	renderer->sprite = std::make_shared<Sprite2D>();
	renderer->sprite->texture = tiles[selectedTileIdx]->texture;
	renderer->sprite->quad = std::make_shared<Quad>(cell.size);
	renderer->sprite->color[3] = tiles[selectedTileIdx]->opacity;
	tiles[tileIdx]->placements.insert(idx[0], idx[1], tile);
}

void Editor::loadTiles()
{
	tiles.clear();
	tileMap.clear();

	auto tileConfig = config.load("Tiles");

	auto sections = tileConfig->getSections();
		
	for (int i = 0; i < sections.size(); i++) {
		auto tile = std::make_shared<Tile>();
		auto section = sections[i];
		tile->name = section;
		tile->texture = game->resources->getTexture(tileConfig->getRaw(section, "texture"));
		tile->opacity = tileConfig->getValue<float>(section, "opacity");
		tile->zIndex = tileConfig->getValue<float>(section, "zIndex");
		tiles.push_back(tile);
		tileMap.insert(std::make_pair(tile->name, i));
	}
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

bool Editor::guiActive()
{
	return ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow | ImGuiHoveredFlags_AllowWhenBlockedByActiveItem);
}
