#ifndef G_MAPBUILDER_H
#define G_MAPBUILDER_H

#include "../../HaGame/HaGame.h"
#include "../../HaGame/UI/UI.h"

using Entity = Ptr<hagame::ecs::Entity>;
using TreeEventType = hagame::ui::EntityTree::EventType;
using TreeEvent = hagame::ui::EntityTree::Event;

hagame::graphics::Color GRAY_LIGHTEST = hagame::graphics::Color("#636363");
hagame::graphics::Color GRAY_LIGHTER = hagame::graphics::Color("#787878");
hagame::graphics::Color GRAY_LIGHT = hagame::graphics::Color("#4D4D4D");
hagame::graphics::Color GRAY = hagame::graphics::Color("#474747");

const Vec2 GRID_SIZE = Vec2(100.0f);

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
	bool expanded = false;
	bool selected = false;
};

struct PlaceHolderComponent  {
	String name;
	hagame::graphics::Color color;
};

struct SpriteRenderer {

};

const Array<String> ResourceTabs = {
	"Textures",
	"Shaders",
	"Audio",
	"Fonts"
};

struct PolygonComponent {
	Array<hagame::math::Line> lines;

	hagame::graphics::Color color = hagame::graphics::Color::green();
	Ptr<hagame::graphics::Sprite> sprite;

	Array<Vec3> polygon;
	Array<int> activePoints;
	Set<int> reflexVerts;
	Set<int> convexVerts;
	Set<int> earTips;

	Ptr<hagame::graphics::VertexBuffer<Vec4>> vbo;
	Ptr<hagame::graphics::VertexArray> vao;
	
	bool hasBuffers = false;

	Array<hagame::math::Triangle> triangles;

	hagame::math::Triangle getTriangle(int idx) {
		auto& [lIdx, rIdx] = getAdjacentVertices(idx);
		return hagame::math::Triangle(polygon[lIdx], polygon[idx], polygon[rIdx]);
	}

	hagame::math::Triangle getTriangle(Vec3 p) {
		for (int i = 0; i < activePoints.size(); i++) {
			if (p == polygon[activePoints[i]]) {
				return getTriangle(activePoints[i]);
			}
		}
		throw new std::exception("Point does not exist on polygon");
	}

	// Return the adjacent active vertex indices for a point
	std::tuple<int, int> getAdjacentVertices(int idx) {
		int N = activePoints.size();
		for (int i = 0; i < N; i++) {
			if (activePoints[i] == idx) {
				auto lIdx = i - 1;
				if (lIdx == -1)
					lIdx = N - 1;
				auto rIdx = (i + 1) % N;
				return std::make_tuple(activePoints[lIdx], activePoints[rIdx]);
			}
		}
		throw new std::exception("Index not in active vertices!");
	}

	void removeActiveVertex(int idx) {
		for (int i = 0; i < activePoints.size(); i++) {
			if (idx == activePoints[i]) {
				activePoints.erase(activePoints.begin() + i);
				return;
			}
		}
	}

	bool isConvex(int idx) {
		return convexVerts.find(idx) != convexVerts.end();
	}

	bool isEarTest(int idx) {
		if (!isConvex(idx))
			return false;

		auto tri = getTriangle(idx);

		for (auto refVert : reflexVerts) {
			if (polygon[refVert] != tri.a && polygon[refVert] != tri.c && tri.containsPoint(polygon[refVert]))
				return false;
		}

		return true;
	}

	void updateVertex(int idx) {
		if (!isConvex(idx)) {
			auto tri = getTriangle(idx);
			auto angle = (tri.c - tri.b).angleBetween(tri.a - tri.b);
			if (angle > 0) {
				reflexVerts.erase(idx);
				convexVerts.emplace(idx);
			}
		}
	}

	Vec4 addTextureMapping(Vec3 vert) {
		float xUv = std::fmod(vert[0], 1.0f);
		float yUv = std::fmod(vert[1], 1.0f);
		if (xUv == 0.0f && vert[0] / 2 == 0.0f)
			xUv = 1.0f;
		if (yUv == 0.0f && vert[1] / 2 == 0.0f)
			yUv = 1.0f;

		return Vec4({ vert[0], vert[1], vert[0], vert[1] });
	}

