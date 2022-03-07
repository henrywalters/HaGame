#ifndef SH_HOME_H
#define SH_HOME_H

#include "./../../HaGame/HaGame.h"

using namespace hagame::ecs;
using namespace hagame::graphics;
using namespace hagame::ui;

class Home : public hagame::Scene {
public:

	const String TITLE = "SPACE\nHORROR";
	const String SUBTITLE = "A HAK Studios Game";

	const String BUILD_TAG = "Build: 0.0.1";

	const String PRIMARY_FONT = "Fonts/horroroid/horroroid.ttf";
	const String SECONDARY_FONT = "Fonts/arial.ttf";

	const Array<Option> MENU_OPTIONS = {
		Option("Quit", "quit"),
		Option("Settings", "settings"),
		Option("Level Editor", "editor"),
		Option("Play Sandbox", "sandbox"),
		Option("New Game", "new")
	};

	Font* pFont;
	Font* smallFont;
	Font* largeFont;

	Vec2 mousePos;
	Vec2 screenCenter;

	Ptr<Entity> cameraEntity;
	Ptr<OrthographicCamera> orth;

	Ptr<Entity> menuEntity;

	float iR = 5;
	float oR = 12.5;

	void onSceneInit() {
		addSystem<RenderSystem>();
		addSystem<UISystem>();

		pFont = game->resources->loadFont("primary", PRIMARY_FONT, 124);
		smallFont = game->resources->loadFont("secondary_small", SECONDARY_FONT, 20);
		largeFont = game->resources->loadFont("secondary_large", SECONDARY_FONT, 36);
	}

	void onSceneActivate() {

		ecs.entities.clear();

		game->audio->play(game->resources->getAudioSample("creepy_home"), 1.0f);

		game->window->clearColor = Color("#1f1f1f");
		screenCenter = game->window->size * 0.5f;
		game->input.keyboardMouse.hideCursor();

		cameraEntity = addEntity();
		auto camera = cameraEntity->addComponent<CameraComponent>();
		orth = std::make_shared<OrthographicCamera>(game->window->size);
		orth->centered = false;
		camera->camera = orth;

		auto buildTagSize = smallFont->calculateMessageSize(BUILD_TAG);

		auto titleSize = pFont->calculateMessageSize(TITLE);
		auto subTitleSize = largeFont->calculateMessageSize(SUBTITLE);
		auto totalY = (titleSize[1] + subTitleSize[1]);

		auto grid = addEntity();
		grid->addComponent<Grid>(game->window->size, 2, 1);

		auto titleCol = grid->getComponent<Grid>()->addGrid(0, 0, 1, 4);
		auto menuCol = grid->getComponent<Grid>()->addGrid(1, 0, 1, 3);

		auto title = addLabel(Vec2::Zero(), TITLE, pFont, Color::red(), TextHAlignment::Center, TextVAlignment::Center);
		auto subtitle = addLabel(Vec2::Zero(), SUBTITLE, largeFont, Color::white(), TextHAlignment::Center, TextVAlignment::Center);

		menuEntity = addEntity();
		auto menu = menuEntity->addComponent<MultiSelect>(this, largeFont, Vec2(300, 300), MENU_OPTIONS);

		menu->onHover = [this](String selected) {
			game->audio->play(game->resources->getAudioSample("thud"), 0.5f);
		};

		menu->onSelect = [this](String selected) {
			if (selected == "quit") {
				game->running = false;
			}

			if (selected == "sandbox") {
				game->scenes.activate("demo");
			}

			if (selected == "editor") {
				game->scenes.activate("editor");
			}
		};

		menuCol->addEntity(0, 1, menuEntity.get(), [](Entity* menu) { return menu->getComponent<MultiSelect>(); });

		auto buildTag = addLabel(Vec2(game->window->size[0], 0) - buildTagSize * Vec2(1, 0), BUILD_TAG, smallFont, Color::white(), TextHAlignment::Left, TextVAlignment::Top);

		menuCol->addEntity(0, 0, buildTag.get(), [](Entity* tag) { return tag->getComponent<TextRenderer>(); });
		menuCol->setAnchor(0, 0, AnchorType::BottomRight);

		titleCol->addEntity(0, 2, title.get(), [](Entity* title) { return title->getComponent<TextRenderer>(); });
		titleCol->addEntity(0, 1, subtitle.get(), [](Entity* title) { return title->getComponent<TextRenderer>(); });

		int stars = 1000;

		for (int i = 0; i < stars; i++) {
			auto star = addEntity();
			auto size = game->random.real(1.0f, 5.0f);
			auto pos = Vec3(game->random.real<double>(0, game->window->size[0]), game->random.real<double>(0, game->window->size[1]), 100);
			auto quad = star->addComponent<QuadRenderer>(size);
			auto color = Color(255, 0, 0, game->random.integer(0, 50));
			star->move(pos);
			quad->color = color;
			quad->shader = game->resources->getShaderProgram("color");
		}

		game->input.keyboardMouse.mouseEvents.subscribe(hagame::input::devices::MouseEvents::Moved, [this](hagame::input::devices::MouseEvent e) {
			mousePos = e.mousePos;
			mousePos[1] = game->window->size[1] - mousePos[1];
			});

		game->input.windowEvents.subscribe(hagame::input::WindowEvents::Resized, [this, grid](hagame::input::WindowEvent e) {
			orth->size = e.data;
			grid->getComponent<Grid>()->setSize(e.data);
			game->window->setViewport(Rect(Vec2::Zero(), e.data));
		});
	}

