#include "Runtime.h"

using namespace hagame::input::devices;
using namespace hagame::graphics;
using namespace hagame::physics;

void Runtime::onSceneInit()
{
	// Load systems
	addSystem<RenderSystem>();
	addSystem<PartitionSystem>();
	addSystem<CollisionSystem>();
	addSystem<PlatformerSystem>();
	addSystem<UISystem>();
	addSystem<AISystem>();
	addSystem<CustomRenderSystem>();
	addSystem<PhysicsSystem>();
	addSystem<WeaponSystem>();
	addSystem<HealthSystem>();

	game->window->pixelsPerMeter = 50.0f;

	// Initialize variables and such
	initializeEditorTools();
	m_gameConfig = m_config.load("Game");
	m_devMode = m_gameConfig->getBool("RuntimeSettings", "dev_mode");

	m_previewer = addSprite("prototype", mousePos, BLOCK_SIZE);
	m_previewer->getComponent<Sprite2DRenderer>()->sprite->renderMode = RenderMode::Unlit;
	m_previewer->active = false;

	addCamera();
	camera->addComponent<OrthographicCameraDragController>();
	uiOrth = std::make_shared<OrthographicCamera>(game->window->size);
	uiOrth->centered = false;

	// Subscribe to any events
	game->input.windowEvents.subscribe(hagame::input::WindowEvents::Resized, [this](hagame::input::WindowEvent e) {
		setWindowSize(e.data);
		game->window->setViewport(Rect(Vec2::Zero(), e.data));
	});
}

void Runtime::onSceneActivate()
{
	game->window->pixelsPerMeter = 50.0f;
}

void Runtime::onSceneBeforeUpdate()
{
	game->window->ambientColor = Color::black();
	game->window->clearColor = Color::black();

}

void Runtime::onSceneUpdate(double dt)
{
	auto rawMousePos = game->input.keyboardMouse.mouse.position;
	rawMousePos[1] = game->window->size[1] - rawMousePos[1];

	setMousePos(game->input.keyboardMouse.mouse.position);

	if (m_devMode) {

		Ptr<EditorTool> activeTool;

		for (auto tool : EDITOR_TOOLS) {
			if (editorMode.value() == tool->id) {
				activeTool = tool;
			}
		}

		auto tris = computeLineOfSight(mousePos);

		for (auto tri : tris) {
			hagame::graphics::drawTriangle(tri, Color::blue(), DEBUG_SHADER, 0.1);
		}

		for (auto edge : edges) {
			hagame::graphics::drawLine(hagame::math::Line(edge.start, edge.end), Color::blue(), DEBUG_SHADER, 0.1);
			hagame::graphics::drawRect(Rect(edge.start, Vec2(0.05)), Color::red(), DEBUG_SHADER, 0.1);
			hagame::graphics::drawRect(Rect(edge.end, Vec2(0.05)), Color::red(), DEBUG_SHADER, 0.1);
		}

		for (auto light : m_lights) {
			hagame::graphics::drawCircle2D(light.entity->getPos(), 0.5, Color::white());
		}

		renderDeveloperWindow();

		if (!running) {

			activeTool->onUpdate(mousePos);

			if (game->input.keyboardMouse.mouse.left && !guiActive()) {
				activeTool->onPlace(mousePos);
			}

			if (game->input.keyboardMouse.mouse.right && !guiActive()) {
				activeTool->onRemove(mousePos);
			}

			camera->getComponent<OrthographicCameraDragController>()->update(
				camera,
				rawMousePos / game->window->pixelsPerMeter,
				game->input.keyboardMouse.mouse.middle
			);

			orth->zoomIn(game->input.keyboardMouse.mouse.wheel * dt * 1000.0);
		}
	}
	
	if (running) {
		getSystem<WeaponSystem>()->setMousePos(mousePos);
		camera->setPos(m_player->getPos());
	}
}

