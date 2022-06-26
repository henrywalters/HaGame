#ifndef EDITOR_TOOLS_H
#define EDITOR_TOOLS_H

#include "../../HaGame/HaGame.h"

static int LastEditorToolId = 0;

struct EditorTool {
	int id;
	String label;
	std::function<void()> onActivate = []() {};
	std::function<void(Vec2)> onUpdate = [](Vec2 pos) {};
	std::function<void(Vec2)> onPlace = [](Vec2 pos) {};
	std::function<void(Vec2)> onRemove = [](Vec2 pos) {};
	std::function<void()> onMenuRender = []() {};

	EditorTool(String _label) :
		id(LastEditorToolId++),
		label(_label) {}
};


#endif