	void generateBuffers() {

		auto points = Array<Vec4>();

		for (auto tri : triangles) {
			// If you cant see try reverse thsi order...

			// points.insert(points.end(), { tri.c, tri.b, tri.a });

			Vec4 c = addTextureMapping(tri.c);
			Vec4 b = addTextureMapping(tri.b);
			Vec4 a = addTextureMapping(tri.a);

			std::cout << a.toString() << "\n";

			points.insert(points.end(), { c, b, a });
		}

		vao = std::make_shared<hagame::graphics::VertexArray>();
		vbo = hagame::graphics::VertexBuffer<Vec4>::Static(points);
		vao->initialize();
		vao->defineAttribute<Vec4>(vbo.get(), hagame::graphics::DataType::Float, 0, 4);

		hasBuffers = true;
	}

	void tesselate() {

		int N = lines.size();

		// Fill in algorithm to generate the triangles from the bounds of the polygon
		if (lines.size() < 3) {
			return;
		}
		else if (lines.size() == 3) {
			triangles.push_back(hagame::math::Triangle(lines[0].a, lines[0].b, lines[1].b));
			return;
		}
		else {

			float edgeSum = 0.0f;

			for (auto line : lines) {
				polygon.push_back(line.a);
				edgeSum += ((line.b[0] - line.a[0]) / (line.b[1] + line.a[1]));
			}

			// If the edge sum is less than 0 than the winding order is clock wise and we need to reverse it.
			if (edgeSum < 0) {
				std::reverse(polygon.begin(), polygon.end());
			}

			for (int i = 0; i < N; i++) {
				activePoints.push_back(i);
				auto lIdx = i - 1;
				if (lIdx == -1)
					lIdx = N - 1;
				auto rIdx = (i + 1) % N;
				auto tri = hagame::math::Triangle(polygon[lIdx], polygon[i], polygon[rIdx]);
				auto angle = (tri.c - tri.b).angleBetween(tri.a - tri.b);


				if (angle <= 0) {
					reflexVerts.emplace(i);
				}
				else {
					convexVerts.emplace(i);
				}
			}

			for (int i = 0; i < N; i++) {
				if (isEarTest(i)) {
					earTips.emplace(i);
				}
			}

			for (auto ear : earTips) {

				if (activePoints.size() == 3) {
					triangles.push_back(hagame::math::Triangle(polygon[activePoints[0]], polygon[activePoints[1]], polygon[activePoints[2]]));
					break;
				}

				auto tri = getTriangle(ear);
				triangles.push_back(tri);
				auto& [lIdx, rIdx] = getAdjacentVertices(ear);

				removeActiveVertex(ear);

				updateVertex(lIdx);
				updateVertex(rIdx);

				if (isEarTest(lIdx)) {
					earTips.emplace(lIdx);
				}
				else {
					earTips.erase(lIdx);
				}

				if (isEarTest(rIdx)) {
					earTips.emplace(rIdx);
				}
				else {
					earTips.erase(rIdx);
				}

				earTips.erase(ear);
				
			}
		}

		generateBuffers();
	}

	void render() {
		if (hasBuffers) {
			vao->bind();
			glActiveTexture(GL_TEXTURE0);
			sprite->texture->bind();
			glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3);
		}
	}
};

template <>
void ComponentEditorWidget<PolygonComponent>(hagame::Game* game, Ptr<hagame::ecs::Entity> entity) {
	auto poly = entity->getComponent<PolygonComponent>();
	ImGui::ColorEdit4("Fill Color", poly->color.vector);

	auto imgSize = poly->sprite->texture != NULL ? poly->sprite->texture->image.size.cast<float>().fill(Vec2(100.0f)) : Vec2(100.0f);
	if (poly->sprite->texture != NULL) {
		ImGui::Image((ImTextureID)poly->sprite->texture->id, ImVec2(imgSize[0], imgSize[1]));
	}
	else {
		ImGui::Button("No texture!");
	}

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE")) {
			String textName = *(const String*)payload->Data;
			auto text = game->resources->getTexture(textName);
			poly->sprite->texture = text;
			poly->sprite->rect.size = text->image.size.cast<float>().bounded(Vec2(1.0f));
			ImGui::EndDragDropTarget();
		}
	}
};

template <>
void ComponentEditorWidget<PlaceHolderComponent>(hagame::Game* game, Ptr<hagame::ecs::Entity> entity) {
	auto ph = entity->getComponent<PlaceHolderComponent>();
	ImGui::ColorEdit4("Color", ph->color.vector);
};