void Runtime::onSceneAfterUpdate()
{
	auto shader = game->resources->getShaderProgram("batch_line");
	shader->use();
	shader->setMVP(Mat4::Identity(), Mat4::Identity(), orth->getProjMatrix(camera->transform.get()));
	lineBuffer.draw();
}

void Runtime::onSceneDeactivate()
{
	game->window->pixelsPerMeter = 1.0f;
}

void Runtime::loadLevel(String level)
{
}

void Runtime::renderDeveloperWindow()
{
	ImGui::ShowDemoWindow();
	ImGui::Begin("Developer Window");

	if (running) {
		if (ImGui::Button("Stop")) stop();
		ImGui::SameLine();
		if (ImGui::Button("Reset")) reset();
	}
	else {
		if (ImGui::Button("Play")) play();
		// ImGui::SameLine();
		// if (ImGui::Button("Reset")) reset();
	}

	ImGui::Text(("Frame Time: " + std::to_string(1.0 / game->fps)).c_str());
	ImGui::Text(("Mouse Pos: " + mousePos.toString()).c_str());

	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

	int renderMode = (int)game->window->renderPassMode;

	for (int i = 0; i < RENDER_MODES.size(); i++) {
		ImGui::RadioButton(RENDER_MODES[i].c_str(), &renderMode, i);
	}

	game->window->renderPassMode = (RenderMode)renderMode;

	for (auto tool : EDITOR_TOOLS) {
		ImGui::RadioButton(tool->label.c_str(), editorMode.ref(), tool->id);
	}

	editorMode.clean();

	ImGui::SeparatorEx(ImGuiSeparatorFlags_Horizontal);

	for (auto tool : EDITOR_TOOLS) {
		if (tool->id == editorMode.value()) {
			tool->onMenuRender();
		}
	}

	ImGui::End();
}

void Runtime::play()
{
	running = true;
	m_playerStart->active = false;
	m_startTime = game->secondsElapsed;
	//m_player = addSprite("henry", m_playerStart->getPos(), BLOCK_SIZE);
	m_player = addEXR("test_exr", m_playerStart->getPos(), BLOCK_SIZE);
	m_player->addTag("player");
	m_player->name = "player";
	m_player->addComponent<PlayerController>();
	auto blastEmitter = m_player->addComponent<ParticleEmitterRenderer>();
	blastEmitter->shader = game->resources->getShaderProgram("particle");
	auto weaponController = m_player->addComponent<WeaponController>();
	weaponController->playerControlled = true;

	addPhysics(m_player, 1);
	addBoxCollider(m_player, BLOCK_SIZE);
}

void Runtime::stop()
{
	running = false;
	m_playerStart->active = true;
	removeEntity(m_player);
}

void Runtime::reset()
{
	m_player->setPos(m_playerStart->getPos());
}

