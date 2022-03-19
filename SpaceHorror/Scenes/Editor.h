#ifndef SH_EDITOR_H
#define SH_EDITOR_H

#include "./../../HaGame/HaGame.h"
#include "./../Common/Colors.h"

using namespace hagame::ecs;
using namespace hagame::graphics;
using namespace hagame::ui;

class Editor : public hagame::Scene {
public:

	Vec2 mousePos;
	Vec2 dragStartPos;
	Vec2 startOffset;
	bool isDragging = false;

	RawPtr<Entity> cameraEntity;
	Ptr<OrthographicCamera> orth;

	RawPtr<Entity> editorEntity;

	void onSceneInit() {
		addSystem<RenderSystem>();
		addSystem<UISystem>();
	}

	void onSceneActivate() {

		cameraEntity = addEntity().get();
		auto camera = cameraEntity->addComponent<CameraComponent>();
		orth = std::make_shared<OrthographicCamera>(game->window->size);
		orth->centered = false;
		camera->camera = orth;

		editorEntity = addEntity().get();
		auto editorGrid = editorEntity->addComponent<Grid>(Vec2(1000), 50, 50);
		editorGrid->display = true;
		editorGrid->color = PRUSSIAN_BLUE;

		auto gridEntity = addEntity();
		auto grid = gridEntity->addComponent<Grid>(game->window->size, 12, 12);
		// grid->display = true;

		Range range = Range(Vec2Int(0, 2), Vec2Int(10, 10));
		Range sidebarRange = Range(Vec2Int(10, 0), Vec2Int(12, 12));
		Range palletRange = Range(Vec2Int(0, 0), Vec2Int(12, 2));

		//quad = addQuad(Vec2::Zero(), grid->getRange(range).size * 0.80f, Color::green());
		auto sidebar = addQuad(Vec2::Zero(), grid->getRange(sidebarRange).size, GRAY);
		auto pallet = addQuad(Vec2::Zero(), grid->getRange(palletRange).size, GRAY);

		//quad->getComponent<QuadRenderer>()->offset[2] = 10;
		sidebar->getComponent<QuadRenderer>()->offset[2] = 5;
		pallet->getComponent<QuadRenderer>()->offset[2] = 5;

		grid->addEntity(range, editorEntity, [](Entity* quad) { return quad->getComponent<Grid>(); });
		grid->addEntity(sidebarRange, sidebar.get(), [](Entity* quad) { return quad->getComponent<QuadRenderer>(); });
		grid->addEntity(palletRange, pallet.get(), [](Entity* quad) { return quad->getComponent<QuadRenderer>(); });

		game->input.keyboardMouse.mouseEvents.subscribe(hagame::input::devices::MouseEvents::Moved, [this](hagame::input::devices::MouseEvent e) {
			mousePos = e.mousePos;
			mousePos[1] = game->window->size[1] - mousePos[1];
		});

		game->input.windowEvents.subscribe(hagame::input::WindowEvents::Resized, [this, grid, pallet, sidebar, sidebarRange, palletRange](hagame::input::WindowEvent e) {
			orth->size = e.data;
			grid->setSize(e.data);
			sidebar->getComponent<QuadRenderer>()->quad->setSize(grid->getRange(sidebarRange).size, false);
			pallet->getComponent<QuadRenderer>()->quad->setSize(grid->getRange(palletRange).size, false);
			game->window->setViewport(Rect(Vec2::Zero(), e.data));
		});
	}

	void onSceneUpdate(double dt) {
		if (game->input.keyboardMouse.startPressed) {
			game->scenes.activate("home");
		}
		else {
			auto qr = editorEntity->getComponent<Grid>();
			qr->offset += game->input.player(0).lAxis * -1;

			if (game->input.keyboardMouse.keyboard.numbers[1]) {
				auto delta = qr->offset.div(qr->getSize());
				qr->setSize(qr->getSize() * (1 - dt));
				qr->offset = qr->getSize().prod(delta);
			}
			else if (game->input.keyboardMouse.keyboard.numbers[2]) {
				auto delta = qr->offset.div(qr->getSize());
				qr->setSize(qr->getSize() * (1 + dt));
				qr->offset = qr->getSize().prod(delta);
			}
			else if (qr->getGridRect().contains(mousePos)) {

				if (game->input.player(0).rTrigger && !isDragging) {
					isDragging = true;
					dragStartPos = mousePos;
					startOffset = qr->offset;
				}

				auto idx = qr->getCellIdx(mousePos);
				auto cell = qr->getCell(idx);
				cell.size *= 1.2f;
				cell.pos += editorEntity->getPos() - qr->offset - cell.size * 0.1f;
				drawRect(cell, PRIMARY, DEBUG_SHADER, 1.0f);
			}

			if (!game->input.player(0).rTrigger) {
				isDragging = false;
			}

			if (isDragging) {
				auto dragDelta = mousePos - dragStartPos;
				qr->offset = startOffset + dragDelta;
			}

			drawCrosshairs(mousePos, 5, 15, SECONDARY, 2.0f);

			auto shader = game->resources->getShaderProgram("batch_line");

			shader->use();

			shader->setMVP(Mat4::Identity(), Mat4::Identity(), orth->getProjMatrix(cameraEntity->getPos()));
			lineBuffer.draw();

			shader = game->resources->getShaderProgram("color");
			shader->use();
			shader->setMVP(Mat4::Identity(), Mat4::Identity(), orth->getProjMatrix(cameraEntity->getPos()));

			DEBUG_SHADER = game->resources->getShaderProgram("color");
		}


	}

	void onSceneDeactivate() {
		std::cout << "EDITOR DEACTIVATED\n";
		ecs.entities.clear(); 
		// lineBuffer.clear();
	}

	Ptr<Entity> addQuad(Vec3 pos, Vec2 size, Color color) {
		auto entity = addEntity();
		entity->move(pos);
		auto quad = entity->addComponent<QuadRenderer>(size);
		quad->quad->setOrigin(Vec2::Zero());
		quad->color = color;
		quad->shader = game->resources->getShaderProgram("color");
		return entity;
	}
};

#endif