template <>
void ComponentEditorWidget<hagame::graphics::SpriteRenderer>(hagame::Game* game, Ptr<hagame::ecs::Entity> entity) {
	auto renderer = entity->getComponent<hagame::graphics::SpriteRenderer>();
	auto imgSize = renderer->sprite->texture != NULL ? renderer->sprite->texture->image.size.cast<float>().fill(Vec2(100.0f)) : Vec2(100.0f);
	if (renderer->sprite->texture != NULL) {
		ImGui::Image((ImTextureID)renderer->sprite->texture->id, ImVec2(imgSize[0], imgSize[1]));
	}
	else {
		ImGui::Button("No texture!");
	}
	
	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE")) {
			String textName = *(const String*)payload->Data;
			auto text = game->resources->getTexture(textName);
			renderer->sprite->texture = text;
			renderer->sprite->rect.size = text->image.size.cast<float>().bounded(Vec2(1.0f));
			ImGui::EndDragDropTarget();
		}
	}

	if (renderer->shader != NULL) {
		//ImGui::BeginGroup();
		// ImGui::Image((ImTextureID)game->resources->getTexture("shader_texture"), ImVec2(75.0f, 75.0f));
		ImGui::Text(renderer->shader->name.c_str());
	}
	else {
		ImGui::Text("Missing Shader :(");
	}

	if (ImGui::BeginDragDropTarget()) {
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SHADER")) {
			String shaderName = *(const String*)payload->Data;
			renderer->shader = game->resources->getShaderProgram(shaderName);
		}
	}

	ImGui::DragFloat2("Sprite size", renderer->sprite->rect.size.vector, 0.01, 0.0, 5.0f);
}

class MapBuilder : public hagame::Scene {

	hagame::ui::EntityTree entityTree;
	hagame::ui::ResourceBrowser resourceBrowser;

	const hagame::graphics::Color lineColor = hagame::graphics::Color("#424242");

	// Cell size in meters

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

	Array<hagame::math::Line> lines;
	Vec2 lineP1;
	Set<Vec2> polygonPoints;
	bool drawingLine = false;

	bool resourceTabs[4] = { true, false, false, false };

	void onSceneInit() {

		layout = hagame::utils::RectGrid::Columns(Rect(Vec2::Zero(), game->window->size), 12);
		centerLayout = hagame::utils::RectGrid::Rows(layout->getColumns(2, 9), 12);
		rightLayout = hagame::utils::RectGrid::Rows(layout->getColumns(10, 11), 4);

		viewport = centerLayout->getRows(0, 7);

		game->window->setViewport(viewport);
		game->window->clearColor = GRAY;

		camera = addEntity();
		// camera->transform->move();
		auto camC = camera->addComponent<hagame::graphics::CameraComponent>();
		orth = std::make_shared<hagame::graphics::OrthographicCamera>(viewport.size / pixelsPerMeter);
		camC->camera = orth;

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

		ecs.registerComponent<PlaceHolderComponent>("Place Holder");
		ecs.registerComponent<hagame::graphics::SpriteRenderer>("Sprite Renderer");
		ecs.registerComponent<PolygonComponent>("Polygon");

		entityTree.events.subscribe(TreeEventType::AddEntity, [this](TreeEvent e) { addEntity(); });

		entityTree.events.subscribe(TreeEventType::AddChild, [this](TreeEvent e) {
			addChild(e.entity);
		});

		entityTree.events.subscribe(TreeEventType::AddChildTo, [this](TreeEvent e) {
			addChild(e.target, e.entity);
		});

		entityTree.events.subscribe(TreeEventType::RemoveEntity, [this](TreeEvent e) {
			removeEntity(e.entity);
		});

		entityTree.events.subscribe(TreeEventType::SelectEntity, [this](TreeEvent e) {
			selectedEntity = e.entity;
		});
	}

	void handleGameObjectMode() {
		selectionMode = SelectionMode::GridFace;

		if (game->input.keyboardMouse.mouse.leftPressed) {
			auto go = addEntity();
			auto child = addChild(go);
			go->transform->setPosition((mouseIndex.cast<float>().prod(Vec2(1.0f)).resize<3>()));
			auto ph = go->addComponent<PlaceHolderComponent>();
			auto ph2 = child->addComponent<PlaceHolderComponent>();
			ph->color = hagame::graphics::Color::white();
			ph2->color = hagame::graphics::Color::white();
		}
	}