void Runtime::initializeEditorTools()
{
	// Player Start Tool
	auto playerStartTool = std::make_shared<EditorTool>("Player Start");
	playerStartTool->onActivate = [this]() {
		m_previewer->active = true;
		setPreviewer(game->resources->getTexture("player_start"), BLOCK_SIZE);
	};
	playerStartTool->onUpdate = [this](Vec2 pos) {
		m_previewer->setPos(mousePos.rounded());
	};
	playerStartTool->onPlace = [this](Vec2 pos) {
		if (m_playerStart == NULL) {
			m_playerStart = addSprite("player_start", pos.rounded(), game->resources->getTexture("player_start")->image->size.cast<float>().fill(BLOCK_SIZE));
		}
		m_playerStart->setPos(pos.rounded());
	};

	// Tile tool
	auto tileTool = std::make_shared<EditorTool>("Tiles");
	tileTool->onActivate = [this]() {
		m_previewer->active = true;
		setPreviewer(game->resources->getTexture("prototype"), BLOCK_SIZE);
	};
	tileTool->onUpdate = [this](Vec2 pos) {
		renderBlockOutline(pos);
		m_previewer->setPos(pos.rounded());
	};
	tileTool->onPlace = [this](Vec2 pos) {
		auto idx = mousePos.rounded();
		if (!m_tiles.has(idx[0], idx[1])) {
			auto entity = addSprite("prototype", idx * BLOCK_SIZE, BLOCK_SIZE);
			addBoxCollider(entity, BLOCK_SIZE, false);
			m_tiles.insert(idx[0], idx[1], RuntimeEntity{ entity, idx * BLOCK_SIZE });
			edges = m_tiles.getEdges();
		}
	};
	tileTool->onRemove = [this](Vec2 pos) {
		auto idx = mousePos.rounded();
		if (m_tiles.has(idx[0], idx[1])) {
			auto entity = m_tiles.get(idx[0], idx[1]).entity;
			m_tiles.remove(idx[0], idx[1]);
			removeEntity(entity);
			edges = m_tiles.getEdges();
		}
	};

	// Light tool
	auto lightTool = std::make_shared<EditorTool>("Lighting");
	lightTool->onActivate = [this]() {
		m_previewer->active = false;
	};
	lightTool->onPlace = [this](Vec2 pos) {
		auto light = addLight(mousePos, 20.0f, lightEditor.color);
		auto renderer = light->getComponent<DiscLightRenderer>();
		renderer->constant = lightEditor.constant;
		renderer->linear = lightEditor.linear;
		renderer->quadratic = lightEditor.quadratic;
		m_lights.push_back(RuntimeEntity{ light, mousePos });
	};
	lightTool->onMenuRender = [this]() {
		lightEditor.render();
	};

	EDITOR_TOOLS.push_back(tileTool);
	EDITOR_TOOLS.push_back(playerStartTool);
	EDITOR_TOOLS.push_back(lightTool);
	
}

void Runtime::setPreviewer(RawPtr<Texture> texture, Vec2 size)
{
	auto sprite = m_previewer->getComponent<Sprite2DRenderer>();
	sprite->sprite->texture = texture;
	sprite->sprite->quad->setSize(texture->image->size.cast<float>().fill(size));
}

Array<hagame::math::Triangle> Runtime::computeLineOfSight(Vec2 pos)
{
	struct EdgePoint {
		Vec2 pos;
		float theta;
	};

	Array<hagame::math::Triangle> triangles;
	Array<EdgePoint> points;
	hagame::math::Triangle currentTri;
	Vec2 nearestWall;
	float nearestDist;
	bool hasNearest = false;
	bool triStarted = false;
	float t;

	for (auto edge : edges) {
		auto sDelta = pos - edge.start;
		auto eDelta = pos - edge.end;
		points.push_back(EdgePoint{ edge.start, atan2(sDelta.y(), sDelta.x()) });
		points.push_back(EdgePoint{ edge.end, atan2(eDelta.y(), eDelta.x()) });
	}

	std::sort(points.begin(), points.end(), [](EdgePoint a, EdgePoint b) {
		return a.theta > b.theta;
	});

	

	for (auto point : points) {
		bool hit = false;
		Vec2 nearestHit;
		for (auto edge : edges) {

			if (linesIntersect(pos, point.pos, edge.start, edge.end, t) && t < nearestDist) {
				hit = true;
				nearestHit = edge.start + (edge.end - edge.start) * t;
				nearestDist = t;
			}

			if (hit) {
				hagame::graphics::drawRect(Rect(nearestHit, Vec2(0.25)), Color::red());

				if (!hasNearest || nearestHit != nearestWall) {
					hasNearest = true;
					nearestWall = nearestHit;

					if (!triStarted) {
						currentTri = hagame::math::Triangle();
						currentTri.a = pos;
						currentTri.b = nearestWall;
						triStarted = true;
					}
					else {
						currentTri.c = nearestWall;
						triangles.push_back(currentTri);
						triStarted = false;
					}
				}
			}
		}
	}

	return triangles;
}
