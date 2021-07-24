#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include "../../HaGame/HaGame.h"

enum PlacementMode {
	Start,
	Goal,
	Obstacle,
};

const String PlacementModeStrings[] = {
	"Start",
	"Goal",
	"Obstacle"
};

class Pathfinding : public hagame::ecs::System {

public:
	Ptr<hagame::graphics::PixelGrid> pixels;
	const float scale = 0.05;
	const hagame::graphics::Color clearColor = hagame::graphics::Color("#afb3b0");
	const hagame::graphics::Color goalColor = hagame::graphics::Color("#00d912");
	const hagame::graphics::Color startColor = hagame::graphics::Color("#0f2fff");
	const hagame::graphics::Color obstacleColor = hagame::graphics::Color("#c22202");

	const Vec2 padding = Vec2(0.1);
	PlacementMode mode = PlacementMode::Start;
	Vec2Int mouseGridPos;
	Vec3 start;
	Vec3 goal;
	hagame::utils::GridMap<Ptr<hagame::utils::AStarNode>> nodeGrid;
	Ptr<hagame::utils::AStarNode> startNode, goalNode;
	Array<Vec2Int> obstacles;
	hagame::utils::AStar pathfinder;

	bool first = false;
	Vec3 min, max;

	Ptr<hagame::utils::AStarNode> currentNode;
	bool isShowingPath = false;
	bool isPathfinding = false;
	float iterationDelay = 0.016;
	float currentDelay = 0.0f;
	float cost = 1.0f;

	float uiPercent = 0.2;

	bool isObstacle(Vec2Int index) {
		for (auto obs : obstacles) {
			if (index == obs)
				return true;
		}
		return false;
	}