	void handlePolygonMode() {
		selectionMode = SelectionMode::GridVertex;

		// TODO: lol fix this
		if (game->input.keyboardMouse.keyboard.numbersPressed[0]) {
			if (drawingLine) {
				drawingLine = false;
				lines.clear();
				polygonPoints.clear();
			}
		}

		if (game->input.keyboardMouse.mouse.leftPressed) {
			if (drawingLine) {
				lines.push_back(hagame::math::Line(lineP1.resize<3>(), closestVertex.resize<3>()));

				bool alreadyVisited = false;

				for (auto point : polygonPoints) {
					if (point == closestVertex) {
						alreadyVisited = true;
						break;
					}
				}

				if (alreadyVisited) {

					auto entity = addEntity();
					entity->addComponent<PlaceHolderComponent>();
					auto poly = entity->addComponent<PolygonComponent>();
					poly->lines = lines;
					poly->sprite = std::make_shared<hagame::graphics::Sprite>(game->resources->getTexture("kitty"), Rect(Vec2(1.0f) * 0.5f, Vec2(1.0f)));
					poly->tesselate();

					drawingLine = false;
					lines.clear();
					polygonPoints.clear();
					// TODO: fill this with a texture!
				}
				else {
					polygonPoints.insert(closestVertex);
					lineP1 = closestVertex;
				}
			} else {
				polygonPoints.insert(closestVertex);
				lineP1 = closestVertex;
				drawingLine = true;
			}
		}

		if (drawingLine) {
			hagame::graphics::drawLine(hagame::graphics::Line(hagame::math::Line(lineP1.resize<3>(), closestVertex.resize<3>()), hagame::graphics::Color::red()), game->resources->getShaderProgram("color"));
		}

		for (auto line : lines) {
			hagame::graphics::drawLine(hagame::graphics::Line(line, hagame::graphics::Color::red()), game->resources->getShaderProgram("color"));
		}
	}

	void handleSpriteMode() {
		selectionMode = SelectionMode::GridFace;

		if (game->input.keyboardMouse.mouse.leftPressed) {
			auto go = addEntity();
			go->transform->setPosition((mouseIndex.cast<float>().prod(Vec2(1.0f)).resize<3>()));
			auto sRenderer = go->addComponent<hagame::graphics::SpriteRenderer>();
			go->addComponent<PlaceHolderComponent>();
			sRenderer->sprite = std::make_shared<hagame::graphics::Sprite>(game->resources->getTexture("kitty"), Rect(Vec2(1.0f) * 0.5f, Vec2(1.0f)));
			sRenderer->shader = game->resources->getShaderProgram("sprite");
			auto object = std::make_shared<GameObject>();
		}
	}

	void drawGameObjectTree() {
		ecs.entities.forEach([this](Entity entity) {
			// Draw all root game objects!
			if (entity != NULL && entity->parent == NULL && entity->hasComponent<PlaceHolderComponent>()) {
				entityTree.render(game, entity);
			}
		});
	}

	void drawGrid(Rect _rect, Vec2 cellSize, hagame::graphics::Color color) {

		auto colorShader = game->resources->getShaderProgram("color");
		colorShader->use();
		colorShader->setMVP(Mat4::Identity(), Mat4::Identity(), projMat);

		Vec2Int partitions = _rect.size.div(cellSize).ceil().cast<int>();

		for (int i = 0; i <= partitions[1]; i++) {
			hagame::graphics::drawLine(
				hagame::graphics::Line(Vec3({ 0.0f, i * cellSize[1], 0.0f }), Vec3({ cellSize[0] * partitions[0], i * cellSize[1], 0.0f }), color),
				colorShader
			);
		}

		for (int i = 0; i <= partitions[0]; i++) {
			hagame::graphics::drawLine(
				hagame::graphics::Line(Vec3({ i * cellSize[0], 0.0f, 0.0f }), Vec3({ i * cellSize[0], cellSize[1] * partitions[1], 0.0f }), color),
				colorShader
			);
		}
	}

	void onSceneUpdate(double dt) {

		bool uiHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);

		game->input.keyboardMouse.updateImgui(dt);

		screenPos = game->input.keyboardMouse.mouse.position - viewport.pos;
		screenPos[1] = viewport.size[1] - screenPos[1];
		mousePos = screenPos.copy();
		mousePos = orth->getGamePos(camera->transform, mousePos / pixelsPerMeter);

		mouseIndex = mousePos.cast<int>();
		closestVertex = (mousePos).rounded();

		auto colorShader = game->resources->getShaderProgram("color");
		colorShader->use();
		colorShader->setMVP(Mat4::Identity(), Mat4::Identity(), projMat);

