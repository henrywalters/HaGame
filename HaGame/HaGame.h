// HaGame3D_v1.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

#include "Utils/Memory.h"

#include "Math/Vector.h"
#include "Math/Matrix.h"
#include "Math/Hypercube.h"
#include "Math/Functions.h"
#include "Math/Quaternion.h"
#include "Math/Sample.h"
#include "Math/Perlin.h"
#include "Math/Triangle.h"
#include "Math/Line.h"
#include "Math/LineSegment.h"
#include "Math/Ray.h"
#include "Math/Plane.h"
#include "Math/Sphere.h"

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
#include "Graphics/Plane.h"
#include "Graphics/Components/AxisRenderer.h"
#include "Graphics/Components/BoundingBoxRenderer.h"
#include "Graphics/Components/RigidBodyRenderer.h"
#include "Graphics/Components/MeshRenderer.h"
#include "Graphics/Components/CameraComponent.h"
#include "Graphics/Components/LightComponent.h"
#include "Graphics/Systems/RenderSystem.h"
#include "Graphics/Systems/CameraSystem.h"
#include "Graphics/Systems/LightingSystem.h"

#include "Input/Device.h"
#include "Input/Devices/Gamepad.h"
#include "Input/Devices/KeyboardMouse.h"

#include "Utils/File.cpp"
#include "Utils/FileSystem.cpp"
#include "Utils/Clock.h"
#include "Utils/Timer.h"
#include "Utils/Aliases.h"
#include "Utils/String.cpp"
#include "Utils/Random.h"

#include "Physics/Components/RigidBody.h"
#include "Physics/Components/SimpleMovement.h"
#include "Physics/Components/RotationMovement.h"
#include "Physics/Components/Collider.h"
#include "Physics/Systems/PhysicsSystem.h"
#include "Physics/Systems/MovementSystem.h"
#include "Physics/Systems/CollisionSystem.h"


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
