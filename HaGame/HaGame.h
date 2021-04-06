// HaGame3D_v1.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Hypercube.h"
#include "Math/Functions.h"
#include "Math/Quaternion.h"

#include "Graphics/OpenGL.cpp"
#include "Graphics/Monitors.h"
#include "Graphics/Window.cpp"
#include "Graphics/Shader.cpp"
#include "Graphics/ShaderProgram.cpp"
#include "Graphics/Mesh.h"
#include "Graphics/Camera.h"
#include "Graphics/PerspectiveCamera.hpp"
#include "Graphics/Primitives.h"
#include "Graphics/Texture.cpp"
#include "Graphics/Sprite.h"
#include "Graphics/Particle.h"
#include "Graphics/Routines.cpp"
#include "Graphics/Line.h"

#include "Input/Device.h"
#include "Input/Devices/Gamepad.h"
#include "Input/Devices/KeyboardMouse.h"

#include "Utils/File.cpp"
#include "Utils/FileSystem.cpp"
#include "Utils/Clock.h"
#include "Utils/Aliases.h"
#include "Utils/String.cpp"

#include "Physics/RigidBody.h"

#include "Core/Game.cpp"
#include "Core/ResourceManager.cpp"
#include "Core/Transform.cpp"
#include "Core/ECS/Entity.h"
#include "Core/ECS/EntityManager.h"
#include "Core/ECS/System.h"
#include "Core/ECS/Component.h"
#include "Core/Scene.h"

#include "Vendor/imgui/imgui.h"
#include "Vendor/imgui/imgui.cpp"
#include "Vendor/imgui/imgui_draw.cpp"
#include "Vendor/imgui/imgui_tables.cpp"
#include "Vendor/imgui/imgui_widgets.cpp"
#include "Vendor/imgui/imgui_demo.cpp"
#include "Vendor/imgui/backends/imgui_impl_sdl.h"
#include "Vendor/imgui/backends/imgui_impl_sdl.cpp"
#include "Vendor/imgui/backends/imgui_impl_opengl3.h"
#include "Vendor/imgui/backends/imgui_impl_opengl3.cpp"

#include "Vendor/entt/entt.hpp"

#include "Vendor/stb/stb_image.cpp"

// TODO: Reference additional headers your program requires here.