		Rect grid = Rect(-0.5f * GRID_SIZE, GRID_SIZE);

		if (orth->getZoom() < 1.0f) {
			drawGrid(grid, Vec2(1.0f), GRAY_LIGHTEST);
			drawGrid(grid, Vec2(10.0f), GRAY_LIGHTEST);
		}
		else {
			drawGrid(grid, Vec2(1.0f), GRAY_LIGHT);
			drawGrid(grid, Vec2(10.0f), GRAY_LIGHTEST);
		}

		

		if (!uiHovered) {

			if (!game->input.keyboardMouse.mouse.right)
				isDragging = false;

			camera->transform->move(game->input.keyboardMouse.lAxis.resize<3>() * dt * panSpeed * orth->getZoom());

			orth->zoomIn(game->input.keyboardMouse.mouse.wheel * 0.125f);

			if (game->input.keyboardMouse.mouse.rightPressed) {
				isDragging = true;
				dragStart = screenPos;
				camDragStart = camera->transform->getPosition().resize<2>();
			}

			if (selectionMode == SelectionMode::GridVertex)
				hagame::graphics::drawRect(
					Rect(closestVertex - Vec2(0.15f), Vec2(0.3f)),
					hagame::graphics::Color::red(),
					colorShader
				);
			else
				hagame::graphics::drawRect(
					Rect(mouseIndex.cast<float>() + Vec2(1.0f) * 0.05, Vec2(1.0f) * 0.9),
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
			auto delta = (screenPos - dragStart) * orth->getZoom() / pixelsPerMeter;
			camera->transform->setPosition((camDragStart - delta).resize<3>());
		}

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
				char* buf = const_cast<char*>(selectedEntity.value()->name.c_str());

				ImGui::InputText("Name:", buf, 255);

				selectedEntity.value()->name = buf;

				ImGui::DragFloat3("Position", selectedEntity.value()->transform->getPositionRef()->vector, 0.1, GRID_SIZE[0] / -2.0f, GRID_SIZE[0] / 2.0f);
				ImGui::Button("Add Component");
				if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
					ImGui::OpenPopup(std::to_string(selectedEntity.value()->id).c_str());
				}

				if (ImGui::BeginPopup(std::to_string(selectedEntity.value()->id).c_str())) {

					for (auto& [compId, comp] : ecs.componentInfo) {
						if (ImGui::Button(comp->name.c_str())) {
							comp->add(game, selectedEntity.value());
						}
					}

					ImGui::EndPopup();
				}

				for (auto& [compId, comp] : ecs.componentInfo) {
					if (selectedEntity.value()->hasComponent(compId)) {
						ImGui::Separator();
						ImGui::Text(comp->name.c_str());
						comp->widget(game, selectedEntity.value());
					}
				}
			}

			ImGui::End();
		}

		{
			auto rect = centerLayout->getRows(8, 11);
			ImGui::Begin("Resources");
			ImGui::SetWindowPos(ImVec2(rect.pos[0], rect.pos[1]));
			ImGui::SetWindowSize(ImVec2(rect.size[0], rect.size[1]));

			auto maxPerLine = (int)floor(rect.size[0] / 100.0f);

			resourceBrowser.render(game);

			ImGui::End();
		}

		for (auto entId : ecs.getRegistry()->view<PlaceHolderComponent>()) {
			auto entity = ecs.entities.getByEnttId(entId);
			if (entity != NULL) {
				
				colorShader->setMVP(
					Mat4::Translation(entity->transform->getPosition())* Mat4::Rotation(entity->transform->getRotation())* Mat4::Scale(entity->transform->getScale()),
					viewMat,
					projMat
				);

				auto ph = entity->getComponent<PlaceHolderComponent>();
				hagame::graphics::drawRect(
					Rect(Vec2(1.0f) * 0.05, Vec2(1.0f) * 0.9),
					ph->color,
					colorShader
				);
			}
		}

		auto sShader = game->resources->getShaderProgram("sprite");

		sShader->use();

		for (auto entId : ecs.getRegistry()->view<PolygonComponent>()) {
			auto entity = ecs.entities.getByEnttId(entId);
			if (entity != NULL) {
				
				auto poly = entity->getComponent<PolygonComponent>();

				sShader->setMVP(entity->transform->getModelMatrix(), Mat4::Identity(), projMat);
				sShader->setVec4("color",poly->color);

				if (poly->triangles.size() > 0) 
					poly->render();
			}
		}
	}
};

#endif