	void onSceneDeactivate() {

	}

	void onSceneUpdate(double dt) {
		auto shader = game->resources->getShaderProgram("batch_line");

		if (!game->input.usingGamepad(0)) {
			drawCrosshairs(mousePos.resize<3>() + Vec3(0, 0, -1), iR, oR, Color::green(), 2.0f);
			menuEntity->getComponent<MultiSelect>()->setSelectionByMouse(mousePos, game->input.keyboardMouse.mouse.leftPressed);
		}
		else {
			menuEntity->getComponent<MultiSelect>()->setActive(true);
			menuEntity->getComponent<MultiSelect>()->setSelectionByDeviceState(game->input.player(0));
		}

		
		shader->use();

		shader->setMVP(Mat4::Identity(), Mat4::Identity(), orth->getProjMatrix(cameraEntity->getPos()));
		lineBuffer.draw();

		shader = game->resources->getShaderProgram("color");
		shader->use();
		shader->setMVP(Mat4::Identity(), Mat4::Identity(), orth->getProjMatrix(cameraEntity->getPos()));

		DEBUG_SHADER = game->resources->getShaderProgram("color");
	}

	void onSceneAfterUpdate() {

	}

	Ptr<Entity> addQuad(Vec3 pos, Vec2 size, Color color) {
		auto entity = addEntity();
		entity->move(pos);
		auto quad = entity->addComponent<QuadRenderer>(size);
		quad->color = color;
		quad->shader = game->resources->getShaderProgram("color");
		return entity;
	}

	Ptr<Entity> addButton(Vec2 pos, Vec2 size, String message, Font* font, Color color, Color hoverColor, TextHAlignment alignmentH, std::function<void(Ptr<Entity>)> onClick) {
		auto entity = addEntity();
		auto textEntity = addChild(entity);
		auto btnEntity = addChild(entity);

		entity->setPos(pos);
		entity->move(Vec3::Face(1.0f));

		auto text = textEntity->addComponent<TextRenderer>();
		auto messageSize = font->calculateMessageSize(message);
		textEntity->move(messageSize * -0.5f);
		text->color = color;
		text->font = font;
		text->message = message;
		text->shader = game->resources->getShaderProgram("text");
		text->fontSize = font->getFontSize();
		text->alignmentH = alignmentH;
		
		auto button = btnEntity->addComponent<Button>(size);
		button->onClick = onClick;
		
		button->onEnter = [hoverColor, this](Ptr<Entity> entity) {
			game->audio->play(game->resources->getAudioSample("thud"), 0.4f);
			entity->parent->getComponentInChildren<TextRenderer>()->color = hoverColor;
		};
		button->onLeave = [color](Ptr<Entity> entity) {
			entity->parent->getComponentInChildren<TextRenderer>()->color = color;
		};
		
		return entity;
	}

	Ptr<Entity> addLabel(Vec2 pos, String message, Font* font, Color color, TextHAlignment alignmentH = TextHAlignment::Center, TextVAlignment alignmentV = TextVAlignment::Center) {
		auto entity = addEntity();
		entity->setPos(pos);
		entity->move(Vec3::Face(1.0f));
		auto text = entity->addComponent<TextRenderer>();
		text->color = color;
		text->font = font;
		text->message = message;
		text->shader = game->resources->getShaderProgram("text");
		text->fontSize = font->getFontSize();
		text->alignmentH = alignmentH;
		text->alignmentV = alignmentV;
		return entity;
	}
};

#endif
