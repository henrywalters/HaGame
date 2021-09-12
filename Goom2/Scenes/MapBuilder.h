#ifndef G_MAPBUILDER_H
#define G_MAPBUILDER_H

#include "../../HaGame/HaGame.h"
#include "../../HaGame/UI/UI.h"

enum SelectionMode {
	GridVertex,
	GridFace
};

enum BuilderMode {
	GameObjectMode,
	PolygonMode,
	SpriteMode,
};

const Array<String> BuilderModeNames = {
	"Add empty game object",
	"Add polygon renderer",
	"Add sprite renderer"
};

struct GameObject {
	Ptr<hagame::ecs::Entity> entity;
	bool expanded = false;
	bool selected = false;
};

class MapBuilder : public hagame::Scene {

	struct PlaceHolderComponent {
		String name;
	};

	const hagame::graphics::Color lineColor = hagame::graphics::Color("#424242");

	// Cell size in meters
	Vec2 cellSize = Vec2(1.0f);
	Vec2Int partitions = Vec2Int(30);

	float panSpeed = 10.0f;
	float zoomSpeed = 5.0f;
	float pixelsPerMeter = 50.0f;

	Ptr<hagame::ecs::Entity> camera;
	Ptr<hagame::graphics::OrthographicCamera> orth;

	Ptr<hagame::utils::RectGrid> layout;
	Ptr<hagame::utils::RectGrid> centerLayout;
	Ptr<hagame::utils::RectGrid> rightLayout;
	Rect viewport;

	Vec2 mousePos;
	Vec2 screenPos;
	Vec2Int mouseIndex;
	Vec2 closestVertex;

	Vec2 dragStart;
	Vec2 camDragStart;
	bool isDragging = false;

	Optional<Ptr<hagame::ecs::Entity>> selectedEntity = std::nullopt;

	BuilderMode mode = BuilderMode::GameObjectMode;
	SelectionMode selectionMode = SelectionMode::GridFace;

	Array<Ptr<GameObject>> entities;

	void onSceneInit() {

		layout = hagame::utils::RectGrid::Columns(Rect(Vec2::Zero(), game->window->size), 12);
		centerLayout = hagame::utils::RectGrid::Rows(layout->getColumns(2, 9), 12);
		rightLayout = hagame::utils::RectGrid::Rows(layout->getColumns(10, 11), 4);

		viewport = centerLayout->getRows(0, 8);

		game->window->setViewport(viewport);
		game->window->clearColor = hagame::graphics::Color("#91caf2");

		camera = addEntity();
		camera->transform->move((partitions.cast<float>() * 0.5).prod(cellSize).resize<3>());
		auto camC = camera->addComponent<hagame::graphics::CameraComponent>();
		orth = std::make_shared<hagame::graphics::OrthographicCamera>(viewport.size / pixelsPerMeter);
		orth->zoom = 2.5f;
		camC->camera = orth;

		std::cout << "Starting UI System\n";
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer backends
		ImGui_ImplSDL2_InitForOpenGL(game->window->window, game->window->gl);
		ImGui_ImplOpenGL3_Init("#version 330 core");
	}

	void handleGameObjectMode() {
		selectionMode = SelectionMode::GridFace;

		if (game->input.keyboardMouse.mouse.leftPressed) {
			auto go = addEntity();
			go->transform->setPosition((mouseIndex.cast<float>().prod(cellSize).resize<3>()));
			go->addComponent<PlaceHolderComponent>();
			auto object = std::make_shared<GameObject>();
			object->entity = go;
			entities.push_back(object);
		}
	}

	void handlePolygonMode() {
		selectionMode = SelectionMode::GridVertex;
	}

	void handleSpriteMode() {
		selectionMode = SelectionMode::GridFace;

		if (game->input.keyboardMouse.mouse.leftPressed) {
			auto go = addEntity();
			go->transform->setPosition((mouseIndex.cast<float>().prod(cellSize).resize<3>()));
			auto sRenderer = go->addComponent<hagame::graphics::SpriteRenderer>();
			//sRenderer->textureName = "kitty";
			//sRenderer->shaderName = "sprite";
			sRenderer->sprite = std::make_shared<hagame::graphics::Sprite>(game->resources->getTexture("kitty"), Rect(cellSize * 0.5f, cellSize));
			sRenderer->shader = game->resources->getShaderProgram("sprite");
			auto object = std::make_shared<GameObject>();
			object->entity = go;
			entities.push_back(object);
		}
	}

	void drawGameObject(Ptr<GameObject> object) {
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

		if (selectedEntity.has_value() && selectedEntity.value()->id == object->entity->id)
			node_flags |= ImGuiTreeNodeFlags_Selected;

		object->expanded = ImGui::TreeNodeEx(object->entity->name.c_str(), node_flags);

		if (ImGui::IsItemClicked())
			selectedEntity = object->entity;
	}

	void drawGameObjectTree() {

		for (auto entity : entities) {
			drawGameObject(entity);
		}
	}

