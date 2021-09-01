#include "../../HaGame/HaGame.h"
#include "../../HaGame/UI/UI.h"

class SpriteSheetHelper : public hagame::Scene {

public:

	float uiPercent = 0.2;

	float scale = 1.0f;

	Vec2Int gridPartitions = Vec2Int(10);

	Ptr<hagame::graphics::GridLines> grid;
	Ptr<hagame::ui::FileTree> filetree;
	Ptr<hagame::graphics::Sprite> currentSheet;

	hagame::graphics::ShaderProgram* pixelShader;
	hagame::graphics::ShaderProgram* gridLineShader;

	void handleUI() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(game->window->window);
		ImGui::NewFrame();

		// ImGui::ShowDemoWindow();

		{
			ImGui::SetNextWindowPos(ImVec2(game->window->size[0] - game->window->size[0] * uiPercent, 0));
			ImGui::SetNextWindowSize(ImVec2(game->window->size[0] * uiPercent, game->window->size[1]));
			ImGui::Begin("SpriteSheetHelper");

			filetree->render();

			ImGui::SliderFloat("Sprite scale", &scale, 0.1, 4.0, "Scale = %.3f");
			ImGui::SliderInt2("Sprite Sheet Parititons", gridPartitions.vector, 0, 100);

			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void onSceneActivate() {

		addSystem<hagame::graphics::RenderSystem>();

		currentSheet = std::make_shared<hagame::graphics::Sprite>();

		pixelShader = game->resources->loadShaderProgram("pixel", "Shaders/pixel_vert.glsl", "Shaders/pixel_frag.glsl");
		auto spriteShader = game->resources->loadShaderProgram("sprite", "Shaders/sprite_vert.glsl", "Shaders/sprite_frag.glsl");
		gridLineShader = game->resources->loadShaderProgram("grid_line", "Shaders/grid_line_vert.glsl", "Shaders/grid_line_frag.glsl");

		auto display = addEntity();
		display->transform->position = ((game->window->size * 0.5) - Vec2({ (game->window->size[0] * uiPercent) / 2, 0 })).resize<3>();
		auto displayRenderer = display->addComponent<hagame::graphics::SpriteRenderer>();
		displayRenderer->sprite = currentSheet.get();
		displayRenderer->shader = spriteShader;
		displayRenderer->projection = Mat4::Orthographic(0, game->window->size[0], 0, game->window->size[1], 0, 1);

		//grid = std::make_shared<hagame::graphics::PixelGrid>(
		//	(game->window->size.resize<2>() * scale).cast<int>(),
		//	game->window->size,
		//	clearColor,
		//	padding
		//);

		grid = std::make_shared<hagame::graphics::GridLines>(
			Rect(((game->window->size * 0.5) - Vec2({ (game->window->size[0] * uiPercent) / 2, 0 })), Vec2::Zero()),
			gridPartitions,
			hagame::graphics::Color::green(),
			1.0f
			);

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
		Array<String> imgTypes = { "png", "jpg" };
		filetree = std::make_shared<hagame::ui::FileTree>(game->resources->getFileSystem()->basePath, imgTypes);
		filetree->onFileSelect([this](String file) {
			std::cout << "Loading " << file << "\n";
			if (currentSheet->texture != NULL) {
				delete currentSheet->texture;
			}
			currentSheet->texture = new hagame::graphics::Texture(file);

			if (currentSheet->texture->image.size[0] > currentSheet->texture->image.size[1]) {
				if (currentSheet->texture->image.size[0] > (game->window->size[0] * (1 - uiPercent) * 0.9)) {
					scale = (game->window->size[0] * (1 - uiPercent) * 0.9) / currentSheet->texture->image.size[0];
				}
			}
			else {
				if (currentSheet->texture->image.size[1] > game->window->size[1] * 0.9) {
					scale = game->window->size[1] * 0.9 / currentSheet->texture->image.size[1];
				}
			}
			
			currentSheet->rect = Rect(Vec2::Zero(), currentSheet->texture->image.size.cast<float>());
			std::cout << "Complete\n";
		});
	}

	void onSceneUpdate(double dt) {
		handleUI();

		if (currentSheet->texture != NULL) {
			currentSheet->rect = Rect(Vec2::Zero(), currentSheet->texture->image.size.cast<float>() * scale);
			grid->setSize(currentSheet->rect.size);
			grid->setPartitions(gridPartitions);
		}
		gridLineShader->use();
		gridLineShader->setMat4("projection", Mat4::Orthographic(0, game->window->size[0], 0, game->window->size[1], 0, 1));
		grid->draw(gridLineShader);
	}
};