	void onSystemStart() {
		pixels = std::make_shared<hagame::graphics::PixelGrid>(
			(game->window->size.resize<2>() * scale).cast<int>(),
			game->window->size,
			clearColor,
			padding
		);

		for (int i = 0; i < pixels->partitions[1]; i++) {
			for (int j = 0; j < pixels->partitions[0]; j++) {
				auto node = std::make_shared<hagame::utils::AStarNode>();
				node->position = Vec3({ (float)j, (float)i, 0 });
				node->cost = isObstacle(Vec2Int({ j, i })) ? 1.0 : 0.0;

				nodeGrid.set(i, j, node);
			}
		}

		pathfinder.getNeighbors = [this](Ptr<hagame::utils::AStarNode> node) {
			auto neighbors = Array<Ptr<hagame::utils::AStarNode>>();

			for (int y = -1; y < 2; y++) {
				for (int x = -1; x < 2; x++) {
					if (y == 0 && x == 0 )
						continue;
					if (node->position[1] + y >= 0 && node->position[1] + y < pixels->partitions[1] && node->position[0] + x >= 0 && node->position[0] + x < pixels->partitions[0]) {
						neighbors.push_back(nodeGrid.getOrFail(node->position[1] + y, node->position[0] + x));
					}
				}
			}

			return neighbors;
		};

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

	void handleInputs() {
		Vec2 mousePos = game->input.keyboardMouse.mouse.position;
		mousePos[1] = game->window->size[1] - mousePos[1];
		//mousePos += pixels->pixelSize * 0.5;

		mousePos += scene->getSystem<ViewportSystem>()->offset;

		if (mousePos[0] >= game->window->size[0] * (1 - uiPercent))
			return;

		mouseGridPos = pixels->getIndex(mousePos);

		if (!RectInt(Vec2Int::Zero(), pixels->partitions).contains(mouseGridPos))
			return;

		if (game->input.keyboardMouse.mouse.left) {

			/*
			reset();
			pixels->setColor(mouseGridPos[1], mouseGridPos[0], hagame::graphics::Color::purple());
			
			for (auto neighbor : pathfinder.getNeighbors(nodeGrid.getOrFail(mouseGridPos[1], mouseGridPos[0]))) {
				pixels->setColor(neighbor->position[1], neighbor->position[0], hagame::graphics::Color::red());
			}
			*/
			switch (mode) {
			case PlacementMode::Start:

				pixels->setColor(start[1], start[0], clearColor);
				start = mouseGridPos.cast<float>().resize<3>();
				startNode = nodeGrid.getOrFail(mouseGridPos[1], mouseGridPos[0]);
				pixels->setColor(start[1], start[0], startColor);
				if (!first) {
					first = true;
					min = start;
					max = start;
				}

				for (int i = 0; i < 2; i++) {
					if (start[i] < min[i]) {
						min[i] = start[i];
					}

					if (start[i] > max[i]) {
						max[i] = start[i];
					}
				}
				break;
			case PlacementMode::Goal:

				pixels->setColor(goal[1], goal[0], clearColor);
				goal = mouseGridPos.cast<float>().resize<3>();
				goalNode = nodeGrid.getOrFail(mouseGridPos[1], mouseGridPos[0]);
				pixels->setColor(goal[1], goal[0], goalColor);
				if (!first) {
					first = true;
					min = goal;
					max = goal;
				}

				for (int i = 0; i < 2; i++) {
					if (goal[i] < min[i]) {
						min[i] = goal[i];
					}

					if (goal[i] > max[i]) {
						max[i] = goal[i];
					}
				}

				break;
			case PlacementMode::Obstacle:

				bool exists = false;
				for (auto obs : obstacles) {
					if (obs == mouseGridPos) {
						exists = true;
						break;
					}
				}
				if (!first) {
					first = true;
					min = mouseGridPos.cast<float>().resize<3>();
					max = mouseGridPos.cast<float>().resize<3>();
				}

				for (int i = 0; i < 2; i++) {
					if (mouseGridPos[i] < min[i]) {
						min[i] = mouseGridPos[i];
					}

					if (mouseGridPos[i] > max[i]) {
						max[i] = mouseGridPos[i];
					}
				}

				if (!exists && mouseGridPos.cast<float>().resize<3>() != goal && mouseGridPos.cast<float>().resize<3>() != start) {
					obstacles.push_back(mouseGridPos);
					nodeGrid.getOrFail(mouseGridPos[1], mouseGridPos[0])->cost = cost;
					pixels->setColor(mouseGridPos[1], mouseGridPos[0], hagame::graphics::Color(cost, 0.0, 0.0));
				}

				break;
			}
		}

		if (game->input.keyboardMouse.mouse.right) {
			switch (mode) {
			case PlacementMode::Obstacle:
				for (int i = 0; i < obstacles.size(); i++) {

					if (obstacles[i] == mouseGridPos) {
						obstacles.erase(obstacles.begin() + i);
						pixels->setColor(mouseGridPos[1], mouseGridPos[0], clearColor);
						nodeGrid.getOrFail(mouseGridPos[1], mouseGridPos[0])->cost = 0.0;
					}
				}

				break;
			default:
				break;
			}
		}

		if (game->input.keyboardMouse.keyboard.numbersPressed[1]) {
			mode = PlacementMode::Start;
		}

		if (game->input.keyboardMouse.keyboard.numbersPressed[2]) {
			mode = PlacementMode::Goal;
		}

		if (game->input.keyboardMouse.keyboard.numbersPressed[3]) {
			mode = PlacementMode::Obstacle;
		}
	}

	void reset() {
		for (int i = 0; i < pixels->partitions[0]; i++) {
			for (int j = 0; j < pixels->partitions[1]; j++) {
				pixels->setColor(j, i, clearColor);
				nodeGrid.getOrFail(j, i)->cost = 0.0;
			}
		}
		obstacles.clear();
		startNode = NULL;
		goalNode = NULL;
		isShowingPath = false;
		isPathfinding = false;
	}

	void handleUI() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(game->window->window);
		ImGui::NewFrame();

		{
			ImGui::SetNextWindowPos(ImVec2(game->window->size[0] - game->window->size[0] * uiPercent, 0));
			ImGui::SetNextWindowSize(ImVec2(game->window->size[0] * uiPercent, game->window->size[1]));
			ImGui::Begin("AStar Settings");
			ImGui::Text(mouseGridPos.toString().c_str());
			ImGui::Text("Control the pathfinding simulation");
			ImGui::SliderFloat("Iteration delay", &iterationDelay, 0, 1.0, "delay = %.3f");
			ImGui::SliderFloat("Obstacle cost", &cost, 0.0, 1.0, "cost = %.3f");

			for (int i = 0; i < 3; i++) {
				if (ImGui::RadioButton(PlacementModeStrings[i].c_str(), mode == i)) {
					mode = (PlacementMode) i;
				}
				if (i < 3 - 1)
					ImGui::SameLine();
			}

			if (startNode && goalNode && ImGui::Button("Pathfind!")) {
				pathfinder.reset();
				pathfinder.init(startNode, goalNode);

				isPathfinding = true;
			}

			if (ImGui::Button("Reset")) {
				reset();
			}

			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void onSystemUpdate(double dt) {

		handleInputs();
		

		currentDelay += dt;
		if (isPathfinding && currentDelay >= iterationDelay) {
			currentDelay = 0;

			pathfinder.iterate();

			for (auto cell : pathfinder.openList) {

				if (cell != goalNode && cell != startNode)
					pixels->setColor(cell->position[1], cell->position[0], hagame::graphics::Color::purple());
			}
	
			if (pathfinder.complete) {
				currentNode = goalNode;
				isPathfinding = false;
				if (pathfinder.pathExists) {
					isShowingPath = true;
				}
			}
		}

		if (isShowingPath && currentDelay >= iterationDelay) {
			if (currentNode != goalNode && currentNode != startNode)
				pixels->setColor(currentNode->position[1], currentNode->position[0], hagame::graphics::Color::white());

			if (currentNode == startNode)
				isShowingPath = false;
			else
				currentNode = currentNode->parent;
		}

		game->resources->getShaderProgram("pixel")->use();
		game->resources->getShaderProgram("pixel")->setMat4("projection", scene->getSystem<ViewportSystem>()->projection);
		pixels->draw(game->resources->getShaderProgram("pixel"));

		handleUI();

	}
};

#endif