	void onSceneUpdate(double dt) {

		game->input.keyboardMouse.updateImgui(dt);

		screenPos = game->input.keyboardMouse.mouse.position - viewport.pos;
		screenPos[1] = viewport.size[1] - screenPos[1];
		mousePos = screenPos.copy();
		mousePos = orth->getGamePos(camera->transform, mousePos / pixelsPerMeter);

		mouseIndex = mousePos.cast<int>();
		closestVertex = mousePos.rounded();

		if (game->input.keyboardMouse.keyboard.numbers[1]) {
			orth->zoom = clamp<float>(orth->zoom + zoomSpeed * dt, 0.2, 10.0f);
		}
		else if (game->input.keyboardMouse.keyboard.numbers[2]) {
			orth->zoom = clamp<float>(orth->zoom + zoomSpeed * -dt, 0.2, 10.0f);
		}

		if (!game->input.keyboardMouse.mouse.right)
			isDragging = false;

		camera->transform->move(game->input.keyboardMouse.lAxis.resize<3>() * dt * panSpeed * orth->zoom);

		auto colorShader = game->resources->getShaderProgram("color");
		colorShader->use();
		colorShader->setMVP(Mat4::Identity(), Mat4::Identity(), projMat);

		for (int i = 0; i <= partitions[1]; i++) {
			hagame::graphics::drawLine(
				hagame::graphics::Line(Vec3({ 0.0f, i * cellSize[1], 0.0f }), Vec3({ cellSize[0] * partitions[0], i * cellSize[1], 0.0f }), lineColor),
				colorShader
			);
		}

		for (int i = 0; i <= partitions[0]; i++) {
			hagame::graphics::drawLine(
				hagame::graphics::Line(Vec3({ i * cellSize[0], 0.0f, 0.0f }), Vec3({i * cellSize[0], cellSize[1] * partitions[1], 0.0f }), lineColor),
				colorShader
			);
		}

		if (mouseIndex[0] >= 0 && mouseIndex[1] >= 0 && mouseIndex[0] <= partitions[0] && mouseIndex[1] <= partitions[1]) {

			orth->zoom = clamp<float>(orth->zoom + -game->input.keyboardMouse.mouse.wheel * 0.25f, 0.2, 10.0f);

			if (game->input.keyboardMouse.mouse.rightPressed) {
				isDragging = true;
				dragStart = screenPos;
				camDragStart = camera->transform->position.resize<2>();
			}

			if (selectionMode == SelectionMode::GridVertex)
				hagame::graphics::drawRect(
					Rect(closestVertex - Vec2(0.15f), Vec2(0.3f)),
					hagame::graphics::Color::red(),
					colorShader
				);
			else
				hagame::graphics::drawRect(
					Rect(mouseIndex.cast<float>() + cellSize * 0.05, cellSize * 0.9),
					hagame::graphics::Color::green(),
					colorShader
				);

			switch (mode) {
			case BuilderMode::GameObjectMode:
				handleGameObjectMode();
				break;
			case BuilderMode::PolygonMode:
				handlePolygonMode();
				break;
			case BuilderMode::SpriteMode:
				handleSpriteMode();
				break;
			}
			
		}

		if (isDragging) {
			auto delta = (screenPos - dragStart) * orth->zoom / pixelsPerMeter;
			camera->transform->setPosition((camDragStart - delta).resize<3>());
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(game->window->window);
		ImGui::NewFrame();
		ImGui::ShowDemoWindow();
		{
			auto rect = layout->getColumns(0, 1);
			ImGui::Begin("Entity Tree");
			ImGui::SetWindowPos(ImVec2(rect.pos[0], rect.pos[1]));
			ImGui::SetWindowSize(ImVec2(rect.size[0], rect.size[1]));
			drawGameObjectTree();
			ImGui::End();
		}

		{
			auto rect = rightLayout->getRows(0, 0);
			ImGui::Begin("Editor Settings");
			ImGui::Text(("Mouse_X: " + std::to_string(mousePos[0]) + " Mouse_Y: " + std::to_string(mousePos[1])).c_str());
			ImGui::Text(("X: " + std::to_string(mouseIndex[0]) + " Y: " + std::to_string(mouseIndex[1])).c_str());
			ImGui::SliderInt2("Map size (m)", partitions.vector, 1, 200);
			ImGui::SliderFloat("Zoom level", &orth->zoom, 0.2f, 10.0f);

			for (int i = 0; i < BuilderModeNames.size(); i++) {
				if (ImGui::RadioButton(BuilderModeNames[i].c_str(), mode == i)) {
					mode = (BuilderMode)i;
				}
			}



			ImGui::SetWindowPos(ImVec2(rect.pos[0], rect.pos[1]));
			ImGui::SetWindowSize(ImVec2(rect.size[0], rect.size[1]));
			ImGui::End();
		}

		{
			auto rect = rightLayout->getRows(1, 3);
			ImGui::Begin("Inspector");
			ImGui::SetWindowPos(ImVec2(rect.pos[0], rect.pos[1]));
			ImGui::SetWindowSize(ImVec2(rect.size[0], rect.size[1]));

			if (selectedEntity.has_value()) {
				ImGui::DragFloat3("Position", selectedEntity.value()->transform->position.vector, 1, 0, max(partitions[0], partitions[1]));
			}

			ImGui::End();
		}

		/*{
			auto rect = centerLayout->getRows(9, 11);
			ImGui::Begin("Console");
			ImGui::SetWindowPos(ImVec2(rect.pos[0], rect.pos[1]));
			ImGui::SetWindowSize(ImVec2(rect.size[0], rect.size[1]));
			ImGui::End();
		}*/

		for (auto entId : ecs.getRegistry()->view<PlaceHolderComponent>()) {
			auto entity = ecs.entities.getByEnttId(entId);
			hagame::graphics::drawRect(
				Rect(entity->transform->position.resize<2>() + cellSize * 0.05, cellSize * 0.9),
				selectedEntity.has_value() && selectedEntity.value()->id == entity->id ? hagame::graphics::Color::blue() : hagame::graphics::Color::white(),
				colorShader
			);
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


	}
};

#endif
