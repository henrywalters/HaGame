// HaGame3D_v1.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Hypercube.h"
#include "Math/Functions.h"
#include "Graphics/Monitors.h"
#include "Graphics/Window.h"
#include "Graphics/Shader.h"
#include "Graphics/ShaderProgram.h"
#include "Graphics/Mesh.h"
#include "Graphics/Camera.h"
#include "Graphics/Primitives.h"
#include "Graphics/Texture.h"
#include "Graphics/Sprite.h"
#include "Graphics/Particle.h"
#include "Graphics/Routines.h"
#include "Input/Device.h"
#include "Input/Devices/Gamepad.h"
#include "Input/Devices/KeyboardMouse.h"
#include "Utils/FileSystem.h"
#include "Utils/Clock.h"
#include "Utils/Aliases.h"
#include "Core/ResourceManager.h"
#include "Core/Transform.h"

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

// TODO: Reference additional headers your program requires here.
